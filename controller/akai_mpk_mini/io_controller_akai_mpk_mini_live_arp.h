#ifndef IO_CONTROLLER_AKAI_MPK_MINI_LIVE_ARP_H_
#define IO_CONTROLLER_AKAI_MPK_MINI_LIVE_ARP_H_

#include <Arduino.h>
#include <USBHost_t36.h>

#include "io_display.h"
#include "io_arp.h"
#include "io_controller_akai_mpk_mini_def.h"

class IO_ControllerAkaiMPKminiLiveArp
{
protected:
    IO_Arp *arp;
    IO_Display *display;
    MIDIDevice_BigBuffer *midiGroovebox = NULL;
    byte *currentChannel;
    char var[12];

public:
    IO_ControllerAkaiMPKminiLiveArp(IO_Display *_display, byte *_currentChannel, IO_Arp *_arp)
    {
        display = _display;
        currentChannel = _currentChannel;
        arp = _arp;
    }

    void render()
    {
        display->setDefaultName("Live Arp\n\n", 0);

        snprintf(var, 12, "Channel %02d", *currentChannel);
        // Serial.println(var);
        display->setDefaultValue(var);
    }

    void setMidiGroovebox(MIDIDevice_BigBuffer *_midi)
    {
        midiGroovebox = _midi;
        arp->setMidi(_midi);
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
            arp->noteOn(*currentChannel, note, velocity);
        }
        else
        {
            display->displayValue("Please connect", "Groovebox");
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
            arp->noteOff(note);
        }
        else
        {
            display->displayValue("Please connect", "Groovebox");
        }
    }

    void controlChangeHandler(byte channel, byte control, byte value)
    {
        if (control == 13)
        {
            arp->setArpMode(value / 128.0f * ARP_COUNT);
            display->displayValue("ARP mode", arp->getModeName());
        }
    }
};

#endif
