#include <Arduino.h>

#include "Keypad/Keypad.h"

Keypad keypad;

Preset presets[NUM_PRESETS];

void setup()
{
  for (uint8_t i = 0; i < NUM_PRESETS; i++)
  {
    presets[i] = Preset(i);
  }

  presets[0].SetColor(blue, LED_HIGH_INT);
  presets[0].SetName({"MIDI"}, 5);
  presets[0].SetMode(HwModes::MIDI_MODE);
  Action actions[NUM_BUTTONS];

  actions[0] = {
      .callbackOn = noteOn(MIDI_CH, BTN_0_NOTE, MIDI_VELOCITY),
      .callbackOff = {},
      .baseColor = blue,
      .accentColor = blue,
      .baseIntensity = LED_LOW_INT,
      .accentIntensity = LED_HIGH_INT};

  presets[0].SetActions(actions);

  keypad.Init();
}

void loop()
{
  keypad.Tick();
}

void midiCallbackOn0()
{
  noteOn(MIDI_CH, BTN_0_NOTE, MIDI_VELOCITY);
}
void midiCallbackOn1()
{
  noteOn(MIDI_CH, BTN_1_NOTE, MIDI_VELOCITY);
}
void midiCallbackOn2()
{
  noteOn(MIDI_CH, BTN_2_NOTE, MIDI_VELOCITY);
}
void midiCallbackOn3()
{
  noteOn(MIDI_CH, BTN_3_NOTE, MIDI_VELOCITY);
}
void midiCallbackOn4()
{
  noteOn(MIDI_CH, BTN_4_NOTE, MIDI_VELOCITY);
}
void midiCallbackOff0()
{
  noteOff(MIDI_CH, BTN_0_NOTE, MIDI_VELOCITY);
}
void midiCallbackOff1()
{
  noteOff(MIDI_CH, BTN_1_NOTE, MIDI_VELOCITY);
}
void midiCallbackOff2()
{
  noteOff(MIDI_CH, BTN_2_NOTE, MIDI_VELOCITY);
}
void midiCallbackOff3()
{
  noteOff(MIDI_CH, BTN_3_NOTE, MIDI_VELOCITY);
}
void midiCallbackOff4()
{
  noteOff(MIDI_CH, BTN_4_NOTE, MIDI_VELOCITY);
}

void setKeyboardMode()
{
}

void kbCallbackOn0()
{
  Keyboard.press(BTN_0_CHAR);
}

void kbCallbackOn1()
{
  Keyboard.press(BTN_1_CHAR);
}

void kbCallbackOn2()
{
  Keyboard.press(BTN_2_CHAR);
}

void kbCallbackOn3()
{
  Keyboard.press(BTN_3_CHAR);
}

void kbCallbackOn4()
{
  Keyboard.press(BTN_4_CHAR);
}

void kbCallbackOff0()
{
  Keyboard.release(BTN_0_CHAR);
}

void kbCallbackOff1()
{
  Keyboard.release(BTN_1_CHAR);
}

void kbCallbackOff2()
{
  Keyboard.release(BTN_2_CHAR);
}

void kbCallbackOff3()
{
  Keyboard.release(BTN_3_CHAR);
}

void kbCallbackOff4()
{
  Keyboard.release(BTN_4_CHAR);
}

void hidCallbackOn0()
{
  Consumer.press(BTN_0_HID);
}

void hidCallbackOn1()
{
  Consumer.press(BTN_1_HID);
}

void hidCallbackOn2()
{
  Consumer.press(BTN_2_HID);
}

void hidCallbackOn3()
{
  Consumer.press(BTN_3_HID);
}

void hidCallbackOn4()
{
  Consumer.press(BTN_4_HID);
}

void hidCallbackOff0()
{
  Consumer.release(BTN_0_HID);
}

void hidCallbackOff1()
{
  Consumer.release(BTN_1_HID);
}

void hidCallbackOff2()
{
  Consumer.release(BTN_2_HID);
}

void hidCallbackOff3()
{
  Consumer.release(BTN_3_HID);
}

void hidCallbackOff4()
{
  Consumer.release(BTN_4_HID);
}
