#ifndef IO_CONTROLLER_AKAI_MPK_MINI_PATTERN_EDITOR_H_
#define IO_CONTROLLER_AKAI_MPK_MINI_PATTERN_EDITOR_H_

#include <Arduino.h>
#include <USBHost_t36.h>

#include "io_display.h"
#include "io_pattern_editor.h"
#include "note.h"

class IO_ControllerAkaiMPKminiPatternEditor
{
protected:
    IO_Display *display;
    IO_PatternEditor editor;

    char val[7];

public:
    IO_ControllerAkaiMPKminiPatternEditor(IO_Display *_display)
    {
        display = _display;
    }

    void render()
    {
        display->setDefaultName("Pattern Editor\n\nStep     Note\n\n ");
        if (editor.getStep()->note == 0) {
        snprintf(val, 7, "%02d --", editor.stepPos + 1);
        } else {
        snprintf(val, 7, "%02d %s%d", editor.stepPos + 1, getNoteStr(editor.getStep()->note), getNoteOctave(editor.getStep()->note));
        }
        display->setDefaultValue(val, 3);
    }

    // TODO play midi note in the same time
    // TODO pressing note should set note and increment pos
    // TODO use pad as well to increment from one empty step
    void noteOnHandler(byte channel, byte note, byte velocity)
    {
    }

    void noteOffHandler(byte channel, byte note, byte velocity)
    {
    }

    void controlChangeHandler(byte channel, byte control, byte value)
    {
        if (control == 13)
        {
            editor.setPattern(value / 128.0f * PATTERN_COUNT);
            display->displayValue("Edit pattern", editor.patternPos);
        }
        else if (control == 14)
        {
            editor.setStep(value / 128.0f * MAX_STEPS_IN_PATTERN);
            render();
        }
        // TODO be able to select length
    }
};

#endif
