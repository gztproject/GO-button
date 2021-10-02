#include <Arduino.h>
#include <config.h>
#include <Keyboard.h>

bool btn1Pressed = false;
bool btn1Released = true;
uint32_t lastPressMillis1 = 0;

bool btn2Pressed = false;
bool btn2Released = true;
uint32_t lastPressMillis2 = 0;

bool btn3Pressed = false;
bool btn3Released = true;
uint32_t lastPressMillis3 = 0;

bool btn4Pressed = false;
bool btn4Released = true;
uint32_t lastPressMillis4 = 0;

void flashLed(int pin, uint16_t time);

void setup()
{
  pinMode(BTN1PIN, INPUT_PULLUP);
  pinMode(BTN2PIN, INPUT_PULLUP);
  pinMode(BTN3PIN, INPUT_PULLUP);
  pinMode(BTN4PIN, INPUT_PULLUP);

  pinMode(BTN1LED, OUTPUT);
  pinMode(BTN2LED, OUTPUT);
  pinMode(BTN3LED, OUTPUT);
  pinMode(BTN4LED, OUTPUT);
  analogWrite(BTN1LED, LED_LOW_INT);
  analogWrite(BTN2LED, LED_LOW_INT);
  analogWrite(BTN3LED, LED_LOW_INT);
  analogWrite(BTN4LED, LED_LOW_INT);
  
  Keyboard.begin();
}

void loop()
{
  uint32_t time = millis();

  // Buttons released
  if (digitalRead(BTN1PIN) == HIGH && (time - lastPressMillis1 > DEBOUNCE_TIME) && !btn1Pressed)
  {
    btn1Released = true; 
    lastPressMillis1 = time;  
  }

  if (digitalRead(BTN2PIN) == HIGH && (time - lastPressMillis2 > DEBOUNCE_TIME) && !btn2Pressed)
  {
    btn2Released = true;
    lastPressMillis2 = time;
  }

  if (digitalRead(BTN3PIN) == HIGH && (time - lastPressMillis3 > DEBOUNCE_TIME) && !btn3Pressed)
  {
    btn3Released = true;
    lastPressMillis3 = time;
  }

  if (digitalRead(BTN4PIN) == HIGH && (time - lastPressMillis4 > DEBOUNCE_TIME) && !btn4Pressed)
  {
    btn4Released = true;
    lastPressMillis4 = time;
  }

  // Buttons pressed
  if (digitalRead(BTN1PIN) == LOW && (time - lastPressMillis1 > DEBOUNCE_TIME) && !btn1Pressed && btn1Released)
  {
    btn1Pressed = true;
    btn1Released = false;
    lastPressMillis1 = time;
  }

  if (digitalRead(BTN2PIN) == LOW && (time - lastPressMillis2 > DEBOUNCE_TIME) && !btn2Pressed && btn2Released)
  {
    btn2Pressed = true;
    btn2Released = false;
    lastPressMillis2 = time;
  }

  if (digitalRead(BTN3PIN) == LOW && (time - lastPressMillis3 > DEBOUNCE_TIME) && !btn3Pressed && btn3Released)
  {
    btn3Pressed = true;
    btn3Released = false;
    lastPressMillis3 = time;
  }

  if (digitalRead(BTN4PIN) == LOW && (time - lastPressMillis4 > DEBOUNCE_TIME) && !btn4Pressed && btn4Released)
  {
    btn4Pressed = true;
    btn4Released = false;
    lastPressMillis4 = time;
  }

  // Actions

  if (btn1Pressed && (time - lastPressMillis1 > MIN_PRESS))
  {
    //Keyboard.press('+');
    Keyboard.press(0xDA);
    Keyboard.releaseAll();
    btn1Pressed = false;
  }

  if (btn2Pressed && (time - lastPressMillis2 > MIN_PRESS))
  {
    //Keyboard.press('-');
    Keyboard.press(0xD9);
    Keyboard.releaseAll();
    btn2Pressed = false;
  }

  if (btn3Pressed && (time - lastPressMillis3 > MIN_PRESS))
  {
    Keyboard.press(' ');
    Keyboard.releaseAll();
    btn3Pressed = false;
  }

  if (btn4Pressed && (time - lastPressMillis4 > MIN_PRESS))
  {
    Keyboard.press('S');
    Keyboard.releaseAll();
    btn4Pressed = false;
  }
}

//LED stuff

void flashLed(int pin, uint16_t time)
{
  analogWrite(pin, LED_HIGH_INT);
  //delay(time);
  analogWrite(pin, LED_LOW_INT);
}