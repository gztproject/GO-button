#include <Arduino.h>

#include "../lib/settings/settings.h"

#include <NeoPixelBus.h>
#include "Button.h"
// #include <Keyboard.h>
#include "MIDIUSB.h"
#include "HID-Project.h"

#define SERIAL_DEBUG 0

#include <config.h>

void setMidiMode();
void midiCallbackOn0();
void midiCallbackOn1();
void midiCallbackOn2();
void midiCallbackOn3();
void midiCallbackOn4();
void midiCallbackOff0();
void midiCallbackOff1();
void midiCallbackOff2();
void midiCallbackOff3();
void midiCallbackOff4();
void noteOn(byte channel, byte pitch, byte velocity);
void noteOff(byte channel, byte pitch, byte velocity);
void controlChange(byte channel, byte control, byte value);

void setKeyboardMode();
void kbCallbackOn0();
void kbCallbackOn1();
void kbCallbackOn2();
void kbCallbackOn3();
void kbCallbackOn4();
void kbCallbackOff0();
void kbCallbackOff1();
void kbCallbackOff2();
void kbCallbackOff3();
void kbCallbackOff4();

void setHIDMode();
void hidCallbackOn0();
void hidCallbackOn1();
void hidCallbackOn2();
void hidCallbackOn3();
void hidCallbackOn4();
void hidCallbackOff0();
void hidCallbackOff1();
void hidCallbackOff2();
void hidCallbackOff3();
void hidCallbackOff4();

void emptyCallback();

SETTINGS::Modes currentMode = SETTINGS::Modes::UNKNOWN;

unsigned long start;
unsigned long lastMillis;
unsigned long time;

// three element pixels, in different order and speeds
NeoPixelBus<NeoGrbFeature, NeoWs2812Method> strip(NUM_BUTTONS, LED_PIN);

RgbColor red(255, 0, 0);
RgbColor green(0, 255, 0);
RgbColor blue(0, 0, 255);
RgbColor yellow(255, 255, 0);
RgbColor purple(255, 0, 255);
RgbColor cyan(0, 255, 255);
RgbColor white(255);
RgbColor black(0);

Button buttons[NUM_BUTTONS] = {
    Button(BTN_0_PIN, BTN_0_COL, setKeyboardMode, emptyCallback),
    Button(BTN_1_PIN, BTN_1_COL, setMidiMode, emptyCallback),
    Button(BTN_2_PIN, BTN_2_COL, setHIDMode, emptyCallback),
    Button(BTN_3_PIN, BTN_3_COL, emptyCallback, emptyCallback),
    Button(BTN_4_PIN, BTN_4_COL, emptyCallback, emptyCallback)};

void setup()
{
  // this resets all the neopixels to an off state
  strip.Begin();

  // Serial.begin(115200);

  for (uint8_t i = 0; i < NUM_BUTTONS; i++)
  {
    buttons[i].Tick();
    strip.SetPixelColor(i, buttons[i].GetLedColorState());
  }
  strip.Show();

  buttons[0].SetLedColor(red);
  buttons[1].SetLedColor(cyan);
  buttons[2].SetLedColor(purple);

  buttons[0].Flash();
  buttons[1].Flash();
  buttons[2].Flash();

  start = millis();
  lastMillis = millis();
  time = millis();

  // For the first 3 second
  while (currentMode == SETTINGS::Modes::UNKNOWN && time - start < 3000)
  {

    time = millis();

    for (uint8_t i = 0; i < NUM_BUTTONS; i++)
    {
      buttons[i].Tick();
      strip.SetPixelColor(i, buttons[i].GetLedColorState());
    }

    if (time % 50 == 0 && lastMillis != time)
    {
      // Serial.println(time);
      strip.Show();
      lastMillis = time;
    }
  }

  if (currentMode == SETTINGS::Modes::UNKNOWN)
  {
    switch (DEFAULT_MODE)
    {
    case SETTINGS::Modes::KB_MODE:
      setKeyboardMode();
      break;
    case SETTINGS::Modes::MIDI_MODE:
      setMidiMode();
      break;
    case SETTINGS::Modes::HID_MODE:
      setHIDMode();
      break;
    }
  }


  buttons[0].SetDefaultColor();
  buttons[1].SetDefaultColor();
  buttons[2].SetDefaultColor();

  for (uint8_t i = 0; i < NUM_BUTTONS; i++)
  {
    buttons[i].LedDimm();
    strip.SetPixelColor(i, buttons[i].GetLedColorState());
  }

  strip.Show();  
}

void loop()
{
  time = millis();
  
  for (uint8_t i = 0; i < 5; i++)
  {
    buttons[i].Tick();
    strip.SetPixelColor(i, buttons[i].GetLedColorState());
  }

  if (time % 50 == 0 && lastMillis != time)
  {
    // Serial.println(time);
    strip.Show();
    lastMillis = time;
  }
}

void setMidiMode()
{
  buttons[0].FlashOff();
  buttons[1].FlashOff();
  buttons[2].FlashOff();
  buttons[1].Flash(5, 100);
  currentMode = SETTINGS::Modes::MIDI_MODE;
  buttons[0].SetCallbacks(midiCallbackOn0, midiCallbackOff0);
  buttons[1].SetCallbacks(midiCallbackOn1, midiCallbackOff1);
  buttons[2].SetCallbacks(midiCallbackOn2, midiCallbackOff2);
  buttons[3].SetCallbacks(midiCallbackOn3, midiCallbackOff3);
  buttons[4].SetCallbacks(midiCallbackOn4, midiCallbackOff4);
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

void noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity)
{
  if (MidiUSB.connectionAvailable() == 0)
  {
    midiEventPacket_t noteOn = {0x09, uint8_t(0x90 | channel), pitch, velocity};
    MidiUSB.sendMIDI(noteOn);
  }
  MidiUSB.flush();
}

void noteOff(uint8_t channel, uint8_t pitch, uint8_t velocity)
{
  if (MidiUSB.connectionAvailable() == 0)
  {
    midiEventPacket_t noteOff = {0x08, uint8_t(0x80 | channel), pitch, velocity};
    MidiUSB.sendMIDI(noteOff);
  }
  MidiUSB.flush();
}

void controlChange(uint8_t channel, uint8_t control, uint8_t value)
{
  if (MidiUSB.connectionAvailable() == 0)
  {
    midiEventPacket_t event = {0x0B, uint8_t(0xB0 | channel), control, value};
    MidiUSB.sendMIDI(event);
  }
  MidiUSB.flush();
}

void setKeyboardMode()
{
  buttons[0].FlashOff();
  buttons[1].FlashOff();
  buttons[2].FlashOff();
  buttons[0].Flash(5, 100);
  currentMode = SETTINGS::Modes::KB_MODE;
  Keyboard.begin();
  buttons[0].SetCallbacks(kbCallbackOn0, kbCallbackOff0);
  buttons[1].SetCallbacks(kbCallbackOn1, kbCallbackOff1);
  buttons[2].SetCallbacks(kbCallbackOn2, kbCallbackOff2);
  buttons[3].SetCallbacks(kbCallbackOn3, kbCallbackOff3);
  buttons[4].SetCallbacks(kbCallbackOn4, kbCallbackOff4);
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

void setHIDMode()
{
  buttons[0].FlashOff();
  buttons[1].FlashOff();
  buttons[2].FlashOff();
  buttons[2].Flash(5, 100);
  currentMode = SETTINGS::Modes::HID_MODE;
  Consumer.begin();
  buttons[0].SetCallbacks(hidCallbackOn0, hidCallbackOff0);
  buttons[1].SetCallbacks(hidCallbackOn1, hidCallbackOff1);
  buttons[2].SetCallbacks(hidCallbackOn2, hidCallbackOff2);
  buttons[3].SetCallbacks(hidCallbackOn3, hidCallbackOff3);
  buttons[4].SetCallbacks(hidCallbackOn4, hidCallbackOff4);
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

void emptyCallback()
{
}