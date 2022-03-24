#ifndef IO_CONTROLLER_AKAI_MPK_MAIN_H_
#define IO_CONTROLLER_AKAI_MPK_MAIN_H_

#include <Arduino.h>

#include "./io_controller_akai_mpk_mini_base.h"

class IO_ControllerAkaiMPKminiMain : virtual public IO_ControllerAkaiMPKminiBase
{
public:
    void controlChangeHandlerMain(byte channel, byte control, byte value)
    {
        // bottom row
        if (control == 13)
        {
            synth->filter.setResonance(value);
            display->displayValue("Resonnance", value);
        }
        else if (control == 14)
        {
            synth->filterLadder.setResonance(value);
            display->displayValue("Ladder Resonnance", value);
        }
        else if (control == 15)
        {
        }
        else if (control == 16)
        {
            synth->effect.edit2(value);
            display->displayValue("Edit 2 effect", value);
        }
        // top row
        else if (control == 17)
        {
            synth->filter.setCutoff(value);
            display->displayValue("Cutoff", value);
        }
        else if (control == 18)
        {
            synth->filterLadder.setCutoff(value);
            display->displayValue("Ladder Cutoff", value);
        }
        else if (control == 19)
        {
        }
        else if (control == 20)
        {
            synth->effect.edit1(value);
            display->displayValue("Edit 1 effect", value);
        }
    }
};

#endif
