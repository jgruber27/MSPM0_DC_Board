"""Run the real control loop with simulated time, queues and peripheral calls."""
from pathlib import Path
import subprocess
import tempfile

project = Path(__file__).resolve().parents[1]
source = (project / 'main.c').read_text()
control = source[source.index('void mainThread(void *arg0)'):]
header = (project / 'app_tasks.h').read_text()
types = header[header.index('typedef struct'):header.index('/* Created before scheduling')]
app = (project / 'app_tasks.c').read_text()
transport = app[app.index('bool App_submitSetpoint'):]
harness = r'''
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include "voltage_input.h"
#include "feedback_adc.h"
#define DAC_COMMAND_MAX_MV 2500U
#define DAC_FULL_SCALE_MV 2500U
#define pdTRUE 1
#define pdMS_TO_TICKS(ms) (ms)
typedef uint32_t TickType_t;
static const int appCommandQueue=1, appResultQueue=2, appMeasurementQueue=3;
'''
stubs = r'''
static bool requestPending, commandFull, resultFull;
static SetpointCommand queuedCommand;
static SetpointResult queuedResult;
static Measurement latest;
static TickType_t ticks, startTicks, sampleTimes[3];
static unsigned int samples, replies, writes, mode, eventIndex, invalidReplies;
static bool simulateLoop, initOK=true, failWrite;
static uint16_t lastCode;
static jmp_buf done;
static const SetpointCommand events[] = {{0,2500},{0,3300},{1,500},{0,1000},{0,500}};
static TickType_t xTaskGetTickCount(void) { return ticks; }
static int xQueueSend(int queue, const void *item, TickType_t wait) {
    assert(queue==appCommandQueue && wait==0);
    if(commandFull) return 0;
    queuedCommand=*(const SetpointCommand*)item; commandFull=true; return pdTRUE;
}
static int xQueueReceive(int queue, void *item, TickType_t wait) {
    if(queue==appResultQueue) {
        assert(wait==0);
        if(!resultFull) return 0;
        *(SetpointResult*)item=queuedResult; resultFull=false; return pdTRUE;
    }
    assert(queue==appCommandQueue);
    if(commandFull) {
        *(SetpointCommand*)item=queuedCommand; commandFull=false; return pdTRUE;
    }
    if(!simulateLoop) return 0;
    if(mode==1 && wait>0) { /* Continuous command stream. */
        ++ticks; *(SetpointCommand*)item=(SetpointCommand){0,1250}; return pdTRUE;
    }
    if(mode==2 && eventIndex<5 && wait>0) {
        ticks += 1;
        failWrite=(eventIndex==3);
        *(SetpointCommand*)item=events[eventIndex++]; return pdTRUE;
    }
    ticks+=wait; return 0;
}
static int xQueueOverwrite(int queue, const void *item) {
    if(queue==appResultQueue) {
        queuedResult=*(const SetpointResult*)item; ++replies;
        if(mode==2) {
            assert(queuedResult.success == (replies==1));
            if(!queuedResult.success) ++invalidReplies;
        }
        return pdTRUE; /* Slow console never blocks publication. */
    }
    assert(queue==appMeasurementQueue);
    latest=*(const Measurement*)item;
    if(samples==0) assert(!latest.adcValid && latest.dacReady==initOK);
    if(mode==2 && samples==2) assert(latest.adcError && !latest.adcValid);
    if(samples==3) longjmp(done,1);
    return pdTRUE;
}
static bool AD5672R_init(void) { return initOK; }
static bool AD5672R_write(uint8_t channel, uint16_t code) {
    assert(channel==0); ++writes; lastCode=code; ++ticks; return !failWrite;
}
void FeedbackADC_init(void) {}
bool FeedbackADC_read(uint16_t raw[FEEDBACK_ADC_COUNT],uint32_t mv[FEEDBACK_ADC_COUNT]) {
    sampleTimes[samples++]=ticks; ++ticks;
    if(mode==2 && samples==2) return false;
    raw[0]=1000; mv[0]=806; raw[1]=2000; mv[1]=1611; return true;
}
'''
tests = r'''
static void run(unsigned scenario, bool ready, TickType_t start) {
    mode=scenario; initOK=ready; failWrite=false;
    ticks=start; writes=samples=replies=eventIndex=invalidReplies=0;
    commandFull=resultFull=false; simulateLoop=true;
    startTicks=start+(ready ? 1U : 0U); /* zero-write cost */
    if(setjmp(done)==0) mainThread(NULL);
    for(unsigned i=0;i<3;++i) {
        TickType_t elapsed=sampleTimes[i]-startTicks;
        TickType_t deadline=1000U*(i+1);
        /* A sample may wait for the already-running two-tick mock transaction. */
        assert(elapsed>=deadline && elapsed-deadline <= (scenario==1 ? 2U : 0U));
    }
    assert(latest.adcValid && latest.raw[0]==1000 && latest.millivolts[0]==806
        && latest.raw[1]==2000 && latest.millivolts[1]==1611);
}
int main(void) {
    /* Real transport: one outstanding command persists until result consumed. */
    assert(!App_submitSetpoint(1,500));
    assert(!App_submitSetpoint(0,2501));
    commandFull=true;
    assert(!App_submitSetpoint(0,500) && !requestPending);
    commandFull=false;
    assert(App_submitSetpoint(0,1250) && requestPending);
    assert(!App_submitSetpoint(0,500));
    SetpointCommand cmd; assert(xQueueReceive(appCommandQueue,&cmd,0));
    assert(!App_submitSetpoint(0,500)); /* Still pending after dequeue. */
    SetpointResult result;
    assert(!App_takeResult(&result) && requestPending);
    queuedResult=(SetpointResult){cmd,2048,true}; resultFull=true;
    assert(App_takeResult(&result) && !requestPending && result.code==2048);
    assert(App_submitSetpoint(0,500)); requestPending=false; commandFull=false;

    run(0,true,0); assert(writes==1 && lastCode==0); /* Idle holds zero. */
    run(1,true,0); assert(replies>1000); /* Commands cannot starve ADC. */
    run(2,true,0); assert(replies==5 && invalidReplies==4 && writes==3);
    assert(!latest.dacReady); /* SPI failure latches; ADC continues. */
    run(0,false,0); assert(writes==0 && !latest.dacReady);
    run(0,true,UINT32_MAX-1500U); /* Wrap-safe ADC deadline. */
    puts("PASS: queue backpressure, zero startup, ADC cadence, command flood, validation, errors and tick wrap");
}
'''
with tempfile.TemporaryDirectory(prefix='control-task-test-') as tmp:
    test=Path(tmp)/'test.c'; exe=Path(tmp)/'test'
    test.write_text(harness+types+stubs+transport+control+tests)
    subprocess.run(['cc','-std=c11','-Wall','-Wextra','-Werror','-I',str(project),str(test),'-o',str(exe)],check=True)
    subprocess.run([str(exe)],check=True)
