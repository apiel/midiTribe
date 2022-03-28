#ifndef IO_PATTERNS_H_
#define IO_PATTERNS_H_

#include "Pattern.h"

#define PATTERN_COUNT 25

// NOTE
// Should there be some banks?
// selectable with the lower row of pots
Pattern patterns[PATTERN_COUNT] = {
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C3, SLIDE}, {_C3}, {0}, {_C3}, {END}},
    (Pat){{_C3}, {_E3}, {_F3}, {_C3}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
};

// void showPatterns()
// {
//     for (byte pos = 0; pos < PATTERN_COUNT; pos++)
//     {
//         Serial.printf("pattern %d:\n", pos);
//         patterns[pos].print();
//     }
// }

#endif
