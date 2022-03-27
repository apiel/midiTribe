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
            byte channelStart = floor(*currentChannel / 4) * 4;
            if (note == PAD_1)
            {
                *currentChannel = channelStart + 1;
            }
            else if (note == PAD_2)
            {
                *currentChannel = channelStart + 2;
            }
            else if (note == PAD_3)
            {
                *currentChannel = channelStart + 3;
            }
            else if (note == PAD_4)
            {
                *currentChannel = channelStart + 4;
            }
            display->displayValue("Channel", *currentChannel);
            return;
        }

        if (midiGroovebox)
        {
            midiGroovebox->sendNoteOn(note, velocity, *currentChannel);
        }
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
