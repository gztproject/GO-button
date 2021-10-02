#ifndef BUTTON_C
#define BUTTON_C
#include "Button.h"

Button::Button(uint8_t btn, uint8_t led, void (*callback)(void))
{
    btnPin = btn;
    ledPin = led;
    action = callback;
    pinMode(btnPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    analogWrite(ledPin, LED_LOW_INT);
}

void Button::Tick()
{
    uint32_t time = millis();

    // Button released
    if (digitalRead(btnPin) == HIGH && (time - lastPressMillis > DEBOUNCE_TIME) && !btnPressed)
    {
        btnReleased = true;
        lastPressMillis = time;
    }

    // Button pressed
    if (digitalRead(btnPin) == LOW && (time - lastPressMillis > DEBOUNCE_TIME) && !btnPressed && btnReleased)
    {
        btnPressed = true;
        btnReleased = false;
        lastPressMillis = time;
    }

    // Action
    if (btnPressed && (time - lastPressMillis > MIN_PRESS))
    {
        //Callback
        action();
        btnPressed = false;
    }
}

#endif