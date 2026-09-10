"""Host tests for the actual command parser, editor and rendering code."""
from pathlib import Path
import subprocess
import tempfile

project = Path(__file__).resolve().parents[1]
source = (project / 'terminal.c').read_text()
# Exercise actual firmware functions, with hardware writes replaced by a recorder.
functions = source[source.index('void Terminal_render(void)'):source.index('/* Communications task starts')]
functions += source[source.index('static bool terminalTakeResult(void)'):source.index('void Terminal_task(void *arg)')]
harness = r'''
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "voltage_input.h"
#include "feedback_adc.h"
#include "current_sense.h"
#define DAC_COMMAND_MAX_MV 2500U
#define DAC_FULL_SCALE_MV 2500U
#define COMMAND_SIZE 24U
static char command[COMMAND_SIZE], output[8192];
static unsigned int commandLength, writes;
static bool discardCommand, previousWasCR, adcValid, adcError, dacError;
static uint32_t dacSetMillivolts, adcMillivolts[FEEDBACK_ADC_COUNT];
static uint16_t adcRaw[FEEDBACK_ADC_COUNT], lastCode;
static bool transferOK = true, setpointPending, resultReady, requestBusy;
typedef struct { uint8_t channel; uint32_t millivolts; } SetpointCommand;
typedef struct { SetpointCommand command; uint16_t code; bool success; } SetpointResult;
static SetpointResult queued;
static bool App_submitSetpoint(uint8_t channel, uint32_t mv) {
    assert(channel == 0);
    if (requestBusy) return false;
    requestBusy = true;
    ++writes;
    lastCode = voltageToCode(mv, 2500);
    queued = (SetpointResult){{channel, mv}, lastCode, transferOK};
    return true;
}
static bool App_takeResult(SetpointResult *result) {
    if (!resultReady) return false;
    *result = queued;
    requestBusy = resultReady = false;
    return true;
}
static void terminalWrite(const char *s) {
    if (strlen(output) + strlen(s) < sizeof(output)) strcat(output, s);
}
'''
tests = r'''
static void send(const char *s) { while (*s) handleCharacter((unsigned char)*s++); }
static void complete(void) { resultReady = true; assert(terminalTakeResult()); }
static void applied(const char *s) { send(s); complete(); }
int main(void) {
    const char *valid[] = {"0", "2.5", "2.500", ".625", "1.", " 1.250 ", "000.001"};
    const uint32_t expected[] = {0, 2500, 2500, 625, 1000, 1250, 1};
    for (unsigned i = 0; i < sizeof(expected)/sizeof(expected[0]); ++i) {
        uint32_t mv = 9999;
        assert(voltageParse(valid[i], 2500, &mv) && mv == expected[i]);
    }
    const char *invalid[] = {"", " ", ".", "-1", "+1", "2.501", "3.3", "nan", "inf", "1e0", "1.0000", "1 2", "2V", "1..2", "999999999999999999999"};
    for (unsigned i = 0; i < sizeof(invalid)/sizeof(invalid[0]); ++i) {
        uint32_t mv = 9999;
        assert(!voltageParse(invalid[i], 2500, &mv) && mv == 9999);
    }
    assert(voltageToCode(0, 2500) == 0);
    assert(voltageToCode(625, 2500) == 1024);
    assert(voltageToCode(1250, 2500) == 2048);
    assert(voltageToCode(1875, 2500) == 3072);
    assert(voltageToCode(2500, 2500) == 4095);
    send("1.25"); assert(writes == 0); /* No write before Enter. */
    adcValid = true; adcMillivolts[0] = 1200; adcMillivolts[1] = 101; adcRaw[1] = 125;
    Terminal_render();
    assert(strcmp(command, "1.25") == 0);
    assert(strstr(output, "PA27:1.200V PA26:100.708mV I:41.152mA") && strstr(output, "volts> 1.25"));
    output[0]='\0'; adcRaw[1]=4095; Terminal_render();
    assert(strstr(output, "PA27:1.200V PA26:3299.194mV FULL I:1348.156mA SAT"));
    adcError=true; output[0]='\0'; Terminal_render();
    assert(strstr(output, "ADC timeout") && !strstr(output, "mA"));
    adcError=false;
    assert(FeedbackADC_rawToMicrovolts(0)==0);
    assert(CurrentSense_rawToMicroamps(0)==0);
    assert(FeedbackADC_rawToMicrovolts(1)==806);
    assert(CurrentSense_rawToMicroamps(1)==329);
    assert(CurrentSense_rawToMicroamps(1024)==337121);
    assert(CurrentSense_rawToMicroamps(2048)==674242);
    assert(CurrentSense_rawToMicroamps(4095)==1348156);
    output[0]='\0'; adcRaw[1]=1; Terminal_render();
    assert(strstr(output,"PA26:0.806mV I:0.329mA"));
    send("\r\n"); assert(writes == 1 && lastCode == 2048);
    assert(dacSetMillivolts == 0 && setpointPending);
    send("2\n"); assert(writes == 1); /* Pending command is not overwritten. */
    assert(!terminalTakeResult());
    complete(); assert(dacSetMillivolts == 1250 && !setpointPending);
    send("\n"); assert(writes == 1); /* Empty line. */
    send("3.3\n-1\nNaN\n1.0000\n"); assert(writes == 1);
    applied("2.4\b5\n"); assert(writes == 2 && lastCode == 4095);
    applied("2.2\1775\n"); assert(writes == 3 && lastCode == 4095);
    applied("1.23\0250\n"); assert(writes == 4 && lastCode == 0);
    send("2\003"); assert(writes == 4 && commandLength == 0);
    send("1111111111111111111111111111111111\n"); assert(writes == 4);
    send("1"); handleCharacter(-2); send(".5\n"); assert(writes == 4);
    applied(".5\n"); assert(writes == 5 && dacSetMillivolts == 500);
    send("1\033[A\n"); assert(writes == 5); /* Reject escape sequences. */
    transferOK = false;
    applied("1\n"); assert(writes == 6 && dacError && dacSetMillivolts == 500);
    send("2\n"); assert(writes == 6); /* No more writes on failed bus. */
    puts("PASS: parser, code conversion, editing, CRLF, overflow, errors, live redraw and asynchronous acknowledgements");
}
'''
with tempfile.TemporaryDirectory(prefix='dac-console-test-') as tmp:
    test = Path(tmp) / 'test.c'
    exe = Path(tmp) / 'test'
    test.write_text(harness + functions + tests)
    subprocess.run(['cc', '-std=c11', '-Wall', '-Wextra', '-Werror', '-I', str(project), str(test), '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
