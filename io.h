#ifndef IO_H_
#define IO_H_

#include <Arduino.h>

#include "io_display.h"
#include "io_midi.h"
#include "io_core.h"

void ioInit()
{
    Serial.println("Mini synth init");
    display.init();

    midiInit();
    editor.load();

    display.update();
}

void ioLoop()
{
    midiLoop();
    display.loop();
}

#endif
