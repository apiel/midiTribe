#ifndef IO_CONTROLLER_AKAI_MPK_MINI_H_
#define IO_CONTROLLER_AKAI_MPK_MINI_H_

#include <Arduino.h>
#include <USBHost_t36.h>

#include "io_loop_poly.h"
#include "io_pattern_editor.h"
#include "io_display.h"
#include "io_controller_akai_mpk_mini_lock.h"
#include "io_controller_akai_mpk_mini_live_loop.h"
#include "io_controller_akai_mpk_mini_live_synth.h"
#include "io_controller_akai_mpk_mini_pattern_editor.h"
#include "io_controller_akai_mpk_mini_def.h"

enum
{
    MODE_LOCK,
    MODE_LIVE_LOOP,
    MODE_LIVE_SYNTH,
    MODE_PATTERN_EDITOR,
    MODE_COUNT
};

class IO_ControllerAkaiMPKmini
{
protected:
    byte mode = MODE_LOCK;

    IO_Display *display;

    bool modeSustainPressed = false;
    byte currentChannel = 1;

    IO_ControllerAkaiMPKminiLock modeLock;
    IO_ControllerAkaiMPKminiLiveLoop modeLiveLoop;
    IO_ControllerAkaiMPKminiLiveSynth modeLiveSynth;
    IO_ControllerAkaiMPKminiPatternEditor modePattermEditor;

    const char *getModeName()
    {
        switch (mode)
        {
        case MODE_LOCK:
            return "Locked";
        case MODE_LIVE_LOOP:
            return "Live Loop";
        case MODE_LIVE_SYNTH:
            return "Live Synth";
        case MODE_PATTERN_EDITOR:
            return "Pattern Editor";
        }
        return "Unknown";
    }

public:
    IO_ControllerAkaiMPKmini(IO_Display *_display, IO_Poly_Loop **_loops, IO_PatternEditor *_editor) : modeLock(_display),
                                                                                                       modeLiveLoop(_display, _loops, &currentChannel),
                                                                                                       modeLiveSynth(_display, &currentChannel),
                                                                                                       modePattermEditor(_display, _editor)
    {
        display = _display;
        setMode(mode);
    }

    void setMidiGroovebox(MIDIDevice_BigBuffer *_midi)
    {
        modeLiveSynth.setMidiGroovebox(_midi);
        modeLock.setMidiGroovebox(_midi);
    }

    void setMidiController(MIDIDevice_BigBuffer *_midi)
    {
        modeLock.setMidiController(_midi);
    }

    void setMode(byte pos)
    {
        mode = pos % MODE_COUNT;
        display->displayValue("Mode", getModeName());
        // Serial.println("Set mode");
        render();
    }

    void render()
    {
        switch (mode)
        {
        case MODE_LIVE_LOOP:
            modeLiveLoop.render();
            break;
        case MODE_LIVE_SYNTH:
            modeLiveSynth.render();
            break;
        case MODE_LOCK:
            modeLock.render();
            break;
        case MODE_PATTERN_EDITOR:
            modePattermEditor.render();
            break;
        }
    }

    void noteOnHandler(byte channel, byte note, byte velocity)
    {
        if (modeSustainPressed)
        {
            if (channel == PAD_CHANNEL)
            {
                setMode(note - PAD_1);
            }
            else if (setChannelFromNote(note))
            {
                display->displayValue("Channel", currentChannel);
                render();
            }
            else
            {
                display->displayValue("Channel", "Out of range");
            }
            return;
        }

        switch (mode)
        {
        case MODE_LIVE_LOOP:
            modeLiveLoop.noteOnHandler(channel, note, velocity);
            break;
        case MODE_LIVE_SYNTH:
            modeLiveSynth.noteOnHandler(channel, note, velocity);
            break;
        case MODE_LOCK:
            modeLock.noteOnHandler(channel, note, velocity);
            break;
        case MODE_PATTERN_EDITOR:
            modePattermEditor.noteOnHandler(channel, note, velocity);
            break;
        }
    }

    void noteOffHandler(byte channel, byte note, byte velocity)
    {
        if (modeSustainPressed)
        {
            display->displayValue("Mode", getModeName());
            return;
        }

        switch (mode)
        {
        case MODE_LIVE_LOOP:
            modeLiveLoop.noteOffHandler(channel, note, velocity);
            break;
        case MODE_LIVE_SYNTH:
            modeLiveSynth.noteOffHandler(channel, note, velocity);
            break;
        case MODE_LOCK:
            modeLock.noteOffHandler(channel, note, velocity);
            break;
        case MODE_PATTERN_EDITOR:
            modePattermEditor.noteOffHandler(channel, note, velocity);
            break;
        }
    }

    void controlChangeHandler(byte channel, byte control, byte value)
    {
        if (control == 64) // when pressin sustain button
        {
            modeSustainPressed = value == 127;
            // Serial.println("Mode sustain");
            display->displayValue("Mode", getModeName());
            return;
        }

        if (modeSustainPressed)
        {
            display->displayValue("Mode", getModeName());
            return;
        }

        switch (mode)
        {
        case MODE_LIVE_LOOP:
            modeLiveLoop.controlChangeHandler(channel, control, value);
            break;
        case MODE_LIVE_SYNTH:
            modeLiveSynth.controlChangeHandler(channel, control, value);
            break;
        case MODE_LOCK:
            modeLock.controlChangeHandler(channel, control, value);
            break;
        case MODE_PATTERN_EDITOR:
            modePattermEditor.controlChangeHandler(channel, control, value);
            break;
        }
    }

    byte setChannelFromNote(byte note)
    {
        switch (note)
        {
        case _C3:
            currentChannel = 1;
            return currentChannel;
        case _D3:
            currentChannel = 2;
            return currentChannel;
        case _E3:
            currentChannel = 3;
            return currentChannel;
        case _F3:
            currentChannel = 4;
            return currentChannel;
        case _G3:
            currentChannel = 5;
            return currentChannel;
        case _A3:
            currentChannel = 6;
            return currentChannel;
        case _B3:
            currentChannel = 7;
            return currentChannel;
        case _C4:
            currentChannel = 8;
            return currentChannel;
        case _D4:
            currentChannel = 9;
            return currentChannel;
        case _E4:
            currentChannel = 10;
            return currentChannel;
        case _F4:
            currentChannel = 11;
            return currentChannel;
        case _G4:
            currentChannel = 12;
            return currentChannel;
        case _A4:
            currentChannel = 13;
            return currentChannel;
        case _B4:
            currentChannel = 14;
            return currentChannel;
        case _C5:
            currentChannel = 15;
            return currentChannel;
        case _D5:
            currentChannel = 16;
            return currentChannel;
        }
        return 0;
    }
};

#endif
