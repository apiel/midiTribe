#ifndef PATTERN_H_
#define PATTERN_H_

#include "./Step.h"

#define MAX_STEPS_IN_PATTERN 32
#define PATTERN_NAME_SIZE 15
#define SLIDE 1
#define END 1

typedef byte Pat[MAX_STEPS_IN_PATTERN][2];

class Pattern
{
public:
    byte pos;
    byte stepCount = MAX_STEPS_IN_PATTERN;
    Step steps[MAX_STEPS_IN_PATTERN];

    Pattern(Pat _steps)
    {
        for (byte pos = 0; pos < MAX_STEPS_IN_PATTERN; pos++)
        {
            if (_steps[pos][0] == END) {
                stepCount = pos;
                break;
            }
            steps[pos].set(_steps[pos][0], _steps[pos][1] == 1);
        }
    }

    // for testing
    void print()
    {
        for (byte x = 0; x < stepCount; x++)
        {
            Serial.print("[");
            Serial.print(x);
            Serial.print(", ");
            Serial.print(steps[x].note);
            Serial.print(", ");
            Serial.print(steps[x].slide);
            Serial.print("],");
            if (x % 8 == 7)
            {
                Serial.println("");
            }
        }
        Serial.println("\n");
    }
};

#endif
