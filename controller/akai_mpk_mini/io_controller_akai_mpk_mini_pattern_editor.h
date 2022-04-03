#ifndef IO_CONTROLLER_AKAI_MPK_MINI_PATTERN_EDITOR_H_
#define IO_CONTROLLER_AKAI_MPK_MINI_PATTERN_EDITOR_H_

#include <Arduino.h>
#include <USBHost_t36.h>

#include "io_display.h"
#include "io_pattern_editor.h"
#include "note.h"
#include "io_controller_akai_mpk_mini_def.h"

class IO_ControllerAkaiMPKminiPatternEditor
{
protected:
    IO_Display *display;
    IO_PatternEditor *editor;

    char val[10];
    char name[70];

    void addNoteToName(byte pos)
    {
        if (editor->getStep(pos)->note == 0)
        {
            snprintf(name, 70, "%s--- ", name);
        }
        else
        {
            snprintf(name, 70, "%s%s%d%c", name, 
                getNote2Str(editor->getStep(pos)->note), 
                getNoteOctave(editor->getStep(pos)->note),
                editor->getStep(pos)->slide ? '^' : ' '
            );
        }
    }

    byte mod(int8_t a, byte b)
    {
        int8_t c = a % b;
        return (c < 0) ? c + b : c;
    }

public:
    IO_ControllerAkaiMPKminiPatternEditor(IO_Display *_display, IO_PatternEditor *_editor)
    {
        display = _display;
        editor = _editor;
    }

    void render()
    {
        snprintf(name, 70, "Pattern Editor\n\n");
        addNoteToName(-2);
        addNoteToName(-1);
        addNoteToName(0);
        addNoteToName(1);
        addNoteToName(2);
        snprintf(name, 70, "%s\n\n %02d  %02d  %02d  %02d  %02d\n\n", name,
                 mod(editor->stepPos - 2, 4) + 1, mod(editor->stepPos - 1, 4) + 1,
                 editor->stepPos + 1,
                 mod(editor->stepPos + 1, 4) + 1, mod(editor->stepPos + 2, 4) + 1);

        display->setDefaultName(name);

        if (editor->getStep()->note == 0)
        {
            display->setDefaultValue("    --", 2);
        }
        else
        {
            snprintf(val, 10, "    %s%d", getNoteStr(editor->getStep()->note), getNoteOctave(editor->getStep()->note));
            display->setDefaultValue(val, 2);
        }
    }

    // TODO play midi note in the same time
    // TODO pressing note should set note and increment pos
    // TODO use pad as well to increment from one empty step
    void noteOnHandler(byte channel, byte note, byte velocity)
    {
        if (channel == PAD_CHANNEL)
        {
            // TODO use pad to set slid on current and previous step
            // TODO set empty step
            return;
        }
        editor->getStep()->note = note;
        editor->stepPos = (editor->stepPos + 1) % editor->pattern->stepCount;
        render();
    }

    void noteOffHandler(byte channel, byte note, byte velocity)
    {
    }

    void controlChangeHandler(byte channel, byte control, byte value)
    {
        if (control == 13)
        {
            editor->setPattern(value / 128.0f * PATTERN_COUNT);
            display->displayValue("Edit pattern", editor->patternPos);
        }
        else if (control == 14)
        {
            editor->setStep(value / 128.0f * editor->pattern->stepCount);
            render();
        }
        else if (control == 15)
        {
            editor->pattern->stepCount = value / 128.0f * MAX_STEPS_IN_PATTERN;
            display->displayValue("Set steps count", editor->pattern->stepCount);
        }
    }
};

#endif
