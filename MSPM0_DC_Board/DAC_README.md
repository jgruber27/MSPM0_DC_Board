# AD5672R SPI example

The FreeRTOS control task initializes VOUT0 to code 0 (nominally 0 V).
It holds the output until a valid decimal voltage is entered over USB serial.
Accepted commands are **0 to 2.5 V**, with up to three decimal places.
Type, for example, `1.250` and press Enter. Invalid commands leave the output
unchanged. The ADC reading refreshes every second while you type.

The terminal displays an updating line such as:

```text
PA27:1.247V PA26:100.708mV I:41.152mA | DAC:1.250V | volts> 2.
```

The partial command survives each refresh. Use an ANSI/VT100-compatible
terminal at **115200 baud, 8N1, no flow control, local echo off**. Backspace or
Delete edits the line; Ctrl-U or Ctrl-C cancels it. CR, LF, and CRLF are accepted
as Enter. Arrow keys and other escape sequences are not supported. Open the
XDS110 **Application/User UART** port, not the auxiliary port or build console.
UART0 TX is PA10 and RX is PA11: set both J12 and J13 to the XDS position
(pins 2–3), with the corresponding UART isolation jumpers on J101 installed.

| LaunchPad pin | DAC signal | MCU configuration |
| --- | --- | --- |
| PA13 | /SYNC | GPIO output, idle high |
| PB9 | SCLK | SPI1 clock, idle low |
| PA18 | SDI | SPI1 PICO (MOSI) output |
| PA7 | /RESET | GPIO output, idle high, pulsed low at initialization |
| PB7 | SDO | SPI1 POCI (MISO) input |

Use a common ground and connect VLOGIC to the LaunchPad's 3.3 V logic supply.
With the current 3.3 V VDD, keep GAIN grounded (gain 1), RSTSEL grounded for
a zero-scale reset, and /LDAC grounded as already wired. Do not leave those inputs floating. Supply VDD
within the DAC's specified range and provide its recommended decoupling.
The internal 2.5 V reference is enabled; do not drive VREF from another source.

For gain 1, `VOUT = 2.5 V * code / 4096`. Entered voltages are converted to
12-bit codes using that reference, not 3.3 V. Entering 2.5 selects code 4095:
the ideal output is approximately **2.4994 V**, before offset/gain errors.
Code zero gives nominal 0 V, subject to the DAC's zero-code offset.
`DAC_FULL_SCALE_MV` and `DAC_COMMAND_MAX_MV` in `app_tasks.h` match the current wiring.
The TSSOP's gain is set physically; changing a software constant cannot change
it. A 3.3 V output would require different supply/gain wiring.

These are DAC setpoints, not closed-loop LDO output targets. The ADC independently
measures PA27 and PA26; firmware does not adjust the DAC based on that reading.

Call `AD5672R_init()` once from a running task after `SYSCFG_DL_init()` and
before other tasks access the DAC. Call `AD5672R_write(channel, code)` for
channels 0–7 and 12-bit codes 0–4095. Calls are serialized with a FreeRTOS
mutex and must not be made from an ISR. The driver owns SPI1 exclusively.
It uses the existing 32 MHz BUSCLK to generate 1 MHz SPI mode 1, MSB first.
Update the clock divider and cycle delays if the system clock changes.

Every write sends command 3 plus the channel and the code shifted left four
bits, with /SYNC low for all 24 clocks. This command updates the output
independently of /LDAC. PB7 is configured as SPI input and its receive FIFO
is drained; register readback is not implemented. A successful write indicates
completion of the MCU transfer, not acknowledgement from the DAC.

On a peripheral timeout the driver disables SPI1 and requires reinitialization.
The example sets `dacError` on failure and rejects further voltage writes until
restart, while ADC monitoring continues. A failed transfer leaves the physical
output unconfirmed. Invalid channel/code arguments are rejected without a transfer.

The driver and example are ordinary CCS source files and require no SysConfig
regeneration. Build the `MSPM0_DC_Board` project with its FreeRTOS dependency,
load it onto the LaunchPad, and measure VOUT0 against ground. A logic analyzer
should show `30 00 00` at initialization. Entering `1.25` sends `30 80 00`
and entering `2.5` sends `30 FF F0`; there is no automatic switching.

Reference: [AD5672R/AD5676R data sheet, Rev. E](https://www.analog.com/media/en/technical-documentation/data-sheets/ad5672r_5676r.pdf),
serial interface, write/update commands, and internal reference setup sections.

UART reception uses an interrupt-driven 128-byte ring buffer; ADC conversions and
screen refreshes do not block reception. A line longer than 23 characters or a
receive error is rejected as a whole, so a truncated command is never applied.
After a receive error, press Enter and retype the voltage. Avoid bulk streaming;
this console is intended for human-entered commands.

## Source organization and tasks

- `main.c`: control/measurement task (`mainThread`), priority 2. Owns DAC/SPI and
  ADC hardware, initializes DAC0 to zero, applies queued commands, and samples
  PA27 and PA26 once per second.
- `terminal.c` / `terminal.h`: communications task (`Terminal_task`), priority 1.
  Owns UART0, command editing/validation and screen output. UART reception is
  interrupt-driven and feeds its existing byte buffer.
- `app_tasks.c` / `app_tasks.h`: creates both tasks and their queues before the
  scheduler starts; defines setpoint/result/measurement messages and voltage limits.
- `feedback_adc.c` / `feedback_adc.h`: PA27/PA26/ADC0 setup and voltage conversion.
- `current_sense.h`: PA26-to-output-current conversion using the sense ratio and
  amplifier resistor values.
- `ad5672r.c` / `ad5672r.h`: DAC SPI protocol and hardware initialization.
- `main_freertos.c`: hardware startup, task creation and FreeRTOS hooks.

The two application tasks use native FreeRTOS static allocation: the control
stack is 1024 bytes and the communications stack is 2048 bytes. Priorities are
relative to idle (0); larger numbers have higher priority. There is no POSIX
application-thread wrapper now. FreeRTOS's own idle/timer tasks remain.

A command queue sends a channel and requested millivolts from communications to
control. A result queue carries the SPI result back. One command may be in flight:
a second Enter before acknowledgement is rejected with a message, never silently
substituted for the first command. The display marks the request `pending` and
updates its DAC setpoint only after a successful response. Success means the MCU
completed the SPI transfer; it is not readback confirmation from the DAC.

A separate one-item measurement mailbox always contains the latest ADC sample.
The control task overwrites it without waiting for the terminal, so slow text
output cannot fill a telemetry queue and delay sampling. Intermediate readings
can be skipped by a stalled terminal. The console waits for the control task's
initialization report before accepting input.

The control task blocks on the command queue until either a command arrives or
the next ADC deadline expires. It checks the ADC deadline between commands, so
command traffic cannot indefinitely postpone sampling. A sample can be delayed
by a transfer already in progress or higher-priority interrupts; this is not a
hard real-time sampling guarantee. UART formatting and TX run only in the lower
priority communications task. Its input/display service interval is about 10 ms.
The task-owned state is copied through queues; volatile debug variables are
written only by the communications task, not used for cross-task synchronization.

This implements the task separation for the existing single DAC output and two ADC
inputs. Channel 0 is the only accepted channel. The future 24-channel mux, scan
sequence and GUI protocol are not implemented. They can extend the control task
and message structures without moving hardware access into the terminal task.

Validation commands:

```sh
python3 MSPM0_DC_Board/tests/test_voltage_console.py
python3 MSPM0_DC_Board/tests/test_control_tasks.py
python3 MSPM0_DC_Board/tests/test_feedback_adc.py
```

The host tests exercise asynchronous acknowledgements, queue backpressure,
startup at zero, ADC scheduling under command traffic, failure handling and tick
counter wrap. Hardware/RTOS timing still requires testing on the LaunchPad.
