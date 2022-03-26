#ifndef IO_LOOP_H_
#define IO_LOOP_H_

#include <Arduino.h>
#include <USBHost_t36.h>

#include "Pattern.h"
#include "io_patterns.h"

#define REF_NOTE _C4
#define PATTERN_SELECTOR_COUNT 4

class IO_Loop
{
protected:
    byte currentStep = 0;
    Step lastStep;
    MIDIDevice_BigBuffer *midiGroovebox = NULL;
    byte velocity = 100;
    byte nextVelocity = 100;

    byte nextToPlay = 0;


public:
    byte channel = 1;
    bool active = true;
    bool modeSingleLoop = true;
    byte play = 0;
    byte previousLoopNote = 0;

    byte patternSelector[PATTERN_SELECTOR_COUNT] = {2, 1, 0, 0};
    byte currentPatternSelector = 0;
    byte nextPattern = 0;
    Pattern *pattern = &patterns[nextPattern];

    IO_Loop()
    {
        setCurrentPatternSelector(currentPatternSelector);
    }

    void setMidi(MIDIDevice_BigBuffer *_midiGroovebox)
    {
        midiGroovebox = _midiGroovebox;
    }

    void setPatternSelector(byte pos, byte value)
    {
        patternSelector[pos % PATTERN_SELECTOR_COUNT] = value;
    }

    void setCurrentPatternSelector(byte value)
    {
        currentPatternSelector = value % PATTERN_SELECTOR_COUNT;
        nextPattern = patternSelector[currentPatternSelector] % PATTERN_COUNT;
    }

    void next()
    {
        if (midiGroovebox)
        {
            if (!lastStep.slide)
            {
                midiGroovebox->sendNoteOff(lastStep.note, lastStep.velocity, channel);
                // To avoid repeating this again, let set slide to true
                lastStep.slide = true;
            }

            if (play)
            {
                Step *step = &pattern->steps[currentStep];
                if (step->note > 0)
                {
                    lastStep.set(step);
                    // Add note difference to note
                    lastStep.note += (int)play - (int)REF_NOTE;
                    midiGroovebox->sendNoteOn(lastStep.note, velocity, channel);
                }
            }
        }
        currentStep = (currentStep + 1) % pattern->stepCount;

        if (currentStep == 0)
        {
            pattern = &patterns[nextPattern];
            play = nextToPlay ? nextToPlay : 0;
            velocity = nextVelocity;
        }
    }

    void noteOn(byte _channel, byte note, byte _velocity)
    {
        if (active)
        {
            channel = _channel;
            nextVelocity = _velocity;
            nextToPlay = note;
        }
    }

    void noteOff(byte note)
    {
        if (modeSingleLoop && note == nextToPlay)
        {
            nextToPlay = 0;
        }
    }

    void toggle() { activate(!active); }
    void activate(bool value = true) { active = value; }

    void toggleMode() { activateSingleLoopMode(!modeSingleLoop); }
    void activateSingleLoopMode(bool value = true)
    {
        modeSingleLoop = value;
        if (modeSingleLoop)
        {
            previousLoopNote = nextToPlay;
            nextToPlay = 0;
        }
        else
        {
            nextToPlay = previousLoopNote;
        }
    }
};

#endif
