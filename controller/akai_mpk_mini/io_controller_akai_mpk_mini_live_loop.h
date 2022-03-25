#ifndef IO_CONTROLLER_AKAI_MPK_MINI_LIVE_LOOP_H_
#define IO_CONTROLLER_AKAI_MPK_MINI_LIVE_LOOP_H_

#include <Arduino.h>

#include "io_loop.h"
#include "io_display.h"

class IO_ControllerAkaiMPKminiLiveLoop
{
protected:
    IO_Loop **loops;
    IO_Loop *loop;
    IO_Display *display;

    byte currentPattern = 0;
    Pattern *pattern;

    IO_Loop *getLoop(byte pos) { return loops[0]; } // return loops[pos % SYNTH_COUNT]; }

public:
    IO_ControllerAkaiMPKminiLiveLoop(IO_Display *_display, IO_Loop **_loops)
    {
        display = _display;
        loops = _loops;
        loop = getLoop(0);
    }

    void noteOnHandler(byte channel, byte note, byte velocity)
    {
        loop->noteOn(note);
    }

    void noteOffHandler(byte channel, byte note, byte velocity)
    {
        loop->noteOff(note);
    }

    void controlChangeHandler(byte channel, byte control, byte value)
    {
    }
};

#endif
