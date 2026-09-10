# LDO output monitoring on PA27

`feedback_adc.c` configures PA27 (PINCM60) as ADC0 channel 0, with its digital input
and output circuitry disconnected. It performs a software-triggered 12-bit
conversion once per second and sends the measured pin voltage over UART0.
The DAC starts at nominal 0 V and changes only after a valid user voltage
command (0–2.5 V). ADC sampling continues independently while you type.
This is monitoring only: the measurement does not regulate the DAC setting.

The ADC reference is the MCU supply, assumed to be 3300 mV. Set
`ADC_REFERENCE_MV` in `feedback_adc.c` to the measured supply for better accuracy.
Conversion uses `raw * reference_mV / 4096`, rounded to the nearest millivolt.
The driver uses 125 us acquisition time at the existing 32 MHz ULPCLK.

Connect the LDO ground to LaunchPad ground. A direct connection is valid only
while PA27 stays between ground and the MCU supply (nominally 3.3 V), including
startup and transitions. For higher LDO outputs, use a resistor divider sized
for the maximum output and scale the reported voltage by the divider ratio.
The current code reports voltage at PA27 with no divider compensation.

## Serial terminal

1. Build and load `MSPM0_DC_Board` in CCS, then run/resume the target.
2. Connect the LaunchPad's XDS110 USB port. UART0 TX uses PA10 through the
   onboard XDS110 serial bridge. Set J12 to the XDS position (pins 2–3) and
   keep the corresponding UART isolation jumper on J101 installed.
3. Set J13 to the XDS position (pins 2–3) for PA11/UART0 RX and keep its
   UART isolation jumper installed so voltage commands can reach the MCU.
4. Open the **XDS110 Class Application/User UART** port in a serial terminal
   (the CCS serial terminal or another terminal program). Select **115200 baud,
   8 data bits, no parity, 1 stop bit, no flow control, local echo off**. Use
   an ANSI/VT100-compatible terminal. On Linux the device
   usually appears as `/dev/ttyACM*`; use the XDS110 application UART, not the
   auxiliary port. Type a voltage such as `1.250` and press Enter.

The same status/prompt line refreshes once per second and whenever you edit the
command. For example, while typing a new setpoint:

```text
ADC: 1.247 V | DAC set: 1.250 V | volts> 2.
```

Backspace edits; Ctrl-U or Ctrl-C clears the input. Invalid values leave the
DAC unchanged. See [DAC_README.md](DAC_README.md) for command and range details.

Actual readings depend on the LDO circuit and ADC/reference errors. Text goes
to the USB serial port, not the CCS build/debug console; no semihosting is used.
Opening the terminal after boot still shows subsequent readings every second.

`adcRaw`, `adcMillivolts`, `adcError`, and `dacError` are also visible in the
debugger. On an ADC timeout, the terminal reports an error and the ADC is
reinitialized for the next reading. Code 4095 is marked `FULL`; it
cannot tell how far above the reference an input may be. DAC failures prevent
further DAC writes while ADC reporting continues.

References: [LaunchPad user guide](https://www.ti.com/lit/ug/slau947/slau947.pdf)
for pin routing and jumpers; [MSPM0G3519 data sheet](https://www.ti.com/lit/ds/symlink/mspm0g3519.pdf)
for ADC input limits.
