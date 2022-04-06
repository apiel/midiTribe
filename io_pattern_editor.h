#ifndef IO_PATTERN_EDITOR_H_
#define IO_PATTERN_EDITOR_H_

#include <Arduino.h>
#include <SD.h>

#include "io_patterns.h"
#include "Pattern.h"
#include "Step.h"
#include "io_utils.h"

class IO_PatternEditor
{
protected:
    bool sdAvailable = false;

    // TODO
    // load pattern from electribe using sysex

public:
    byte patternPos = 0;
    Pattern *pattern = &patterns[patternPos];

    byte stepPos = 0;

    void setPattern(byte pos)
    {
        stepPos = 0;
        patternPos = pos % PATTERN_COUNT;
        pattern = &patterns[patternPos];
    }

    void setStep(byte pos)
    {
        stepPos = pos;
    }

    Step *getStep()
    {
        return &pattern->steps[stepPos];
    }

    Step *getStep(byte pos)
    {
        return &pattern->steps[mod(stepPos + pos, pattern->stepCount)];
    }

    void setNote(byte note)
    {
        getStep()->note = note;
        stepPos = (stepPos + 1) % pattern->stepCount;
    }

    void toggleSlide(byte pos = 0)
    {
        Step *step = &pattern->steps[mod(stepPos + pos, pattern->stepCount)];
        step->slide = !step->slide;
    }

    void load()
    {
        sdAvailable = SD.begin(BUILTIN_SDCARD);
        if (!sdAvailable)
        {
            Serial.println("Unable to access the SD card");
        }
        else
        {
            Serial.println("SD card ready");
            char filePath[14];

            for (byte pos = 0; pos < PATTERN_COUNT; pos++)
            {
                snprintf(filePath, 14, "patterns/%03d.pat", pos);
                File file = SD.open(filePath);
                if (file)
                {
                    // Serial.println(filePath);
                    setPattern(pos);
                    pattern->stepCount = 1;
                    getStep()->reset();

                    while (file.available())
                    {
                        char c = file.read();
                        // Serial.print(c);
                        if (c == ' ' || c == '\n' || c == '*')
                        {
                            stepPos++;
                            getStep()->reset();
                            pattern->stepCount++;
                        }
                        else if (c == '_')
                        {
                            getStep()->slide = true;
                        }
                        else if (c == '#')
                        {
                            getStep()->note++;
                        }
                        else if (c >= '0' && c <= '8')
                        {
                            // C0 = 12
                            getStep()->note += (1 + (c - '0')) * 12;
                        }
                        else if (c == 'C' || c == 'c')
                        {
                            // do nothing
                        }
                        else if (c == 'D' || c == 'd')
                        {
                            getStep()->note += 2;
                        }
                        else if (c == 'E' || c == 'e')
                        {
                            getStep()->note += 4;
                        }
                        else if (c == 'F' || c == 'f')
                        {
                            getStep()->note += 5;
                        }
                        else if (c == 'G' || c == 'g')
                        {
                            getStep()->note += 7;
                        }
                        else if (c == 'A' || c == 'a')
                        {
                            getStep()->note += 9;
                        }
                        else if (c == 'B' || c == 'b')
                        {
                            getStep()->note += 11;
                        }
                    }
                    file.close();
                    return;
                }
            }
        }
    }

    bool save()
    {
        if (sdAvailable)
        {
            char filePath[14];
            snprintf(filePath, 14, "patterns/%03d.pat", patternPos);
            SD.remove(filePath);
            File file = SD.open(filePath, FILE_WRITE);

            if (file)
            {
                file.seek(0);
                for (byte pos = 0; pos < pattern->stepCount; pos++)
                {
                    if (pos > 0)
                    {
                        file.print(' ');
                    }

                    Step *step = getStep(pos);
                    file.print(getNoteStr(step->note));
                    file.print(getNoteOctave(step->note));
                    // Serial.printf("%s %d\n",getNoteStr(step->note), getNoteOctave(step->note));
                    if (step->slide)
                    {
                        file.print('_');
                    }
                }
                file.close();
                return true;
            }
        }
        return false;
    }
};

#endif
