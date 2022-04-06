#ifndef IO_MIDI_H_
#define IO_MIDI_H_

#include <Arduino.h>
#include <USBHost_t36.h>

#include "io_display.h"
#include "io_core.h"
#include "io_utils.h"

#define MIDI_USB_COUNT 3

USBHost myusb;
USBHub hub1(myusb);
USBHub hub2(myusb);
USBHub hub3(myusb);
MIDIDevice_BigBuffer midi[MIDI_USB_COUNT] = MIDIDevice_BigBuffer(myusb);

MIDIDevice_BigBuffer *midiController = NULL;
MIDIDevice_BigBuffer *midiGroovebox = NULL;

void noteOnController(byte channel, byte note, byte velocity)
{
    Serial.print("Note On controller, ch=");
    Serial.print(channel, DEC);
    Serial.print(", note=");
    Serial.print(note, DEC);
    Serial.print(", velocity=");
    Serial.println(velocity, DEC);

    controller.noteOnHandler(channel, note, velocity);
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

    controller.noteOffHandler(channel, note, velocity);
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

    controller.controlChangeHandler(channel, control, value);
    display.update();
}

void noteOnGroovebox(byte channel, byte note, byte velocity)
{
    Serial.print("Note On Groovebox, ch=");
    Serial.print(channel, DEC);
    Serial.print(", note=");
    Serial.print(note, DEC);
    Serial.print(", velocity=");
    Serial.println(velocity, DEC);

    // controller.noteOnHandler(channel, note, velocity);
    display.update();
}

void noteOffGroovebox(byte channel, byte note, byte velocity)
{
    Serial.print("Note Off Groovebox, ch=");
    Serial.print(channel, DEC);
    Serial.print(", note=");
    Serial.print(note, DEC);
    Serial.print(", velocity=");
    Serial.println(velocity, DEC);

    // controller.noteOffHandler(channel, note, velocity);
    display.update();
}

void controlChangeGroovebox(byte channel, byte control, byte value)
{
    Serial.print("Control Change Groovebox, ch=");
    Serial.print(channel, DEC);
    Serial.print(", control=");
    Serial.print(control, DEC);
    Serial.print(", value=");
    Serial.println(value, DEC);

    // controller.controlChangeHandler(channel, control, value);
    display.update();
}

void sysExHandlerGroovebox(const uint8_t *data, uint16_t length, bool complete)
{
    Serial.printf("sysExHandlerGroovebox data (%d) %s\n", length, complete ? "complete" : "not complete");
    printIntArray(data, length);
    Serial.println("");
}

void clockHandlerGroovebox()
{
    // Serial.println("clock");
    clockHandler();
}

void midiInit()
{
    myusb.begin();
}

bool midiNeedToFindProduct = true;
unsigned long lastMidiProductCheck = millis();
void midiLoop()
{
    myusb.Task();
    for (byte n = 0; n < MIDI_USB_COUNT; n++)
    {
        while (midi[n].read())
            ;
    }

    // To use with arduino IDE, usb type: Serial + Midi
    if (usbMIDI.read())
    {
        byte type = usbMIDI.getType();       // which MIDI message, 128-255
        byte channel = usbMIDI.getChannel(); // which MIDI channel, 1-16
        byte data1 = usbMIDI.getData1();     // first data byte of message, 0-127
        byte data2 = usbMIDI.getData2();     // second data byte of message, 0-127

        switch (type)
        {
        case usbMIDI.NoteOn: // 0x90
            noteOnController(channel, data1, data2);
            break;

        case usbMIDI.NoteOff: // 0x80
            noteOffController(channel, data1, data2);
            break;

        case usbMIDI.ControlChange: // 0xB0
            controlChangeController(channel, data1, data2);
            break;
        }
    }

    // TODO
    // might find a way to detect disconnect
    // in /home/alex/dev/arduino/arduino-1.8.19/hardware/teensy/avr/libraries/USBHost_t36/USBHost_t36.h
    // enable debug with #define USBHOST_PRINT_DEBUG true
    if (midiNeedToFindProduct && millis() - lastMidiProductCheck >= 1000) // check every seconds
    {
        for (byte n = 0; n < MIDI_USB_COUNT; n++)
        {
            Serial.printf("\nVendor %d: %i %s %i %s", n, midi[n].idVendor(), midi[n].manufacturer(), midi[n].idProduct(), midi[n].product());

            // 09E8 = 2536, 007C = 124
            // Vendor 0: 2536 124 MPK mini
            if (midi[n].idVendor() == 2536 && midi[n].idProduct() == 124)
            {
                if (!midiController)
                {
                    midiController = &midi[n];
                    midiController->setHandleNoteOn(noteOnController);
                    midiController->setHandleNoteOff(noteOffController);
                    midiController->setHandleControlChange(controlChangeController);
                    controller.setMidiController(midiController);
                }
            }
            // 0944 = 2372, 012D = 301 Korg electribe2
            else if (midi[n].idVendor() == 2372 && midi[n].idProduct() == 301)
            {
                if (!midiGroovebox)
                {
                    midiGroovebox = &midi[n];
                    midiGroovebox->setHandleNoteOn(noteOnController);
                    midiGroovebox->setHandleNoteOff(noteOffController);
                    midiGroovebox->setHandleControlChange(controlChangeController);
                    midiGroovebox->setHandleSysEx(sysExHandlerGroovebox);
                    midiGroovebox->setHandleClock(clockHandlerGroovebox);
                    controller.setMidiGroovebox(midiGroovebox);
                    for (byte i = 0; i < 16; i++)
                    {
                        loopsPtr[i]->setMidi(midiGroovebox);
                    }
                }
            }
        }
        lastMidiProductCheck = millis();
        midiNeedToFindProduct = midiGroovebox == NULL || midiController == NULL;
        // // This might be removed if teensy could work in standalone
        // if (midiGroovebox == NULL)
        // {
        //     display.displayValue("Please connect", "Groovebox");
        // }
        // else if (midiController == NULL)
        // {
        //     display.displayValue("Please connect", "Controller");
        // }
        // display.update();
    }
}

#endif
