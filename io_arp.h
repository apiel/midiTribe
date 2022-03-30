#ifndef IO_ARP_H_
#define IO_ARP_H_

#include <Arduino.h>
#include <USBHost_t36.h>

#define ARP_NOTES_COUNT 6
#define ARP_PATTERN_LEN ARP_NOTES_COUNT * 2
#define NOTE_END 255

enum
{
    ARP_IN_ORDER,
    ARP_UP,
    ARP_DOWN,
    ARP_UP_DOWN,
    ARP_COUNT
};

class IO_Arp
{
protected:
    MIDIDevice_BigBuffer *midiGroovebox = NULL;
    byte notes[ARP_NOTES_COUNT];
    byte pattern[ARP_PATTERN_LEN];

    byte arpMode = 0;
    byte currentStep = 0;
    byte lastStep = 0;

    byte channel = 1;
    byte velocity = 100;

    void setPattern()
    {
        if (arpMode == ARP_IN_ORDER)
        {
            byte p = 0;
            for (byte i = 0; i < ARP_NOTES_COUNT; i++)
            {
                if (notes[i] != NOTE_END)
                {
                    pattern[p] = notes[i];
                    p++;
                }
            }
            pattern[p] = NOTE_END;
            return;
        }
        if (arpMode == ARP_UP)
        {
            byte p = 0;
            byte lastFound = 0;
            for (; p < ARP_PATTERN_LEN; p++)
            {
                byte found = NOTE_END;
                for (byte i = 0; i < ARP_NOTES_COUNT; i++)
                {
                    // not necessary to do `notes[i] != NOTE_END &&` as long NOTE_END = 255
                    if (notes[i] > lastFound && notes[i] < found)
                    {
                        found = notes[i];
                    }
                }
                pattern[p] = found;
                lastFound = found;
                if (found == NOTE_END)
                {
                    return;
                }
            }
        }
    }

    byte getFreeNoteSpot(byte note)
    {
        for (byte i = 0; i < ARP_NOTES_COUNT; i++)
        {
            if (notes[i] == note)
            {
                return i;
            }
        }
        return NOTE_END;
    }

public:
    void setArpMode(byte mode)
    {
        arpMode = mode % ARP_COUNT;
    }

    void setMidi(MIDIDevice_BigBuffer *_midiGroovebox)
    {
        midiGroovebox = _midiGroovebox;
    }

    void next()
    {
        if (midiGroovebox)
        {
            midiGroovebox->sendNoteOff(lastStep, velocity, channel);
            lastStep = pattern[currentStep];
            midiGroovebox->sendNoteOff(lastStep, velocity, channel);
        }
        currentStep++;
        if (pattern[currentStep] == NOTE_END || currentStep == ARP_PATTERN_LEN)
        {
            currentStep = 0;
        }
    }

    void noteOn(byte _channel, byte note, byte _velocity)
    {
        channel = _channel;

        byte freeSpot = getFreeNoteSpot(NOTE_END);
        if (freeSpot != NOTE_END)
        {
            notes[freeSpot] = note;
        }
    }

    void noteOff(byte note)
    {
        byte pos = getFreeNoteSpot(note);
        if (pos != NOTE_END)
        {
            notes[pos] = NOTE_END;
        }
    }
};

#endif
