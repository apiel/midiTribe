#ifndef IO_CONTROLLER_AKAI_MPK_MINI_LIVE_LOOP_H_
#define IO_CONTROLLER_AKAI_MPK_MINI_LIVE_LOOP_H_

#include <Arduino.h>

#include "io_utils.h"
#include "io_patterns.h"
#include "io_loop_poly.h"
#include "io_display.h"
#include "io_controller_akai_mpk_mini_def.h"

class IO_ControllerAkaiMPKminiLiveLoop
{
protected:
    IO_Poly_Loop **loops;
    IO_Display *display;

    byte topPadPressed = 255; // 255 if no
    bool topPadPressedDidAction = false;

    byte currentPattern = 0;
    Pattern *pattern;

    byte *currentChannel;

    IO_Poly_Loop *getLoop() { return getLoop(*currentChannel); }
    IO_Poly_Loop *getLoop(byte pos) { return loops[(pos - 1) % 16]; }

    byte minVelocity = 100;

public:
    IO_ControllerAkaiMPKminiLiveLoop(IO_Display *_display, IO_Poly_Loop **_loops, byte *_currentChannel)
    {
        display = _display;
        loops = _loops;
        currentChannel = _currentChannel;
    }

    void render()
    {
        display->setDefaultName("Live Loop", 0);
    }

// NOTE
// Maybe to complicated?
// Pad might still do like synth, the 8 pads are for channel selection
// if press again change mode from loop to single
// if press and in the same time press note then select pattern

    void noteOnHandler(byte channel, byte note, byte velocity)
    {
        if (channel == PAD_CHANNEL)
        {

            if (between(note, PAD_1, PAD_4))
            {
                onBottomPad(note - PAD_1 + 1);
            }
            else if (between(note, PAD_5, PAD_8))
            {
                topPadPressed = note - PAD_5;
            }
            return;
        }

        if (isTopPadPressed())
        {
            setPatternSelector(topPadPressed, note);
            topPadPressedDidAction = true;
        }
        else
        {
            getLoop()->noteOn(*currentChannel, note, max(velocity, minVelocity));
        }
    }

    void setPatternSelector(byte bankPos, byte patternPos)
    {
        getLoop()->setPatternSelector(bankPos, patternPos);
        display->displayValue("Pattern selector", getLoop()->getPatternAtBank(bankPos));
    }

    bool isTopPadPressed()
    {
        return topPadPressed != 255;
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
                display->displayValue("Pattern mode", "Pressed");
            }
            else
            {
                display->displayValue("Pattern mode", "Loop");
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
        if (channel == PAD_CHANNEL)
        {
            if (between(note, PAD_5, PAD_8))
            {
                if (!topPadPressedDidAction)
                {
                    getLoop()->setCurrentPatternSelector(note - PAD_5);
                    display->displayValue("Select pattern", getLoop()->getCurrentPattern());
                }
            }
            topPadPressed = 255;
            topPadPressedDidAction = false;
            return;
        }
        if (!isTopPadPressed())
        {
            getLoop()->noteOff(note);
        }
    }

    void controlChangeHandler(byte channel, byte control, byte value)
    {
        // set min max velocity
        if (control == 16)
        {
            minVelocity = max(value, 10);
            display->displayValue("Min velocity", minVelocity);
        }
        else if (between(control,17, 20))
        {
            setPatternSelector(control - 17, (value / 128.0f) * PATTERN_COUNT);
        }
    }
};

#endif
