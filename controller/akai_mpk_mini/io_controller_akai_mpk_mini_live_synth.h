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
    byte *currentChannel;

public:
    IO_ControllerAkaiMPKminiLiveSynth(IO_Display *_display, byte *_currentChannel)
    {
        display = _display;
        currentChannel = _currentChannel;
    }

    void setMidiGroovebox(MIDIDevice_BigBuffer *_midi)
    {
        midiGroovebox = _midi;
    }

    void noteOnHandler(byte channel, byte note, byte velocity)
    {
        if (channel == PAD_CHANNEL)
        {
            onPad(note - PAD_1 + 1);
            return;
        }

        if (midiGroovebox)
        {
            midiGroovebox->sendNoteOn(note, velocity, *currentChannel);
        }
    }

    void onPad(byte padValue)
    {
        byte channelStart = floor((*currentChannel - 1) / 8) * 8;
        *currentChannel = channelStart + padValue;
        display->displayValue("Channel", *currentChannel);
    }

    void noteOffHandler(byte channel, byte note, byte velocity)
    {
        if (midiGroovebox)
        {
            midiGroovebox->sendNoteOff(note, velocity, *currentChannel);
        }
    }

    void controlChangeHandler(byte channel, byte control, byte value)
    {
    }
};

#endif
