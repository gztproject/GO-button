#ifndef BUTTON_C
#define BUTTON_C
#include "Button.h"

Button::Button(uint8_t btn, RgbColor col, void (*callbackOn)(void), void (*callbackOff)(void))
{
    btnPin = btn;
    color = col;
    ledColorState = RgbColor(0);
    actionOn = callbackOn;
    actionOff = callbackOff;
    pinMode(btnPin, INPUT_PULLUP);
    LedOff();
}

void Button::SetCallbacks(void (*callbackOn)(void), void (*callbackOff)(void))
{
    actionOn = callbackOn;
    actionOff = callbackOff;
}

void Button::Tick()
{
    unsigned long time = millis();
    bool newState = (digitalRead(btnPin) == LOW);

    // Button state changed for enough time (DEBOUNCE_TIME)
    if (oldState != newState && (time - lastChangeMillis > DEBOUNCE_TIME))
    {
        oldState = newState;
        lastChangeMillis = time;

        if (newState)
        {
            LedOn();
            actionOn();
        }
        else
        {
            if (ledState && flashingSpeed == 0)
                LedDimm();
            actionOff();
        }
    }

    // LED flashing stuff
    if (flashingSpeed != 0 && (time - lastFlash > flashingSpeed))
    {
        //If we flashed out our count or it's indefinite turn flashing off.
        if (flashes != 0 && flashCount >= flashes)
        {
            FlashOff();
        }
        else
        {
            if (ledState)
            {
                LedDimm();
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

void Button::LedOn(uint8_t intensity)
{
    ledState = true;
    ledColorState = color.Dim(intensity);
}

void Button::FlashOff()
{
    flashingSpeed = 0;
    LedDimm();
}

void Button::LedOff()
{
    ledState = false;
    ledColorState = RgbColor(0);
}

void Button::LedDimm()
{
    ledState = false;
    LedOn(LED_LOW_INT);
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