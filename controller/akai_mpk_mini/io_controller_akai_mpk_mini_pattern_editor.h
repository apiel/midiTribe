#ifndef IO_CONTROLLER_AKAI_MPK_MINI_PATTERN_EDITOR_H_
#define IO_CONTROLLER_AKAI_MPK_MINI_PATTERN_EDITOR_H_

#include <Arduino.h>
#include <USBHost_t36.h>

#include "io_display.h"

class IO_ControllerAkaiMPKminiPatternEditor
{
protected:
    IO_Display *display;

public:
    IO_ControllerAkaiMPKminiPatternEditor(IO_Display *_display)
    {
        display = _display;
    }

    void noteOnHandler(byte channel, byte note, byte velocity)
    {
    }

    void noteOffHandler(byte channel, byte note, byte velocity)
    {
    }

    void controlChangeHandler(byte channel, byte control, byte value)
    {
    }
};

#endif
