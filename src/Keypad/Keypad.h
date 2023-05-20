#ifndef _KEYPAD_H
#define _KEYPAD_H

#include "config.h"

#include "Button.h"
#include <NeoPixelBus.h>
#include "settings.h"
#include "preset.h"


class Keypad
{
public:
    Keypad();

private:
    Button buttons[NUM_BUTTONS];
    NeoPixelBus<NeoGrbFeature, NeoWs2812Method> strip;
    Preset activePreset;
};

#endif