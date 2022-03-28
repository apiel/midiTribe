#ifndef IO_PATTERN_EDITOR_H_
#define IO_PATTERN_EDITOR_H_

#include <Arduino.h>

#include "io_patterns.h"
#include "Pattern.h"
#include "Step.h"

class IO_PatternEditor
{
    // NOTE
    // should there be a way to load pattern from phone
    // and to save as well?

    // TODO
    // load pattern from electribe using sysex

    // TODO
    // edit pattern manually with PAD and Keyboard

    // TODO
    // save and load pattern

public:
    byte patternPos = 0;
    Pattern *pattern = &patterns[patternPos];

    byte stepPos = 0;

    void setPattern(byte pos)
    {
        stepPos = 0;
        patternPos = pos % PATTERN_COUNT;
        pattern = &patterns[patternPos];
    }

    void setStep(byte pos)
    {
        stepPos = pos;
    }

    Step * getStep()
    {
        return &pattern->steps[stepPos];
    }
};

#endif
