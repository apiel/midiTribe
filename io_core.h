#ifndef IO_CORE_H_
#define IO_CORE_H_

#include "io_display.h"
#include "io_loop_poly.h"
#include "controller/akai_mpk_mini/io_controller_akai_mpk_mini.h"

IO_Poly_Loop loop0, loop1, loop2, loop3, loop4, loop5, loop6, loop7,
    loop8, loop9, loop10, loop11, loop12, loop13, loop14, loop15;

IO_Poly_Loop *loopsPtr[16] = {
    &loop0, &loop1, &loop2, &loop3, &loop4, &loop5, &loop6, &loop7,
    &loop8, &loop9, &loop10, &loop11, &loop12, &loop13, &loop14, &loop15};

IO_PatternEditor editor;

IO_ControllerAkaiMPKmini controller(&display, loopsPtr, &editor);

byte clockCount = 0;
void clockHandler()
{
    if (clockCount == 0)
    {
        // clock is not right
        for (byte i = 0; i < 16; i++)
        {
            loopsPtr[i]->next();
        }
    }
    // Clock events are sent at a rate of 24 pulses per quarter note
    // 4 * 6 = 24
    clockCount = (clockCount + 1) % 6;
}

void initCore()
{
    for (byte i = 0; i < 16; i++)
    {
        loopsPtr[i]->load(i);
    }
}

#endif
