#ifndef BUTTON_C
#define BUTTON_C
#include "Button.h"

Button::Button(uint8_t btn, uint8_t led, void (*callbackOn)(void), void (*callbackOff)(void))
{
    btnPin = btn;
    ledPin = led;
    actionOn = callbackOn;
    actionOff = callbackOff;
    pinMode(btnPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    LedOff();
}

void Button::SetCallbacks(void (*callbackOn)(void), void (*callbackOff)(void))
{
    actionOn = callbackOn;
    actionOff = callbackOff;
}

void Button::Tick()
{
    uint32_t time = millis();

    // Button released
    if (digitalRead(btnPin) == HIGH && (time - lastPressMillis > DEBOUNCE_TIME) && !btnPressed && !btnReleased)
    {
        if (ledState && flashingSpeed == 0)
            LedDimm();
        actionOff();
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
        LedOn();
        actionOn();
        btnPressed = false;
    }

    //LED flashing stuff
    if (flashingSpeed > 0)
    {
        if (time - lastFlash > flashingSpeed)
        {
            if (flashes > 0 && flashCount >= flashes)
            {
                FlashOff();
            }
            else
            {
                if (ledState)
                {
                    ledState = false;
                    analogWrite(ledPin, LED_LOW_INT);
                }
                else
                {
                    LedOn(flashIntensity);
                    flashCount++;
                }
                lastFlash = time;
            }
        }
    }
}

void Button::LedOn(uint8_t intensity)
{
    ledState = true;
    analogWrite(ledPin, intensity);
}

void Button::FlashOff()
{
    flashingSpeed = 0;
    LedDimm();
}

void Button::LedOff()
{
    ledState = false;
    digitalWrite(ledPin, LOW);
}

void Button::LedDimm()
{
    ledState = false;
    analogWrite(ledPin, LED_LOW_INT);
}

void Button::Flash(uint8_t number, uint16_t speed, uint8_t intensity)
{
    flashes = number;
    flashCount = 0;
    flashingSpeed = speed;
    flashIntensity = intensity;
    lastFlash = millis();
}

#endif