#ifndef IO_CONTROLLER_AKAI_MPK_MINI_LOCK_H_
#define IO_CONTROLLER_AKAI_MPK_MINI_LOCK_H_

#include <Arduino.h>

#include "io_display.h"

class IO_ControllerAkaiMPKminiLock
{
protected:
    IO_Display *display;

public:
    IO_ControllerAkaiMPKminiLock(IO_Display *_display)
    {
        display = _display;
    }

    void noteOnHandler(byte channel, byte note, byte velocity)
    {
        display->displayString("Keyboard", "Locked");
    }

    void noteOffHandler(byte channel, byte note, byte velocity)
    {
        display->displayString("Keyboard", "Locked");
    }

    void controlChangeHandler(byte channel, byte control, byte value)
    {
        display->displayString("Keyboard", "Locked");
    }
};

#endif
