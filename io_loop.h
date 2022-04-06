#ifndef IO_LOOP_H_
#define IO_LOOP_H_

#include <Arduino.h>
#include <USBHost_t36.h>
#include <EEPROM.h>

#include "Pattern.h"
#include "io_patterns.h"

#define REF_NOTE _C4
#define PATTERN_SELECTOR_COUNT 4

class IO_Loop
{
protected:
    byte currentStep = 0;
    uint16_t eepromAddress = 0;
    Step lastStep;
    MIDIDevice_BigBuffer *midiGroovebox = NULL;
    byte velocity = 100;
    byte nextVelocity = 100;

    byte nextToPlay = 0;

    void activateSingleLoopMode(bool value = true)
    {
        modeSingleLoop = value;
        if (modeSingleLoop)
        {
            previousLoopNote = nextToPlay;
            nextToPlay = 0;
        }
        else
        {
            nextToPlay = previousLoopNote;
        }
    }

    virtual void sendNoteOn(Step *step)
    {
        if (play)
        {
            sendNoteOn(step, &lastStep);
        }
    }

    void sendNoteOn(Step *step, Step *_lastStep)
    {
        _lastStep->set(step);
        // Add note difference to note
        _lastStep->note += (int)play - (int)REF_NOTE;
        midiGroovebox->sendNoteOn(_lastStep->note, velocity, channel);
    }

    virtual void sendNoteOff()
    {
        sendNoteOff(&lastStep);
    }

    void sendNoteOff(Step *_lastStep)
    {
        if (!_lastStep->slide)
        {
            midiGroovebox->sendNoteOff(_lastStep->note, _lastStep->velocity, channel);
            // To avoid repeating this again, let set slide to true
            _lastStep->slide = true;
        }
    }

    virtual void setNextToPlay()
    {
        play = nextToPlay ? nextToPlay : 0;
    }

public:
    byte channel = 1;
    // need to rename this
    bool modeSingleLoop = true;
    byte play = 0;
    byte previousLoopNote = 0;

    byte patternSelector[PATTERN_SELECTOR_COUNT] = {0, 0, 0, 0};
    byte currentPatternSelector = 0;
    byte nextPattern = 0;
    Pattern *pattern = &patterns[nextPattern];

    IO_Loop()
    {
        setCurrentPatternSelector(currentPatternSelector);
    }

    void setMidi(MIDIDevice_BigBuffer *_midiGroovebox)
    {
        midiGroovebox = _midiGroovebox;
    }

    void load(byte pos)
    {
        // Serial.printf("Load loop pads %d\n", pos);
        eepromAddress = pos * PATTERN_SELECTOR_COUNT;
        for (byte i = 0; i < PATTERN_SELECTOR_COUNT; i++)
        {
            byte val = EEPROM.read(eepromAddress + i);
            patternSelector[i] = val == 255 ? 0 : val % PATTERN_COUNT;
            // Serial.printf("- %d %d\n", eepromAddress + i, patternSelector[i]);
        }
    }

    void setPatternSelector(byte pos, byte value)
    {
        pos = pos % PATTERN_SELECTOR_COUNT;
        patternSelector[pos] = value % PATTERN_COUNT;
        EEPROM.write(eepromAddress + pos, patternSelector[pos]);
        // Serial.printf("save pads %d %d\n", eepromAddress + pos, patternSelector[pos]);
    }

    void setCurrentPatternSelector(byte value)
    {
        currentPatternSelector = value % PATTERN_SELECTOR_COUNT;
        nextPattern = patternSelector[currentPatternSelector] % PATTERN_COUNT;
    }

    byte getCurrentPattern()
    {
        return patternSelector[currentPatternSelector];
    }

    void next()
    {
        if (midiGroovebox)
        {
            sendNoteOff();

            Step *step = &pattern->steps[currentStep];
            if (step->note > 0)
            {
                sendNoteOn(step);
            }
        }
        currentStep = (currentStep + 1) % pattern->stepCount;

        if (currentStep == 0)
        {
            pattern = &patterns[nextPattern];
            // play = nextToPlay ? nextToPlay : 0;
            setNextToPlay();
            velocity = nextVelocity;
        }
    }

    void noteOn(byte _channel, byte note, byte _velocity)
    {
        channel = _channel;
        nextVelocity = _velocity;
        nextToPlay = note;
        // Serial.printf("Loop should now play %d on ch %d with velo %d\n", nextToPlay, channel, _velocity);
    }

    void noteOff(byte note)
    {
        if (modeSingleLoop && note == nextToPlay)
        {
            nextToPlay = 0;
        }
    }

    void toggleMode() { activateSingleLoopMode(!modeSingleLoop); }

    byte getPatternAtBank(byte bankPos)
    {
        return patternSelector[bankPos];
    }
};

#endif
