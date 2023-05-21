#include <Arduino.h>

#include "Keypad/Keypad.h"

Preset presets[NUM_PRESETS];

void setup()
{
  for (uint8_t i = 0; i < NUM_PRESETS; i++)
  {
    presets[i] = Preset(i);
  }

  presets[0].SetColor(BLUE, LED_HIGH_INT);
  // char *name = "MIDI";
  // presets[0].SetName(name, 5);
  presets[0].SetMode(HwModes::MIDI_MODE);
  BtnPreset btnPresets[NUM_BUTTONS];

  btnPresets[0] = {
      .key = BTN_0_NOTE,
      .baseColor = BLUE,
      .accentColor = BLUE,
      .baseIntensity = LED_LOW_INT,
      .accentIntensity = LED_HIGH_INT};

  presets[0].SetButtons(btnPresets);

  Keypad::Init(presets);
}

void loop()
{
  Keypad::Tick();
}