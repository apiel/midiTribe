#ifndef IO_PATTERN_EDITOR_H_
#define IO_PATTERN_EDITOR_H_

#include <Arduino.h>

#include "io_patterns.h"

class IO_PatternEditor {
// TODO
// load pattern from electribe using sysex

// TODO
// edit pattern manually with PAD and Keyboard

public:
    byte patternPos = 0;
    Pattern * pattern = &patterns[patternPos];


};

#endif
