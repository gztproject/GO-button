#include <Arduino.h>
#include <config.h>
#include "Button.h"
#include <Keyboard.h>
#include "MIDIUSB.h"

typedef enum Modes : uint8_t
{
  UNKNOWN = 00,
  KB_MODE = 10,
  MIDI = 20,
} Modes;

void setMidiMode();
void midiCallbackOn1();
void midiCallbackOn2();
void midiCallbackOn3();
void midiCallbackOn4();
void midiCallbackOff1();
void midiCallbackOff2();
void midiCallbackOff3();
void midiCallbackOff4();
void noteOn(byte channel, byte pitch, byte velocity);
void noteOff(byte channel, byte pitch, byte velocity);
void controlChange(byte channel, byte control, byte value);

void setKeyboardMode();
void kbCallbackOn1();
void kbCallbackOn2();
void kbCallbackOn3();
void kbCallbackOn4();
void kbCallbackOff1();
void kbCallbackOff2();
void kbCallbackOff3();
void kbCallbackOff4();

void emptyCallback();

Modes currentMode = UNKNOWN;

Button btn1(BTN1PIN, BTN1LED, emptyCallback, emptyCallback);
Button btn2(BTN2PIN, BTN2LED, emptyCallback, emptyCallback);
Button btn3(BTN3PIN, BTN3LED, setKeyboardMode, emptyCallback);
Button btn4(BTN4PIN, BTN4LED, setMidiMode, emptyCallback);

void setup()
{
  btn3.Flash();
  btn4.Flash();
  uint32_t time = millis();
  while (currentMode == UNKNOWN && millis() - time < 3000)
  {
    btn1.Tick();
    btn2.Tick();
    btn3.Tick();
    btn4.Tick();
  }
  btn1.LedDimm();
  btn2.LedDimm();
  btn3.LedDimm();
  btn4.LedDimm();
  if (currentMode == UNKNOWN)
    setMidiMode();
}

void loop()
{
  btn1.Tick();
  btn2.Tick();
  btn3.Tick();
  btn4.Tick();
}

void setMidiMode()
{
  btn3.FlashOff();
  btn4.FlashOff();
  btn4.Flash(5,100);
  currentMode = MIDI;
  btn1.SetCallbacks(midiCallbackOn1, midiCallbackOff1);
  btn2.SetCallbacks(midiCallbackOn2, midiCallbackOff2);
  btn3.SetCallbacks(midiCallbackOn3, midiCallbackOff3);
  btn4.SetCallbacks(midiCallbackOn4, midiCallbackOff4);  
}

void midiCallbackOn1()
{
  noteOn(MIDI_CH, BTN1NOTE, MIDI_VELOCITY);
}
void midiCallbackOn2()
{
  noteOn(MIDI_CH, BTN2NOTE, MIDI_VELOCITY);
}
void midiCallbackOn3()
{
  noteOn(MIDI_CH, BTN3NOTE, MIDI_VELOCITY);
}
void midiCallbackOn4()
{
  noteOn(MIDI_CH, BTN4NOTE, MIDI_VELOCITY);
}
void midiCallbackOff1()
{
  noteOff(MIDI_CH, BTN1NOTE, MIDI_VELOCITY);
}
void midiCallbackOff2()
{
  noteOff(MIDI_CH, BTN2NOTE, MIDI_VELOCITY);
}
void midiCallbackOff3()
{
  noteOff(MIDI_CH, BTN3NOTE, MIDI_VELOCITY);
}
void midiCallbackOff4()
{
  noteOff(MIDI_CH, BTN4NOTE, MIDI_VELOCITY);
}

void noteOn(byte channel, byte pitch, byte velocity)
{
  if (MidiUSB.connectionAvailable() == 0)
  {
    midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
    MidiUSB.sendMIDI(noteOn);
  }
  MidiUSB.flush();
}

void noteOff(byte channel, byte pitch, byte velocity)
{
  if (MidiUSB.connectionAvailable() == 0)
  {
    midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
    MidiUSB.sendMIDI(noteOff);
  }
  MidiUSB.flush();
}

void controlChange(byte channel, byte control, byte value)
{
  if (MidiUSB.connectionAvailable() == 0)
  {
    midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
    MidiUSB.sendMIDI(event);
  }
  MidiUSB.flush();
}

void setKeyboardMode()
{
  btn3.FlashOff();
  btn4.FlashOff();
  btn3.Flash(5, 100);
  currentMode = KB_MODE;
  Keyboard.begin();
  btn1.SetCallbacks(kbCallbackOn1, kbCallbackOff1);
  btn2.SetCallbacks(kbCallbackOn2, kbCallbackOff2);
  btn3.SetCallbacks(kbCallbackOn3, kbCallbackOff3);
  btn4.SetCallbacks(kbCallbackOn4, kbCallbackOff4);
}

void kbCallbackOn1()
{
  Keyboard.press(BTN1CHAR);
}

void kbCallbackOn2()
{
  Keyboard.press(BTN2CHAR);
}

void kbCallbackOn3()
{
  Keyboard.press(BTN3CHAR);
}

void kbCallbackOn4()
{
  Keyboard.press(BTN4CHAR);
}

void kbCallbackOff1()
{
  Keyboard.release(BTN1CHAR);
}

void kbCallbackOff2()
{
  Keyboard.release(BTN2CHAR);
}

void kbCallbackOff3()
{
  Keyboard.release(BTN3CHAR);
}

void kbCallbackOff4()
{
  Keyboard.release(BTN4CHAR);
}

void emptyCallback()
{
}