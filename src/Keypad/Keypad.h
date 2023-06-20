#ifndef _KEYPAD_H
#define _KEYPAD_H

#include "config.h"

#include "Button.h"
#include "preset.h"

#include <HID-Project.h>
#include <MIDIUSB.h>

//#include "settings/settings.h"

namespace Keypad
{
    bool Init(Preset *pres);

    bool SelectPreset(Preset preset);

    void Tick();

    void PresetCallback(uint8_t btn, KeyActions action);
    void KeypadAction(uint8_t btn, KeyActions action);
}
#endif