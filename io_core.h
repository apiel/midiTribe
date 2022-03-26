#ifndef IO_CORE_H_
#define IO_CORE_H_

#include "io_display.h"
#include "io_loop.h"
#include "controller/akai_mpk_mini/io_controller_akai_mpk_mini.h"

IO_Loop loop0, loop1, loop2, loop3, loop4, loop5, loop6, loop7,
    loop8, loop9, loop10, loop11, loop12, loop13, loop14, loop15;

IO_Loop *loopsPtr[16] = {
    &loop0, &loop1, &loop2, &loop3, &loop4, &loop5, &loop6, &loop7,
    &loop8, &loop9, &loop10, &loop11, &loop12, &loop13, &loop14, &loop15};

IO_ControllerAkaiMPKmini controller(&display, loopsPtr);

void clockHandler()
{
    for (byte i = 0; i < 16; i++)
    {
        loopsPtr[i]->next();
    }
}

#endif
