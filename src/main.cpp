#include <Arduino.h>
#include <config.h>
#include "Button.h"
#include <Keyboard.h>
#include "MIDIUSB.h"

void callback1();
void callback2();
void callback3();
void callback4();

Button btn1(BTN1PIN, BTN1LED, callback1);
Button btn2(BTN2PIN, BTN2LED, callback2);
Button btn3(BTN3PIN, BTN3LED, callback3);
Button btn4(BTN4PIN, BTN4LED, callback4);

void setup()
{
  Keyboard.begin();
}

void loop()
{
  btn1.Tick();
  btn2.Tick();
  btn3.Tick();
  btn4.Tick();
}

void callback1()
{
  Keyboard.press(0xDA);
  Keyboard.releaseAll();
}

void callback2()
{
  Keyboard.press(0xD9);
  Keyboard.releaseAll();
}

void callback3()
{
  Keyboard.press(' ');
  Keyboard.releaseAll();
}

void callback4()
{
  Keyboard.press('S');
  Keyboard.releaseAll();
}