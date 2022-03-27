#ifndef IO_CONTROLLER_AKAI_MPK_MINI_H_
#define IO_CONTROLLER_AKAI_MPK_MINI_H_

#include <Arduino.h>
#include <USBHost_t36.h>

#include "io_loop.h"
#include "io_display.h"
#include "io_controller_akai_mpk_mini_lock.h"
#include "io_controller_akai_mpk_mini_live_loop.h"
#include "io_controller_akai_mpk_mini_live_synth.h"
#include "io_controller_akai_mpk_mini_def.h"

enum
{
    MODE_LOCK,
    MODE_LIVE_LOOP,
    MODE_LIVE_SYNTH,
    MODE_COUNT
};

class IO_ControllerAkaiMPKmini
{
protected:
    // IO_Loop *loopPadPressed = NULL;
    // bool loopPadPressedDidAction = false;
    byte mode = MODE_LIVE_LOOP;

    IO_Loop **loops;
    IO_Display *display;

    bool modeSustainPressed = false;
    byte currentChannel = 1;

    IO_ControllerAkaiMPKminiLock modeLock;
    IO_ControllerAkaiMPKminiLiveLoop modeLiveLoop;
    IO_ControllerAkaiMPKminiLiveSynth modeLiveSynth;

    // byte padPressed = 0;

    const char *getModeName()
    {
        switch (mode)
        {
        case MODE_LIVE_LOOP:
            return "Live Loop";
        case MODE_LIVE_SYNTH:
            return "Live Synth";
        case MODE_LOCK:
            return "Locked";
        }
        return "Unknown";
    }

public:
    IO_ControllerAkaiMPKmini(IO_Display *_display, IO_Loop **_loops) : modeLock(_display),
                                                                       modeLiveLoop(_display, _loops, &currentChannel),
                                                                       modeLiveSynth(_display, &currentChannel)
    {
        display = _display;
        loops = _loops;
    }

    void setMidiGroovebox(MIDIDevice_BigBuffer *_midi)
    {
        modeLiveSynth.setMidiGroovebox(_midi);
    }

    void noteOnHandler(byte channel, byte note, byte velocity)
    {
        if (modeSustainPressed)
        {
            if (channel == PAD_CHANNEL)
            {
                mode = (note - PAD_1) % MODE_COUNT;
                display->displayString("Mode", getModeName());
            }
            else if (setChannelFromNote(note))
            {
                display->displayValue("Channel", currentChannel);
            }
            else
            {
                display->displayString("Channel", "Out of range");
            }
            return;
        }

        switch (mode)
        {
        case MODE_LIVE_LOOP:
            modeLiveLoop.noteOnHandler(channel, note, velocity);
            break;
        case MODE_LIVE_SYNTH:
            modeLiveSynth.noteOnHandler(channel, note, velocity);
            break;
        case MODE_LOCK:
            modeLock.noteOnHandler(channel, note, velocity);
            break;
        }
    }

    void noteOffHandler(byte channel, byte note, byte velocity)
    {
        if (modeSustainPressed)
        {
            display->displayString("Mode", getModeName());
            return;
        }

        switch (mode)
        {
        case MODE_LIVE_LOOP:
            modeLiveLoop.noteOffHandler(channel, note, velocity);
            break;
        case MODE_LIVE_SYNTH:
            modeLiveSynth.noteOffHandler(channel, note, velocity);
            break;
        case MODE_LOCK:
            modeLock.noteOffHandler(channel, note, velocity);
            break;
        }
    }

    void controlChangeHandler(byte channel, byte control, byte value)
    {
        if (control == 64) // when pressin sustain button
        {
            modeSustainPressed = value == 127;
            display->displayString("Mode", getModeName());
            return;
        }

        if (modeSustainPressed)
        {
            display->displayString("Mode", getModeName());
            return;
        }

        switch (mode)
        {
        case MODE_LIVE_LOOP:
            modeLiveLoop.controlChangeHandler(channel, control, value);
            break;
        case MODE_LIVE_SYNTH:
            modeLiveSynth.controlChangeHandler(channel, control, value);
            break;
        case MODE_LOCK:
            modeLock.controlChangeHandler(channel, control, value);
            break;
        }
    }

    byte setChannelFromNote(byte note)
    {
        switch (note)
        {
        case _C3:
            currentChannel = 1;
            return currentChannel;
        case _D3:
            currentChannel = 2;
            return currentChannel;
        case _E3:
            currentChannel = 3;
            return currentChannel;
        case _F3:
            currentChannel = 4;
            return currentChannel;
        case _G3:
            currentChannel = 5;
            return currentChannel;
        case _A3:
            currentChannel = 6;
            return currentChannel;
        case _B3:
            currentChannel = 7;
            return currentChannel;
        case _C4:
            currentChannel = 8;
            return currentChannel;
        case _D4:
            currentChannel = 9;
            return currentChannel;
        case _E4:
            currentChannel = 10;
            return currentChannel;
        case _F4:
            currentChannel = 11;
            return currentChannel;
        case _G4:
            currentChannel = 12;
            return currentChannel;
        case _A4:
            currentChannel = 13;
            return currentChannel;
        case _B4:
            currentChannel = 14;
            return currentChannel;
        case _C5:
            currentChannel = 15;
            return currentChannel;
        case _D5:
            currentChannel = 16;
            return currentChannel;
        }
        return 0;
    }

    // void noteOnHandler(byte channel, byte note, byte velocity)
    // {
    //     if (modeSustainPressed)
    //     {
    //         mode = note - 48;
    //         if (note == 72)
    //         {
    //             mode = MODE_LOCK;
    //         }
    //         display->displayString("Mode", getModeName());
    //         return;
    //     }

    //     // if (mode == MODE_LOCK)
    //     // {
    //     //     display->displayString("Mode", getModeName());
    //     //     return;
    //     // }

    //     // if (channel == PAD_CHANNEL)
    //     // {
    //     //     // if (note == PAD_1)
    //     //     // {
    //     //     //     loopPadPressed = getLoop(SYNTH_0);
    //     //     // }
    //     //     // else if (note == PAD_2)
    //     //     // {
    //     //     //     loopPadPressed = getLoop(SYNTH_1);
    //     //     // }
    //     //     // else if (note == PAD_3)
    //     //     // {
    //     //     //     loopPadPressed = getLoop(SYNTH_2);
    //     //     // }
    //     //     // else if (note == PAD_4)
    //     //     // {
    //     //     //     loopPadPressed = getLoop(SYNTH_3);
    //     //     // }
    //     //     // return;
    //     // }

    //     // if (mode == MODE_LIVE_LOOP)
    //     // {
    //     //     loop->noteOn(note);
    //     // }
    // }

    // void loopPadPressedAction(byte id)
    // {
    //     if (loop->id != id)
    //     {
    //         // setCurrentSynth(id);
    //         display->displayValue("Current synth", id);
    //     }
    //     else if (!loopPadPressedDidAction)
    //     {
    //         loop->toggleMode();
    //         if (loop->modeSingleLoop)
    //         {
    //             display->displayString("Pattern mode", "loop");
    //         }
    //         else
    //         {
    //             display->displayString("Pattern mode", "one");
    //         }
    //     }
    //     loopPadPressed = NULL;
    //     loopPadPressedDidAction = false;
    // }

    // void noteOffHandler(byte channel, byte note, byte velocity)
    // {
    //     if (modeSustainPressed)
    //     {
    //         display->displayString("Mode", getModeName());
    //         return;
    //     }

    //     if (mode == MODE_LOCK)
    //     {
    //         display->displayString("Mode", getModeName());
    //         return;
    //     }

    //     if (channel == PAD_CHANNEL)
    //     {
    //         if (note == PAD_1)
    //         {
    //             loopPadPressedAction(0);
    //         }
    //         else if (note == PAD_2)
    //         {
    //             loopPadPressedAction(1);
    //         }
    //         else if (note == PAD_3)
    //         {
    //             loopPadPressedAction(2);
    //         }
    //         else if (note == PAD_4)
    //         {
    //             loopPadPressedAction(3);
    //         }
    //         else if (note == PAD_5)
    //         {
    //             loop->setCurrentPatternSelector(0);
    //             display->displayValue("Pattern selector 0", loop->patternSelector[0]);
    //         }
    //         else if (note == PAD_6)
    //         {
    //             loop->setCurrentPatternSelector(1);
    //             display->displayValue("Pattern selector 1", loop->patternSelector[1]);
    //         }
    //         else if (note == PAD_7)
    //         {
    //             loop->setCurrentPatternSelector(2);
    //             display->displayValue("Pattern selector 2", loop->patternSelector[2]);
    //         }
    //         else if (note == PAD_8)
    //         {
    //             loop->setCurrentPatternSelector(3);
    //             display->displayValue("Pattern selector 3", loop->patternSelector[3]);
    //         }
    //         return;
    //     }

    //     // // should just off all ??
    //     // if (mode == MODE_LIVE_LOOP)
    //     // {
    //     //     loop->noteOff(note);
    //     // }
    // }

    // void controlChangeHandler(byte channel, byte control, byte value)
    // {
    //     if (control == 64) // when pressin sustain button
    //     {
    //         modeSustainPressed = value == 127;
    //         display->displayString("Mode", getModeName());
    //         return;
    //     }
    //     if (mode == MODE_LOCK)
    //     {
    //         display->displayString("Mode", getModeName());
    //         return;
    //     }

    //     if (loopPadPressed)
    //     {
    //         if (control == 17)
    //         {
    //             loopPadPressed->setPatternSelector(0, value);
    //             display->displayValue("Pattern selector 0", loopPadPressed->patternSelector[0]);
    //             loopPadPressedDidAction = true;
    //         }
    //         else if (control == 18)
    //         {
    //             loopPadPressed->setPatternSelector(1, value);
    //             display->displayValue("Pattern selector 1", loopPadPressed->patternSelector[1]);
    //             loopPadPressedDidAction = true;
    //         }
    //         else if (control == 19)
    //         {
    //             loopPadPressed->setPatternSelector(2, value);
    //             display->displayValue("Pattern selector 2", loopPadPressed->patternSelector[2]);
    //             loopPadPressedDidAction = true;
    //         }
    //         else if (control == 20)
    //         {
    //             loopPadPressed->setPatternSelector(3, value);
    //             display->displayValue("Pattern selector 3", loopPadPressed->patternSelector[3]);
    //             loopPadPressedDidAction = true;
    //         }
    //         return;
    //     }
    // }
};

#endif
