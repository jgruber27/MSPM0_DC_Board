import pathlib
import sys
import unittest

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[1]))
from protocol import ConsoleDecoder, Status, voltage_command


class ProtocolTests(unittest.TestCase):
    def test_voltage_encoding(self):
        for text, expected in (("0", b"\x150.000\r"), (".125", b"\x150.125\r"),
                               (" 2.5 ", b"\x152.500\r")):
            self.assertEqual(voltage_command(text), expected)

    def test_invalid_voltage_never_encoded(self):
        for text in ("", "-1", "+1", "2.501", "3", "nan", "1e0", "1.0001", "1\r2", "."):
            with self.subTest(text=text), self.assertRaises(ValueError):
                voltage_command(text)

    def test_all_chunk_boundaries(self):
        wire = (b"\r\x1b[2KDAC0 set to 1.250 V (code 2048)\r\n"
                b"\r\x1b[2KPA27:1.247V PA26:100.708mV I:41.152mA | DAC:1.250V | volts> ")
        expected = ["DAC0 set to 1.250 V (code 2048)",
                    Status("1.247V", "100.708mV", "41.152mA", "1.250V", "ready")]
        for split in range(len(wire) + 1):
            decoder = ConsoleDecoder()
            self.assertEqual(decoder.feed(wire[:split]) + decoder.feed(wire[split:]), expected)
        decoder = ConsoleDecoder()
        self.assertEqual([event for byte in wire for event in decoder.feed(bytes([byte]))], expected)

    def test_faults_and_pending(self):
        decoder = ConsoleDecoder()
        events = decoder.feed(
            b"\r\x1b[2KPA27/PA26: ADC timeout | DAC:1.250V ERROR | volts> "
            b"\r\x1b[2KPA27:-- PA26:--mV I:--mA | DAC:0.000V pending | volts> "
            b"\r\x1b[2KPA27:3.299V FULL PA26:3299.194mV FULL I:1348.156mA SAT | DAC:2.500V | volts> ")
        self.assertEqual(events[0], Status("—", "—", "—", "1.250V", "ADC timeout / ERROR"))
        self.assertIn("pending", events[1].state)
        self.assertEqual(events[2].current, "1348.156mA SAT")

    def test_partial_typed_command_and_recovery(self):
        decoder = ConsoleDecoder()
        events = decoder.feed(b"\r\x1b[2KPA27:-- PA26:--mV I:--mA | DAC:0.000V | volts> 1.2\r\n")
        self.assertIsInstance(events[0], Status)
        self.assertEqual(events[1], "1.2")
        self.assertTrue(decoder.feed(b"x" * 5000))
        self.assertEqual(decoder.buffer, "")


if __name__ == "__main__":
    unittest.main()
