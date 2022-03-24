#ifndef IO_CONTROLLER_AKAI_MPK_MINI_BASE_H_
#define IO_CONTROLLER_AKAI_MPK_MINI_BASE_H_

#include <Arduino.h>
#include <Audio.h>

#include "../../audio/io_audio_synth.h"
#include "../../io_audio_loop.h"
#include "../../io_drum_machine.h"
#include "../../io_instrument_list.h"
#include "../../io_display.h"

#define PAD_CHANNEL 10
#define PAD_1 36
#define PAD_2 37
#define PAD_3 38
#define PAD_4 39
#define PAD_5 40
#define PAD_6 41
#define PAD_7 42
#define PAD_8 43

enum
{
    MODE_MAIN_ARP,
    MODE_MAIN_SYNTH,
    MODE_EDIT_SYNTH,
    MODE_DRUM,
    MODE_LOCK,
    MODE_COUNT
};

class IO_ControllerAkaiMPKminiBase
{
public:
    IO_AudioLoop *loopPadPressed = NULL;
    bool loopPadPressedDidAction = false;
    byte mode = MODE_MAIN_ARP;

    IO_AudioLoop **loops;
    IO_AudioSynth **synths;

    IO_AudioLoop *loop;
    IO_AudioSynth *synth;
    IO_DrumMachine *drums;

    IO_Display *display;

    bool modeSustainPressed = false;

    byte currentPattern = 0;
    Pattern *pattern;

    byte padPressed = 0;

    // for the moment will always be arpMode
    bool arpMode = true;

    IO_AudioSynth *getSynth(byte pos) { return synths[pos % SYNTH_COUNT]; }
    IO_AudioLoop *getLoop(byte pos) { return loops[pos % SYNTH_COUNT]; }

    void setCurrentSynth(byte value)
    {
        byte pos = value % SYNTH_COUNT;
        Serial.printf("Set current synth %d\n", pos);
        loop = getLoop(pos);
        synth = getSynth(pos);
    }

    const char *getModeName()
    {
        switch (mode)
        {
        case MODE_MAIN_ARP:
            return "Live ARP";
        case MODE_MAIN_SYNTH:
            return "Live synth";
        case MODE_EDIT_SYNTH:
            return "Edit synth";
        case MODE_DRUM:
            return "Drums";
        case MODE_LOCK:
            return "Locked";
        }
        return "Unknown";
    }
};

#endif
