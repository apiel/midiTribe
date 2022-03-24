#ifndef IO_CONTROLLER_AKAI_MPK_MINI_EDIT_SYNTH_H_
#define IO_CONTROLLER_AKAI_MPK_MINI_EDIT_SYNTH_H_

#include <Arduino.h>

#include "./io_controller_akai_mpk_mini_base.h"

class IO_ControllerAkaiMPKminiEditSynth : virtual public IO_ControllerAkaiMPKminiBase
{
public:
    void controlChangeHandlerEditSynth(byte channel, byte control, byte value)
    {
        // bottom row
        if (control == 13)
        {
            synth->wave.setWaveform(value);
            display->displayString("Set wave", synth->wave.getName());
        }
        else if (control == 14)
        {
            synth->wave.edit(value);
            display->displayValue("Wave edit", value);
        }
        else if (control == 15)
        {
        }
        else if (control == 16)
        {
            synth->effect.setEffect((value / 128.0f) * IFX_COUNT);
            display->displayString("Set effect", synth->effect.getName());
        }
        // top row
        else if (control == 17)
        {
            synth->setAttack(value);
            display->displayUnit("Env. Attack", synth->attackMs, "ms");
        }
        else if (control == 18)
        {
            synth->setRelease(value);
            display->displayUnit("Env. Release", synth->releaseMs, "ms");
        }
        else if (control == 19)
        {
            synth->setLevel(value);
            display->displayValue("Level", value);
        }
        else if (control == 20)
        {
        }
    }
};

#endif
