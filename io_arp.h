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
    byte notes[ARP_NOTES_COUNT] = {NOTE_END, NOTE_END, NOTE_END, NOTE_END, NOTE_END, NOTE_END};
    byte pattern[ARP_PATTERN_LEN];

    byte arpMode = 0;
    byte currentStep = 0;
    byte lastStep = NOTE_END;

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
                    break;
                }
            }
            return;
        }
        if (arpMode == ARP_DOWN)
        {
            byte p = 0;
            byte lastFound = 255;
            for (; p < ARP_PATTERN_LEN; p++)
            {
                byte found = 0;
                for (byte i = 0; i < ARP_NOTES_COUNT; i++)
                {
                    if (notes[i] != NOTE_END && notes[i] < lastFound && notes[i] > found)
                    {
                        found = notes[i];
                    }
                }
                pattern[p] = found;
                lastFound = found;
                if (found == 0)
                {
                    pattern[p] = NOTE_END;
                    break;
                }
            }
            return;
        }
        if (arpMode == ARP_UP_DOWN)
        {
            byte p = 0;
            byte lastFound = 0;
            bool up = true;
            for (; p < ARP_PATTERN_LEN; p++)
            {
                byte found = NOTE_END;
                if (up)
                {
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
                        up = false;
                    }
                }
                else
                {
                    for (byte i = 0; i < ARP_NOTES_COUNT; i++)
                    {
                        if (notes[i] != NOTE_END && notes[i] < lastFound && notes[i] > found)
                        {
                            found = notes[i];
                        }
                    }
                    pattern[p] = found;
                    lastFound = found;
                    if (found == 0 || pattern[p] == pattern[0])
                    {
                        pattern[p] = NOTE_END;
                        break;
                    }
                }
            }
            return;
        }
    }

    byte getNoteSpot(byte note)
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
        setPattern();
    }

    void setMidi(MIDIDevice_BigBuffer *_midiGroovebox)
    {
        midiGroovebox = _midiGroovebox;
    }

    void next()
    {
        if (midiGroovebox)
        {
            if (pattern[currentStep] == NOTE_END || currentStep == ARP_PATTERN_LEN)
            {
                currentStep = 0;
            }
            if (lastStep && lastStep != NOTE_END)
            {
                midiGroovebox->sendNoteOff(lastStep, velocity, channel);
            }
            lastStep = pattern[currentStep];
            if (lastStep && lastStep != NOTE_END)
            {
                // Serial.println(lastStep);
                midiGroovebox->sendNoteOn(lastStep, velocity, channel);
            }
        }
        currentStep++;
    }

    void noteOn(byte _channel, byte note, byte _velocity)
    {
        channel = _channel;

        byte freeSpot = getNoteSpot(NOTE_END);
        if (freeSpot != NOTE_END)
        {
            notes[freeSpot] = note;
        }
        setPattern();
    }

    void noteOff(byte note)
    {
        byte pos = getNoteSpot(note);
        if (pos != NOTE_END)
        {
            notes[pos] = NOTE_END;
        }
        setPattern();
    }

    const char *getModeName()
    {
        switch (arpMode)
        {
        case ARP_IN_ORDER:
            return "In order";

        case ARP_UP:
            return "Up";

        case ARP_DOWN:
            return "Down";

        case ARP_UP_DOWN:
            return "Up & Down";
        }
        return "Unknown";
    }
};

#endif
