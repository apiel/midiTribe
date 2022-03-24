#ifndef IO_CONTROLLER_AKAI_MPK_MINI_H_
#define IO_CONTROLLER_AKAI_MPK_MINI_H_

#include <Arduino.h>

#include "./io_controller_akai_mpk_mini_main.h"
#include "./io_controller_akai_mpk_mini_edit_synth.h"
#include "./io_controller_akai_mpk_mini_drums.h"

class IO_ControllerAkaiMPKmini : IO_ControllerAkaiMPKminiMain,
                                 IO_ControllerAkaiMPKminiEditSynth,
                                 IO_ControllerAkaiMPKminiDrums
{
public:
    IO_ControllerAkaiMPKmini(IO_Display *_display, IO_AudioLoop **_loops, IO_AudioSynth **_synths, IO_DrumMachine *_drums)
    {
        display = _display;
        loops = _loops;
        synths = _synths;
        drums = _drums;
        loop = getLoop(0);
        synth = getSynth(0);
    }

    void noteOnHandler(byte channel, byte note, byte velocity)
    {
        if (modeSustainPressed)
        {
            mode = note - 48;
            if (note == 72)
            {
                mode = MODE_LOCK;
            }
            display->displayString("Mode", getModeName());
            return;
        }

        if (mode == MODE_LOCK)
        {
            display->displayString("Mode", getModeName());
            return;
        }

        if (mode == MODE_DRUM)
        {
            noteOnDrum(channel, note, velocity);
            return;
        }

        if (channel == PAD_CHANNEL)
        {
            if (note == PAD_1)
            {
                loopPadPressed = getLoop(SYNTH_0);
            }
            else if (note == PAD_2)
            {
                loopPadPressed = getLoop(SYNTH_1);
            }
            else if (note == PAD_3)
            {
                loopPadPressed = getLoop(SYNTH_2);
            }
            else if (note == PAD_4)
            {
                loopPadPressed = getLoop(SYNTH_3);
            }
            return;
        }

        if (mode == MODE_MAIN_ARP) // should synth_edit also allow arp mode
        {
            loop->noteOn(note);
        }
        else
        {
            synth->noteOn(note, velocity);
        }
    }

    void loopPadPressedAction(byte id)
    {
        if (loop->id != id)
        {
            setCurrentSynth(id);
            display->displayValue("Current synth", id);
        }
        else if (!loopPadPressedDidAction)
        {
            loop->toggleMode();
            if (loop->modeSingleLoop)
            {
                display->displayString("ARP mode", "loop");
            }
            else
            {
                display->displayString("ARP mode", "one");
            }
        }
        loopPadPressed = NULL;
        loopPadPressedDidAction = false;
    }

    void noteOffHandler(byte channel, byte note, byte velocity)
    {
        if (modeSustainPressed)
        {
            display->displayString("Mode", getModeName());
            return;
        }

        if (mode == MODE_LOCK)
        {
            display->displayString("Mode", getModeName());
            return;
        }

        if (mode == MODE_DRUM)
        {
            noteOffDrum(channel, note, velocity);
            return;
        }

        if (channel == PAD_CHANNEL)
        {
            if (note == PAD_1)
            {
                loopPadPressedAction(0);
            }
            else if (note == PAD_2)
            {
                loopPadPressedAction(1);
            }
            else if (note == PAD_3)
            {
                loopPadPressedAction(2);
            }
            else if (note == PAD_4)
            {
                loopPadPressedAction(3);
            }
            else if (note == PAD_5)
            {
                loop->setCurrentPatternSelector(0);
                display->displayValue("Pattern selector 0", loop->patternSelector[0]);
            }
            else if (note == PAD_6)
            {
                loop->setCurrentPatternSelector(1);
                display->displayValue("Pattern selector 1", loop->patternSelector[1]);
            }
            else if (note == PAD_7)
            {
                loop->setCurrentPatternSelector(2);
                display->displayValue("Pattern selector 2", loop->patternSelector[2]);
            }
            else if (note == PAD_8)
            {
                loop->setCurrentPatternSelector(3);
                display->displayValue("Pattern selector 3", loop->patternSelector[3]);
            }
            return;
        }

        // should just off all ??
        if (mode == MODE_MAIN_ARP)
        {
            loop->noteOff(note);
        }
        else
        {
            synth->noteOff(note);
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
        if (mode == MODE_LOCK)
        {
            display->displayString("Mode", getModeName());
            return;
        }

        if (loopPadPressed)
        {
            if (control == 17)
            {
                loopPadPressed->setPatternSelector(0, value);
                display->displayValue("Pattern selector 0", loopPadPressed->patternSelector[0]);
                loopPadPressedDidAction = true;
            }
            else if (control == 18)
            {
                loopPadPressed->setPatternSelector(1, value);
                display->displayValue("Pattern selector 1", loopPadPressed->patternSelector[1]);
                loopPadPressedDidAction = true;
            }
            else if (control == 19)
            {
                loopPadPressed->setPatternSelector(2, value);
                display->displayValue("Pattern selector 2", loopPadPressed->patternSelector[2]);
                loopPadPressedDidAction = true;
            }
            else if (control == 20)
            {
                loopPadPressed->setPatternSelector(3, value);
                display->displayValue("Pattern selector 3", loopPadPressed->patternSelector[3]);
                loopPadPressedDidAction = true;
            }
            return;
        }

        if (mode == MODE_EDIT_SYNTH || modeSustainPressed)
        {
            controlChangeHandlerEditSynth(channel, control, value);
        }
        else // MODE_MAIN_ARP || MODE_MAIN_SYNTH
        {
            controlChangeHandlerMain(channel, control, value);
        }
    }
};

#endif
