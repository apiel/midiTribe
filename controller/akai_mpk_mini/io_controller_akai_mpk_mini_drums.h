#ifndef IO_CONTROLLER_AKAI_MPK_DRUMS_H_
#define IO_CONTROLLER_AKAI_MPK_DRUMS_H_

#include <Arduino.h>

#include "./io_controller_akai_mpk_mini_base.h"

class IO_ControllerAkaiMPKminiDrums : virtual public IO_ControllerAkaiMPKminiBase
{
public:
    void noteOnDrum(byte channel, byte note, byte velocity)
    {
        if (channel == PAD_CHANNEL)
        {
            if (note == PAD_5)
            {
                padPressed = PAD_5;
            }
            else if (note == PAD_6)
            {
                padPressed = PAD_6;
            }
            else if (note == PAD_7)
            {
                padPressed = PAD_7;
            }
            else if (note == PAD_8)
            {
                padPressed = PAD_8;
            }
            return;
        }
    }

    void noteOffDrum(byte channel, byte note, byte velocity)
    {
        if (channel == PAD_CHANNEL)
        {
            if (note == PAD_1 || note == PAD_2 || note == PAD_3 || note == PAD_4)
            {
                byte drumType = DRUM_KICK + (note - PAD_1);
                drums->toggleMute(drumType);
                display->displayString(drums->getDrumName(drumType), drums->mute[drumType] ? "Mute" : "ON");
            }
            padPressed = 0;
            return;
        }
        else if (padPressed)
        {
            byte drumType = DRUM_KICK + (padPressed - PAD_5);
            char *file = drums->setDrum(drumType, note - 48);
            if (file)
            {
                display->displayString(drums->getDrumName(drumType), file);
            }
            else
            {
                // Maybe not even display something
                // display->displayString(drums->getDrumName(drumType), "None");
            }
        }
        else
        {
            drums->setNextPattern(note - 48);
            display->displayValue("Drum pattern", note - 48);
        }
    }
};

#endif
