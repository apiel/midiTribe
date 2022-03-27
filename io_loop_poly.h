#ifndef IO_LOOP_POLY_H_
#define IO_LOOP_POLY_H_

#include <Arduino.h>
#include <USBHost_t36.h>

#include "io_loop.h"

// #define POLY_LOOP_COUNT 3
#define POLY_LOOP_COUNT 6

class IO_Poly_Loop : public IO_Loop
{
protected:
    byte playedNote[POLY_LOOP_COUNT] = {255, 255, 255};
    Step lastSteps[POLY_LOOP_COUNT];

    byte getPlayedNotePos(byte note)
    {
        for (byte i = 0; i < POLY_LOOP_COUNT; i++)
        {
            if (playedNote[i] == note)
            {
                return i;
            }
        }
        return 255;
    }

    bool hasNoteToPlay()
    {
        for (byte i = 0; i < POLY_LOOP_COUNT; i++)
        {
            if (playedNote[i] != 255)
            {
                return true;
            }
        }
        return false;
    }

    void sendNoteOn(Step *step) override
    {
        for (byte i = 0; i < POLY_LOOP_COUNT; i++)
        {
            if (playedNote[i] != 255)
            {
                lastSteps[i].set(step);
                // Add note difference to note
                lastSteps[i].note += (int)playedNote[i] - (int)REF_NOTE;
                midiGroovebox->sendNoteOn(lastSteps[i].note, velocity, channel);
            }
        }
    }

    void sendNoteOff() override
    {
        for (byte i = 0; i < POLY_LOOP_COUNT; i++)
        {
            IO_Loop::sendNoteOff(&lastSteps[i]);
        }
    }

public:
    void noteOn(byte channel, byte note, byte velocity)
    {
        byte freeSpot = getPlayedNotePos(255);
        // Serial.printf("Press note on in poly %d (%d)\n", note, freeSpot);
        if (freeSpot != 255)
        {
            playedNote[freeSpot] = note;
        }
        IO_Loop::noteOn(channel, note, velocity);
    }

    void noteOff(byte note)
    {
        byte pos = getPlayedNotePos(note);
        if (pos != 255)
        {
            playedNote[pos] = 255;
        }

        if (modeSingleLoop && !hasNoteToPlay())
        {
            nextToPlay = 0;
        }
    }
};

#endif
