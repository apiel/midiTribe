#ifndef IO_LOOP_H_
#define IO_LOOP_H_

#include <Arduino.h>

#include "Pattern.h"
#include "io_patterns.h"

#define REF_NOTE _C4
#define PATTERN_SELECTOR_COUNT 4

class IO_Loop
{
private:
    IO_AudioSynth *env;

    byte currentStep = 0;
    Step lastStep;

public:
    byte id = 0;
    bool active = true;
    bool modeSingleLoop = true;
    byte nextToPlay = 0;
    byte play = 0;
    byte previousLoopNote = 0;

    byte patternSelector[PATTERN_SELECTOR_COUNT] = {2, 1, 0, 0};
    byte currentPatternSelector = 0;
    byte nextPattern = 0;
    Pattern *pattern = &patterns[nextPattern];

    IO_Loop(IO_AudioSynth *_env, byte _id) : IO_AudioLoop(_env)
    {
        id = _id;
    }

    IO_Loop(IO_AudioSynth *_env)
    {
        env = _env;
        setCurrentPatternSelector(currentPatternSelector);
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
        if (!lastStep.slide)
        {
            env->noteOff(lastStep.note);
            // to avoid repeating this again, let set slide to true
            lastStep.slide = true;
        }

        if (play)
        {
            Step *step = &pattern->steps[currentStep];
            if (step->note > 0)
            {
                lastStep.set(step);
                // add note difference to note
                lastStep.note += (int)play - (int)REF_NOTE;
                env->noteOn(lastStep.note, lastStep.velocity);
                // could have mode with no substain here
                // and directly noteOff
                // for drum?
                // but should this be part of the envelop and not from the sequencer
            }
        }
        currentStep = (currentStep + 1) % pattern->stepCount;

        if (currentStep == 0)
        {
            pattern = &patterns[nextPattern];
            play = nextToPlay ? nextToPlay : 0;
        }
    }

    void noteOn(byte note)
    {
        if (active)
        {
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
