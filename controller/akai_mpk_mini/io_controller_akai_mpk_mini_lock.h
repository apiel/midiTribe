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

    void render()
    {
        display->setDefaultName("Keyboard locked", 0);
    }

    void noteOnHandler(byte channel, byte note, byte velocity)
    {
        display->displayValue("Keyboard", "Locked");
    }

    void noteOffHandler(byte channel, byte note, byte velocity)
    {
        display->displayValue("Keyboard", "Locked");
    }

    void controlChangeHandler(byte channel, byte control, byte value)
    {
        display->displayValue("Keyboard", "Locked");
    }
};

#endif
