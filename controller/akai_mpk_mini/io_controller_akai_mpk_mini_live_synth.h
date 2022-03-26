#ifndef IO_CONTROLLER_AKAI_MPK_MINI_LIVE_SYNTH_H_
#define IO_CONTROLLER_AKAI_MPK_MINI_LIVE_SYNTH_H_

#include <Arduino.h>
#include <USBHost_t36.h>

#include "io_display.h"

class IO_ControllerAkaiMPKminiLiveSynth
{
protected:
    IO_Display *display;
    MIDIDevice_BigBuffer *midiGroovebox = NULL;

public:
    IO_ControllerAkaiMPKminiLiveSynth(IO_Display *_display)
    {
        display = _display;
    }

    // IO_ControllerAkaiMPKminiLiveSynth(IO_Display *_display, MIDIDevice_BigBuffer *_midi)
    // {
    //     midiGroovebox = _midi;
    //     display = _display;
    // }

    void setMidiGroovebox(MIDIDevice_BigBuffer *_midi)
    {
        midiGroovebox = _midi;
    }

    void noteOnHandler(byte channel, byte note, byte velocity)
    {
        if (midiGroovebox)
        {
            midiGroovebox->sendNoteOn(note, velocity, 1);
        }
    }

    void noteOffHandler(byte channel, byte note, byte velocity)
    {
        if (midiGroovebox)
        {
            midiGroovebox->sendNoteOff(note, velocity, 1);
        }
    }

    void controlChangeHandler(byte channel, byte control, byte value)
    {
    }
};

#endif
