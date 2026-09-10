# Voltage monitoring on PA27 and PA26

`feedback_adc.c` configures PA27 (PINCM60, ADC0 channel 0) and PA26
(PINCM59, ADC0 channel 1) as analog inputs, disconnecting their digital circuitry.
The control task triggers one 12-bit sequence each second: PA27 is sampled first,
then PA26, with 125 us acquisition time for each. The pair is published only
when the second conversion finishes; these are sequential, not simultaneous,
measurements. A sequence timeout marks the pair invalid and reinitializes ADC0.

Both values travel in one measurement message to the communications task. The
DAC starts at nominal 0 V and changes only after a valid user command (0–2.5 V).
ADC sampling continues independently while you type. This is monitoring only:
the measurement does not regulate the DAC setting.

The ADC reference is the MCU supply, assumed to be 3300 mV. Set
`ADC_REFERENCE_MV` in `feedback_adc.h` to the measured supply for better accuracy.
PA27 uses `raw * reference_mV / 4096`, rounded to the nearest millivolt.
PA26 is displayed in mV with three decimal places, calculated from the raw
code without first rounding to whole mV.
The driver uses 125 us acquisition time at the existing 32 MHz ULPCLK.

Connect both measured circuit grounds to LaunchPad ground. A direct connection is valid only
while both PA27 and PA26 stay between ground and the MCU supply (nominally 3.3 V), including
startup and transitions. For higher LDO outputs, use a resistor divider sized
for the maximum output and scale the reported voltage by the divider ratio.
The current code reports voltage at each ADC pin with no divider compensation.

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
PA27:1.247V PA26:100.708mV I:41.152mA | DAC:1.250V | volts> 2.
```

Backspace edits; Ctrl-U or Ctrl-C clears the input. Invalid values leave the
DAC unchanged. See [DAC_README.md](DAC_README.md) for command and range details.

Actual readings depend on the LDO circuit and ADC/reference errors. Text goes
to the USB serial port, not the CCS build/debug console; no semihosting is used.
Opening the terminal after boot still shows subsequent readings every second.

`adcRaw[2]`, `adcMillivolts[2]`, `adcError`, and `dacError` are visible in the
debugger. Array index 0 is PA27; index 1 is PA26. On an ADC timeout, the terminal reports an error and the ADC is
reinitialized for the next reading. Code 4095 is marked `FULL`; it
cannot tell how far above the reference an input may be. DAC failures prevent
further DAC writes while ADC reporting continues.

References: [LaunchPad user guide](https://www.ti.com/lit/ug/slau947/slau947.pdf)
for pin routing and jumpers; [MSPM0G3519 data sheet](https://www.ti.com/lit/ds/symlink/mspm0g3519.pdf)
for ADC input limits.

The control task publishes samples to a one-item queue that retains the latest
reading. The communications task displays them; if it falls behind, intermediate
readings may be skipped rather than blocking the ADC schedule. See
[DAC_README.md](DAC_README.md#source-organization-and-tasks) for task ownership.

Use a terminal at least 140 columns wide to accommodate both readings, a full
input line and status indicators without wrapping. The normal idle line fits
in 80 columns.

## PA26 current-sense conversion

PA26 is the amplified current-sense voltage. PA27 remains the LDO output-voltage
measurement. The current displayed beside PA26 is the estimated **LDO output
current**, not the smaller current through the sense resistor.

Using the supplied circuit values:

- Current-sense ratio: I_sense = I_output / 5000.
- Sense resistor: 1000 ohms.
- Noninverting amplifier: gain = 1 + Rf/Rg = 1 + 10000/890 = 12.235955...

Thus `I_output[A] = V_PA26[V] * 5000 / (1000 * (1 + 10000/890))`.
Equivalently, `I_output[mA] = V_PA26[mV] * 0.40863177...`.
For example, 100 mV at PA26 represents about 40.863 mA of output current.

`current_sense.h` contains the ratio/resistor constants and conversion. It uses
64-bit integer intermediate values and rounds once to microamps, directly from
the raw ADC code. The display prints mA with three decimal places. At ADC code
4095, voltage is marked FULL and current SAT; the current is then a full-scale
estimate rather than a valid measurement of any larger signal. An ADC timeout
hides both voltages and current until a complete valid sample arrives.

This change removes display rounding; it does not add ADC resolution. With a
3.3 V reference, one 12-bit ADC count is 0.805664 mV, or about 0.329220 mA of
output current. Extra displayed digits do not imply that level of accuracy.
The conversion assumes ideal sense ratio, resistor values, amplifier gain and
zero offset. No offset calibration or averaging has been added.
