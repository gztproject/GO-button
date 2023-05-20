#ifndef _KEYPAD_H
#define _KEYPAD_H

#include "config.h"

#include "Button.h"
#include <NeoPixelBus.h>

#include <HID-Project.h>
#include <MIDIUSB.h>

#include "settings.h"
#include "preset.h"

class Keypad
{
public:
    Keypad();
    bool Init();

    bool SelectPreset(Preset preset);

    void Tick();

private:
    unsigned long start;
    unsigned long lastMillis;
    unsigned long time;
    bool presetSelected;
    Button buttons[NUM_BUTTONS];
    NeoPixelBus<NeoGrbFeature, NeoWs2812Method> strip;
    Preset activePreset;

    bool setKeyboardMode();
    bool setHidMode();
    bool setMidiMode();

    void midiNoteOn(byte channel, byte pitch, byte velocity);
    void midiNoteOff(byte channel, byte pitch, byte velocity);
    void midiControlChange(byte channel, byte control, byte value);
};

#endif