#ifndef IO_CORE_H_
#define IO_CORE_H_

#include "io_display.h"
#include "io_loop.h"
#include "controller/akai_mpk_mini/io_controller_akai_mpk_mini.h"

IO_Loop loop0;
IO_Loop loop1; // just for test...
IO_Loop *loopsPtr[2] = {&loop0, &loop1};

IO_ControllerAkaiMPKmini controller(&display, loopsPtr);

#endif
