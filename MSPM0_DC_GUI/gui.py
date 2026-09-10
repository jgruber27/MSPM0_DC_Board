"""Run with python gui.py, or python gui.py --demo."""
import argparse
import math
import time
import tkinter as tk
from tkinter import ttk, messagebox
from tkinter.scrolledtext import ScrolledText

from protocol import ConsoleDecoder, Status, voltage_command

try:
    import serial
    from serial.tools import list_ports
except ImportError:
    serial = None


class BoardGUI:
    def __init__(self, root, demo=False):
        self.root, self.demo = root, demo
        self.connection = None
        self.decoder = ConsoleDecoder()
        self.last_status = 0.0
        self.pending_since = None
        self.latest = None
        self.demo_voltage = 0.0
        self.next_demo = 0.0
        root.title("MSPM0 DC Board" + (" — DEMO" if demo else ""))
        root.geometry("850x570")
        root.minsize(720, 500)
        root.protocol("WM_DELETE_WINDOW", self.close)
        frame = ttk.Frame(root, padding=20)
        frame.pack(fill="both", expand=True)
        ttk.Label(frame, text="MSPM0 DC Board", font=("TkDefaultFont", 20, "bold")).pack(anchor="w")
        ttk.Label(frame, text="DAC0 control • PA27 voltage • PA26 current sense").pack(anchor="w", pady=(0, 16))
        ports = ttk.Frame(frame)
        ports.pack(fill="x")
        self.port = ttk.Combobox(ports, width=35)
        self.port.pack(side="left", fill="x", expand=True)
        self.refresh_button = ttk.Button(ports, text="Refresh ports", command=self.refresh_ports)
        self.refresh_button.pack(side="left", padx=8)
        self.connect_button = ttk.Button(ports, text="Connect", command=self.toggle_connection)
        self.connect_button.pack(side="left")
        self.link = tk.StringVar(value="Demo mode — simulated readings" if demo else "Disconnected • 115200 baud, 8N1")
        ttk.Label(frame, textvariable=self.link).pack(anchor="w", pady=10)

        readings = ttk.Frame(frame)
        readings.pack(fill="x", pady=10)
        self.values = {}
        for column, (key, title) in enumerate((("pa27", "PA27 voltage"), ("pa26", "PA26 sense voltage"), ("current", "Estimated output current"), ("dac", "Reported DAC setpoint"))):
            readings.columnconfigure(column, weight=1)
            box = ttk.LabelFrame(readings, text=title, padding=10)
            box.grid(row=0, column=column, sticky="nsew", padx=3)
            self.values[key] = tk.StringVar(value="—")
            ttk.Label(box, textvariable=self.values[key], font=("TkDefaultFont", 14)).pack()
        controls = ttk.LabelFrame(frame, text="DAC0 setpoint (0–2.500 V)", padding=12)
        controls.pack(fill="x", pady=10)
        self.voltage = ttk.Entry(controls, width=15)
        self.voltage.insert(0, "0.000")
        self.voltage.pack(side="left")
        self.voltage.bind("<Return>", lambda event: self.send_voltage())
        self.apply_button = ttk.Button(controls, text="Apply voltage", command=self.send_voltage)
        self.apply_button.pack(side="left", padx=10)
        self.zero_button = ttk.Button(controls, text="Set 0 V", command=lambda: self.send_voltage("0"))
        self.zero_button.pack(side="left")
        self.result = tk.StringVar(value="Waiting for board status")
        ttk.Label(frame, textvariable=self.result, wraplength=780).pack(anchor="w")
        ttk.Label(frame, text="Setpoint is open loop. Disconnecting leaves the board output unchanged.").pack(anchor="w", pady=(4, 12))
        self.log = ScrolledText(frame, height=10, state="disabled", wrap="word")
        self.log.pack(fill="both", expand=True)
        if demo:
            for widget in (self.port, self.refresh_button, self.connect_button):
                widget.configure(state="disabled")
        else:
            self.refresh_ports()
        self.update_controls()
        root.after(50, self.poll)

    def log_message(self, text):
        self.log.configure(state="normal")
        self.log.insert("end", time.strftime("%H:%M:%S ") + text + "\n")
        if int(self.log.index("end-1c").split(".")[0]) > 500:
            self.log.delete("1.0", "100.0")
        self.log.see("end")
        self.log.configure(state="disabled")

    def refresh_ports(self):
        if serial is None:
            self.link.set("Install pyserial: python -m pip install -r requirements.txt")
            return
        try:
            self.port.configure(values=[p.device for p in list_ports.comports()])
        except OSError as error:
            self.log_message(str(error))

    def toggle_connection(self):
        if self.connection:
            self.disconnect()
            return
        if serial is None:
            messagebox.showerror("Missing dependency", "Run: python -m pip install -r requirements.txt")
            return
        if not self.port.get().strip():
            messagebox.showerror("Select UART", "Select or enter the XDS110 Application/User UART port.")
            return
        try:
            self.connection = serial.Serial(self.port.get().strip(), 115200, timeout=0, write_timeout=0.2,
                                            xonxoff=False, rtscts=False, dsrdtr=False)
        except (OSError, serial.SerialException) as error:
            messagebox.showerror("Connection failed", str(error))
            return
        self.decoder = ConsoleDecoder()
        self.connect_button.configure(text="Disconnect")
        self.port.configure(state="disabled")
        self.link.set("Connected • waiting for firmware telemetry")
        self.log_message("Connected to " + self.connection.port)

    def disconnect(self):
        if self.connection:
            self.connection.close()
        self.connection = None
        self.pending_since = None
        self.latest = None
        self.last_status = 0
        self.connect_button.configure(text="Connect")
        self.port.configure(state="normal")
        self.link.set("Disconnected")
        self.result.set("Output state unknown while disconnected")
        for value in self.values.values():
            value.set("—")
        self.update_controls()

    def update_controls(self):
        ready = ((self.demo or self.connection is not None) and self.latest is not None
                 and time.monotonic() - self.last_status < 3
                 and self.pending_since is None
                 and "ERROR" not in self.latest.state and "pending" not in self.latest.state)
        for button in (self.apply_button, self.zero_button):
            button.configure(state="normal" if ready else "disabled")
        return ready

    def send_voltage(self, text=None):
        if not self.update_controls():
            return
        try:
            command = voltage_command(self.voltage.get() if text is None else text)
        except ValueError as error:
            messagebox.showerror("Invalid voltage", str(error))
            return
        requested = command[1:-1].decode("ascii")
        if self.demo:
            self.demo_voltage = float(requested)
            self.next_demo = 0
            self.result.set("Demo DAC0 set to " + requested + " V")
        else:
            try:
                if self.connection.write(command) != len(command):
                    raise OSError("Incomplete serial write; output state unknown")
            except (OSError, serial.SerialException) as error:
                self.log_message(str(error))
                self.disconnect()
                return
            self.pending_since = time.monotonic()
            self.result.set("Waiting for firmware acknowledgement…")
        self.log_message("Requested DAC0 " + requested + " V")
        self.update_controls()

    def handle_event(self, event):
        if isinstance(event, Status):
            self.latest = event
            self.last_status = time.monotonic()
            for key, value in self.values.items():
                value.set(getattr(event, key))
            self.link.set(("DEMO • simulated • " if self.demo else "Connected • ") + event.state)
            # A status redraw alone does not acknowledge our command.
        else:
            self.log_message(event)
            if event.startswith("DAC0 set to ") or any(marker in event for marker in (
                    "Output unchanged", "Command not applied", "Previous command pending",
                    "SPI transfer failed", "Serial data lost", "DAC initialization failed")):
                self.pending_since = None
                self.result.set(event)

    def poll(self):
        now = time.monotonic()
        if self.demo and now >= self.next_demo:
            self.next_demo = now + 1
            sense = max(0, self.demo_voltage * 80 + math.sin(now))
            self.handle_event(Status(f"{self.demo_voltage * .998:.3f}V", f"{sense:.3f}mV",
                                     f"{sense * .40863177:.3f}mA", f"{self.demo_voltage:.3f}V", "ready"))
        elif self.connection:
            try:
                data = self.connection.read(min(self.connection.in_waiting, 8192))
                for event in self.decoder.feed(data):
                    self.handle_event(event)
            except (OSError, serial.SerialException) as error:
                self.log_message("Serial connection lost: " + str(error))
                self.disconnect()
        if self.latest and now - self.last_status >= 3:
            self.link.set("Telemetry stale • check that the board is running")
            for value in self.values.values():
                value.set("—")
        if self.pending_since is not None and now - self.pending_since >= 3:
            self.result.set("No acknowledgement; output unconfirmed. Reconnect before retrying.")
        self.update_controls()
        self.root.after(50, self.poll)

    def close(self):
        if self.connection:
            self.connection.close()
        self.root.destroy()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--demo", action="store_true", help="Use simulated readings; no serial connection")
    args = parser.parse_args()
    root = tk.Tk()
    BoardGUI(root, demo=args.demo)
    root.mainloop()


if __name__ == "__main__":
    main()
