#ifndef IO_CONTROLLER_AKAI_MPK_MINI_LOCK_H_
#define IO_CONTROLLER_AKAI_MPK_MINI_LOCK_H_

#include <Arduino.h>
#include <USBHost_t36.h>

#include "io_display.h"

class IO_ControllerAkaiMPKminiLock
{
protected:
    IO_Display *display;
    MIDIDevice_BigBuffer *midiGroovebox = NULL;
    MIDIDevice_BigBuffer *midiController = NULL;

public:
    IO_ControllerAkaiMPKminiLock(IO_Display *_display)
    {
        display = _display;
    }

    void setMidiGroovebox(MIDIDevice_BigBuffer *_midi)
    {
        midiGroovebox = _midi;
        render();
    }

    void setMidiController(MIDIDevice_BigBuffer *_midi)
    {
        midiController = _midi;
        render();
    }

    void render()
    {
        display->setDefaultName("Keyboard locked\n\n", 0);

        // char val[100];
        // snprintf(val, 100, "Groove", getNoteStr(note), getNoteOctave(note));
        // display->displayValue("Set note", val2);
        if (midiGroovebox && midiController)
        {
            display->setDefaultValue("Groovebox  OK\nController OK");
        }
        else if (midiController)
        {
            display->setDefaultValue("Groovebox  NONE\nController OK");
        }
        else if (midiGroovebox)
        {
            display->setDefaultValue("Groovebox  OK\nController NONE");
        }
        else
        {
            display->setDefaultValue("Groovebox  NONE\nController NONE");
        }
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
