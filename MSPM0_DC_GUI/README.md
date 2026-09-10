# MSPM0 DC Board GUI

Python desktop GUI for the existing firmware in the sibling `MSPM0_DC_Board`
folder. No firmware changes or CCS project imports are required. Uses Tkinter
and pyserial, with a separate console protocol adapter in `protocol.py`.

## Run

From this folder, on Linux:

```sh
python3 -m venv .venv
source .venv/bin/activate
python -m pip install -r requirements.txt
python gui.py
```

Tkinter must be installed in your Python distribution. On Ubuntu/Debian, the
system packages are `python3-tk` and `python3-venv`. A graphical desktop session
is required. On Windows, use `py -m venv .venv`, then
`.venv\Scripts\activate`, followed by the same pip and GUI commands.

To develop without hardware (pyserial is optional for this mode):

```sh
python3 gui.py --demo
```

Demo readings are simulated and do not model the connected circuit.

## Connect and control

1. Load and run the existing firmware in CCS.
2. Close any serial terminal using the board's port.
3. Select the XDS110 **Application/User UART** device and click **Connect**.
   You can type a device path or COM port into the port field. Communication
   uses 115200 baud, 8 data bits, no parity, one stop bit, no flow control.
4. Wait for live readings, then enter 0–2.500 V (up to three decimal places)
   and click **Apply voltage** or press Enter. **Set 0 V** sends a zero setpoint.

UART routing is the same as the firmware terminal: PA10 TX, PA11 RX, J12/J13
in the XDS position (2–3), and UART isolation jumpers on J101 installed. See
[firmware wiring and behavior](../MSPM0_DC_Board/DAC_README.md).

The GUI displays PA27 voltage, PA26 sense voltage, estimated output current,
and the firmware-reported DAC setpoint. FULL/SAT and ADC/DAC errors remain
visible. Measurements older than three seconds are hidden and commands disabled.
Requests wait for the firmware result; an acknowledgement timeout requires
reconnection before retrying unless a delayed result arrives. Commands are
never retried automatically. Opening a connection sends no voltage command.
Disconnecting or closing the GUI leaves the board output unchanged.

Only DAC channel 0 is exposed by the existing console. Setpoints are open loop,
not regulated LDO output targets. Firmware success means the MCU completed its
SPI transfer, not DAC readback. The 2.500 V request saturates at DAC code 4095.

## Development

`gui.py` handles the desktop interface and nonblocking serial reads using Tk's
timer. `protocol.py` handles fragmented ANSI console redraws (which do not end
with newlines), messages, and voltage validation. It sends Ctrl-U to clear any
unfinished console input before each voltage command. This adapter depends on
the current strings in `terminal.c`; update it if that console format changes.
A future structured telemetry protocol can replace the adapter independently.

Run host protocol tests without hardware or third-party packages:

```sh
python3 -m unittest discover -s tests -v
```

Hardware validation: connect and confirm readings agree with the firmware
terminal; apply a known setpoint and check the acknowledgement and measured
output; unplug USB and confirm controls disable. The GUI does not automatically
perform these hardware operations.
