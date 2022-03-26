#ifndef IO_CONTROLLER_AKAI_MPK_MINI_H_
#define IO_CONTROLLER_AKAI_MPK_MINI_H_

#include <Arduino.h>
#include <USBHost_t36.h>

#include "io_loop.h"
#include "io_display.h"
#include "io_controller_akai_mpk_mini_lock.h"
#include "io_controller_akai_mpk_mini_live_synth.h"

#define PAD_CHANNEL 10
#define PAD_1 36
#define PAD_2 37
#define PAD_3 38
#define PAD_4 39
#define PAD_5 40
#define PAD_6 41
#define PAD_7 42
#define PAD_8 43

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
    // IO_Loop *loop;
    IO_Display *display;

    bool modeSustainPressed = false;

    // MIDIDevice_BigBuffer *midiGrouvebox = NULL;

    IO_ControllerAkaiMPKminiLock modeLock;
    IO_ControllerAkaiMPKminiLiveSynth modeLiveSynth;
    // byte padPressed = 0;

    // IO_Loop *getLoop(byte pos) { return loops[0]; } // return loops[pos % SYNTH_COUNT]; }

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
    IO_ControllerAkaiMPKmini(IO_Display *_display, IO_Loop **_loops) : modeLock(_display), modeLiveSynth(_display) // modeLiveSynth(_display, midiGroovebox)
    {
        display = _display;
        loops = _loops;
        // loop = getLoop(0);
    }

    void setMidiGroovebox(MIDIDevice_BigBuffer *_midi)
    {
        // midiGroovebox = _midi;
        modeLiveSynth.setMidiGroovebox(_midi);
    }

    void noteOnHandler(byte channel, byte note, byte velocity)
    {
        if (modeSustainPressed)
        {
            if (channel == PAD_CHANNEL)
            {
                mode = (note - PAD_1) % MODE_COUNT;
            }
            display->displayString("Mode", getModeName());
            return;
        }

        switch (mode)
        {
        case MODE_LIVE_LOOP:
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
            break;
        case MODE_LIVE_SYNTH:
            modeLiveSynth.controlChangeHandler(channel, control, value);
            break;
        case MODE_LOCK:
            modeLock.controlChangeHandler(channel, control, value);
            break;
        }
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
