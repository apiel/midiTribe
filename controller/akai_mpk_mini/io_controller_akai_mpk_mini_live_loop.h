#ifndef IO_CONTROLLER_AKAI_MPK_MINI_LIVE_LOOP_H_
#define IO_CONTROLLER_AKAI_MPK_MINI_LIVE_LOOP_H_

#include <Arduino.h>

#include "io_loop.h"
#include "io_display.h"
#include "io_controller_akai_mpk_mini_def.h"

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
        if (channel == PAD_CHANNEL)
        {
            if (note == PAD_1)
            {
                onBottomPad(1);
            }
            else if (note == PAD_2)
            {
                onBottomPad(2);
            }
            else if (note == PAD_3)
            {
                onBottomPad(3);
            }
            else if (note == PAD_4)
            {
                onBottomPad(4);
            }
            return;
        }

        getLoop()->noteOn(*currentChannel, note, max(velocity, minVelocity));
    }

    // bottom row pad1, pad2, pad3, pad4
    void onBottomPad(byte padValue)
    {
        byte channelStart = floor((*currentChannel - 1) / 4) * 4;
        if (*currentChannel == channelStart + padValue)
        {
            getLoop()->toggleMode();
            if (getLoop()->modeSingleLoop)
            {
                display->displayString("Pattern mode", "Pressed");
            }
            else
            {
                display->displayString("Pattern mode", "Loop");
            }
        }
        else
        {
            *currentChannel = channelStart + padValue;
            display->displayValue("Channel", *currentChannel);
        }
    }

    void noteOffHandler(byte channel, byte note, byte velocity)
    {
        getLoop()->noteOff(note);
    }

    void controlChangeHandler(byte channel, byte control, byte value)
    {
        // set min max velocity
        if (control == 20)
        {
            minVelocity = max(value, 10);
            display->displayValue("Min velocity", minVelocity);
        }
    }
};

#endif
