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
    byte playedNote[POLY_LOOP_COUNT];
    byte nextNotes[POLY_LOOP_COUNT];
    byte previousLoopNotes[POLY_LOOP_COUNT];
    Step lastSteps[POLY_LOOP_COUNT];

    byte getFreeNoteSpot(byte note)
    {
        for (byte i = 0; i < POLY_LOOP_COUNT; i++)
        {
            if (nextNotes[i] == note)
            {
                return i;
            }
        }
        return 255;
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

    void setNextToPlay() override
    {
        for (byte i = 0; i < POLY_LOOP_COUNT; i++)
        {
            playedNote[i] = nextNotes[i];
        }
    }

    bool stopNote(byte note)
    {
        byte pos = getFreeNoteSpot(note);
        if (pos != 255)
        {
            nextNotes[pos] = 255;
            return true;
        }
        return false;
    }

public:
    IO_Poly_Loop()
    {
        for (byte i = 0; i < POLY_LOOP_COUNT; i++)
        {
            playedNote[i] = 255;
            nextNotes[i] = 255;
            previousLoopNotes[i] = 255;
        }
    }

    void noteOn(byte channel, byte note, byte velocity)
    {
        if (!modeSingleLoop && stopNote(note))
        {
            return;
        }
        byte freeSpot = getFreeNoteSpot(255);
        // Serial.printf("Press note on in poly %d (%d)\n", note, freeSpot);
        if (freeSpot != 255)
        {
            nextNotes[freeSpot] = note;
        }
        IO_Loop::noteOn(channel, note, velocity);
    }

    void noteOff(byte note)
    {
        if (modeSingleLoop)
        {
            stopNote(note);
        }
    }

    void toggleMode()
    {
        modeSingleLoop = !modeSingleLoop;
        if (modeSingleLoop)
        {
            for (byte i = 0; i < POLY_LOOP_COUNT; i++)
            {
                previousLoopNotes[i] = nextNotes[i];
                nextNotes[i] = 255;
            }
        }
        else
        {
            for (byte i = 0; i < POLY_LOOP_COUNT; i++)
            {
                nextNotes[i] = previousLoopNotes[i];
            }
        }
    }
};

#endif
