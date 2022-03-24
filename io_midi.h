#ifndef IO_MIDI_H_
#define IO_MIDI_H_

#include <Arduino.h>
#include <USBHost_t36.h>

#include "io_display.h"
#include "io_core.h"
#include "io_utils.h"

#define MIDI_COUNT 4

USBHost myusb;
USBHub hub1(myusb);
USBHub hub2(myusb);
USBHub hub3(myusb);
MIDIDevice midi[MIDI_COUNT] = MIDIDevice(myusb);

MIDIDevice *midiController = NULL;
MIDIDevice *midiGroovebox = NULL;

void noteOnController(byte channel, byte note, byte velocity)
{
    // When a USB device with multiple virtual cables is used,
    // midi[n].getCable() can be used to read which of the virtual
    // MIDI cables received this message.
    Serial.print("Note On controller, ch=");
    Serial.print(channel, DEC);
    Serial.print(", note=");
    Serial.print(note, DEC);
    Serial.print(", velocity=");
    Serial.println(velocity, DEC);

    // controller.noteOnHandler(channel, note, velocity);
    display.update();
}

void noteOffController(byte channel, byte note, byte velocity)
{
    Serial.print("Note Off controller, ch=");
    Serial.print(channel, DEC);
    Serial.print(", note=");
    Serial.print(note, DEC);
    Serial.print(", velocity=");
    Serial.println(velocity, DEC);

    // controller.noteOffHandler(channel, note, velocity);
    display.update();
}

void controlChangeController(byte channel, byte control, byte value)
{
    Serial.print("Control Change controller, ch=");
    Serial.print(channel, DEC);
    Serial.print(", control=");
    Serial.print(control, DEC);
    Serial.print(", value=");
    Serial.println(value, DEC);

    // controller.controlChangeHandler(channel, control, value);
    display.update();
}

void sysExHandler(MIDIDevice *midiTarget, const uint8_t *data, uint16_t length, bool complete)
{
    Serial.printf("SysExHandler data (%d) %s\n", length, complete ? "complete" : "not complete");
    for (uint16_t i = 0; i < length; i++)
    {
        Serial.printf("%d,", data[i]);
    }
    Serial.println("");

    const uint8_t akai[] = {125, 0, 187, 61, 0, 185};
    if (intArrayStartWith(akai, data, 0))
    {
        Serial.println("Is akai");
        midiController = midiTarget;
        midiController->setHandleNoteOn(noteOnController);
        midiController->setHandleNoteOff(noteOffController);
        midiController->setHandleControlChange(controlChangeController);
        return;
    }
}
// 125,122,187,61,92,185, // 125,122,187,61,85,185,
// 113,36,3,
// 96,241,245,24,246,255,71,
// 253,233,255,

void sysExHandler0(const uint8_t *data, uint16_t length, bool complete)
{
    sysExHandler(&midi[0], data, length, complete);
}

void sysExHandler1(const uint8_t *data, uint16_t length, bool complete)
{
    sysExHandler(&midi[1], data, length, complete);
}

void sysExHandler2(const uint8_t *data, uint16_t length, bool complete)
{
    sysExHandler(&midi[2], data, length, complete);
}

void sysExHandler3(const uint8_t *data, uint16_t length, bool complete)
{
    sysExHandler(&midi[3], data, length, complete);
}

void midiInit()
{
    myusb.begin();
    midi[0].setHandleSysEx(sysExHandler0);
    midi[1].setHandleSysEx(sysExHandler1);
    midi[2].setHandleSysEx(sysExHandler2);
    midi[3].setHandleSysEx(sysExHandler3);

    // for (byte n = 0; n < MIDI_COUNT; n++)
    // {
    //     midi[n].setHandleNoteOn(noteOnHandler);
    //     midi[n].setHandleNoteOff(noteOffHandler);
    //     midi[n].setHandleControlChange(controlChangeHandler);
    //     // midi[n].setHandleSysEx(sysExHandler);
    //     midi[n].setHandleSysEx(sysExHandlerYo(100 + n));
    //     // midi[n].setHandleClock
    //     // midi[n].setHandleProgramChange
    // }
}

void midiLoop()
{
    myusb.Task();
    for (byte n = 0; n < MIDI_COUNT; n++)
    {
        while (midi[n].read())
            ;
    }
}

#endif
