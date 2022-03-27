#ifndef IO_CONTROLLER_AKAI_MPK_MINI_LIVE_LOOP_H_
#define IO_CONTROLLER_AKAI_MPK_MINI_LIVE_LOOP_H_

#include <Arduino.h>

#include "io_loop.h"
#include "io_display.h"

class IO_ControllerAkaiMPKminiLiveLoop
{
protected:
    IO_Loop **loops;
    IO_Display *display;

    byte currentPattern = 0;
    Pattern *pattern;

    byte *currentChannel;

    IO_Loop *getLoop() { return loops[(*currentChannel - 1) % 16]; }

    byte minVelocity = 80;

public:
    IO_ControllerAkaiMPKminiLiveLoop(IO_Display *_display, IO_Loop **_loops, byte *_currentChannel)
    {
        display = _display;
        loops = _loops;
        currentChannel = _currentChannel;
    }

    void noteOnHandler(byte channel, byte note, byte velocity)
    {
        getLoop()->noteOn(*currentChannel, note, max(velocity, minVelocity));
    }

    void noteOffHandler(byte channel, byte note, byte velocity)
    {
        getLoop()->noteOff(note);
    }

    void controlChangeHandler(byte channel, byte control, byte value)
    {
        // set min max velocity
        if (control == 20) {
            minVelocity = max(value, 10);
            display->displayValue("Min velocity", minVelocity);
        }
    }
};

#endif
