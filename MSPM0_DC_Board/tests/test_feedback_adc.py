"""Verify the real ADC reader waits for both inputs and publishes one valid pair."""
from pathlib import Path
import subprocess
import tempfile

project = Path(__file__).resolve().parents[1]
source = (project/'feedback_adc.c').read_text()
reader = source[source.index('bool FeedbackADC_read('):]
harness = r'''
#include <assert.h>
#include <stdio.h>
#include "feedback_adc.h"
#define ADC0 0
#define ADC_REFERENCE_MV 3300U
#define ADC_POLL_LIMIT 8U
#define DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED 1U
#define DL_ADC12_INTERRUPT_MEM1_RESULT_LOADED 2U
#define DL_ADC12_MEM_IDX_0 0
#define DL_ADC12_MEM_IDX_1 1
static unsigned polls, reads, cleared;
static bool timeout;
static uint16_t results[2] = {1024, 3072};
static void DL_ADC12_clearInterruptStatus(int adc, unsigned mask) {
    assert(adc==ADC0); cleared=mask;
}
static void DL_ADC12_enableConversions(int adc) { assert(adc==ADC0); }
static void DL_ADC12_startConversion(int adc) { assert(adc==ADC0); polls=reads=0; }
static unsigned DL_ADC12_getRawInterruptStatus(int adc, unsigned mask) {
    assert(adc==ADC0 && mask==DL_ADC12_INTERRUPT_MEM1_RESULT_LOADED);
    ++polls;
    /* Channel 0 finishes first; channel 1 is not ready until the third poll. */
    unsigned status = (!timeout && polls>=3) ? 3U : 1U;
    return status & mask;
}
static uint16_t DL_ADC12_getMemResult(int adc, unsigned index) {
    assert(adc==ADC0 && polls>=3 && !timeout && index==reads);
    ++reads; return results[index];
}
'''
tests = r'''
int main(void) {
    uint16_t raw[2]={11,22}; uint32_t mv[2]={33,44};
    assert(FeedbackADC_read(raw,mv));
    assert(cleared==3 && reads==2 && raw[0]==1024 && raw[1]==3072);
    assert(mv[0]==825 && mv[1]==2475);
    results[0]=0; results[1]=4095;
    assert(FeedbackADC_read(raw,mv) && mv[0]==0 && mv[1]==3299);
    timeout=true;
    assert(!FeedbackADC_read(raw,mv) && reads==0);
    assert(raw[0]==0 && raw[1]==4095 && mv[0]==0 && mv[1]==3299);
    puts("PASS: two-channel completion, channel order, voltage scaling and timeout preservation");
}
'''
with tempfile.TemporaryDirectory(prefix='feedback-adc-test-') as tmp:
    test=Path(tmp)/'test.c'; exe=Path(tmp)/'test'
    test.write_text(harness+reader+tests)
    subprocess.run(['cc','-std=c11','-Wall','-Wextra','-Werror','-I',str(project),str(test),'-o',str(exe)],check=True)
    subprocess.run([str(exe)],check=True)
