"""Adapter for the existing ANSI UART console in MSPM0_DC_Board/terminal.c."""
from dataclasses import dataclass
from decimal import Decimal
import re


def voltage_command(text: str) -> bytes:
    text = text.strip()
    if not re.fullmatch(r"(?:[0-9]+(?:\.[0-9]{0,3})?|\.[0-9]{1,3})", text):
        raise ValueError("Enter a voltage from 0 to 2.500, with at most 3 decimal places.")
    value = Decimal(text)
    if not 0 <= value <= Decimal("2.500"):
        raise ValueError("Voltage must be between 0 and 2.500 V.")
    # Clear any unfinished terminal input before sending a complete command.
    return b"\x15" + f"{value:.3f}\r".encode("ascii")


@dataclass(frozen=True)
class Status:
    pa27: str
    pa26: str
    current: str
    dac: str
    state: str


def parse_status(line: str) -> Status | None:
    match = re.fullmatch(r"(.+) \| DAC:([0-9]+\.[0-9]{3}V)( ERROR| pending)? \| volts> ", line)
    if not match:
        return None
    adc, dac, state = match.groups()
    state = (state or "ready").strip()
    if adc == "PA27/PA26: ADC timeout":
        return Status("—", "—", "—", dac, "ADC timeout / " + state)
    if adc == "PA27:-- PA26:--mV I:--mA":
        return Status("—", "—", "—", dac, "Waiting for ADC / " + state)
    readings = re.fullmatch(r"PA27:([0-9.]+V(?: FULL)?) PA26:([0-9.]+mV(?: FULL)?) I:([0-9.]+mA(?: SAT)?)", adc)
    if readings:
        return Status(*readings.groups(), dac, state)
    return None


class ConsoleDecoder:
    """Accept arbitrary chunks; redraws end in a prompt, not a newline."""
    def __init__(self):
        self.buffer = ""

    def feed(self, data: bytes) -> list[Status | str]:
        self.buffer += data.decode("ascii", errors="replace")
        events = []
        while True:
            boundary = re.search(r"\r|\n|\| volts> ", self.buffer)
            if boundary is None:
                break
            end = boundary.end() if boundary.group() == "| volts> " else boundary.start()
            line = self.buffer[:end].replace("\x1b[2K", "")
            self.buffer = self.buffer[boundary.end():]
            if line:
                events.append(parse_status(line) or line)
        # Recover even if the wrong UART sends a stream without delimiters.
        if len(self.buffer) > 4096:
            self.buffer = ""
            events.append("Unrecognized serial stream; check the selected UART.")
        return events
