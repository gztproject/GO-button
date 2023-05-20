#ifndef BUTTON_C
#define BUTTON_C
#include "Button.h"

Button::Button()
{
    btnPin = 0;
    baseColor = RgbColor{0};
    baseIntensity = 0;
    accentColor = RgbColor{0};
    accentIntensity = 0;
    ledColorState = RgbColor(0);
    actionOn = {};
    actionOff = {};
}

Button::Button(uint8_t btn, RgbColor col, uint8_t ledInt, void (*callbackOn)(void), void (*callbackOff)(void))
{
    btnPin = btn;
    baseColor = col;
    baseIntensity = ledInt;
    accentColor = col;
    accentIntensity = ledInt;
    ledColorState = RgbColor(0);
    actionOn = callbackOn;
    actionOff = callbackOff;
    pinMode(btnPin, INPUT_PULLUP);
    LedOff();
}

Button::Button(uint8_t btn, RgbColor bCol, uint8_t bInt, RgbColor aCol, uint8_t aInt, void (*callbackOn)(void), void (*callbackOff)(void))
{
    btnPin = btn;
    baseColor = bCol;
    baseIntensity = bInt;
    accentColor = aCol;
    accentIntensity = aInt;
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
            LedAccent();
            actionOn();
        }
        else
        {
            if ((ledIntensityState > baseIntensity) && flashingSpeed == 0)
                LedBase();
            actionOff();
        }
    }

    // LED flashing stuff
    if (flashingSpeed != 0 && (time - lastFlash > flashingSpeed))
    {
        // If we flashed out our count or it's indefinite turn flashing off.
        if (flashes != 0 && flashCount >= flashes)
        {
            // Serial.println("Flashing off.");
            FlashOff();
        }
        else
        {
            if (ledIntensityState > baseIntensity)
            {
                // Serial.println("Flashing --");
                LedBase();
            }
            else
            {
                // Serial.println("Flashing ++");
                LedAccent();
                flashCount++;
            }
            lastFlash = time;
        }
    }
}

void Button::LedOn(RgbColor color, uint8_t intensity)
{
    ledColorState = color.Dim(intensity);
    ledIntensityState = intensity;
}

/** @deprecated */
void Button::LedDimm()
{
    LedBase();
}

void Button::LedBase()
{
    LedOn(baseColor, baseIntensity);
}

void Button::LedAccent()
{
    LedOn(accentColor, accentIntensity);
}

void Button::FlashOff()
{
    flashingSpeed = 0;
    LedDimm();
}

void Button::LedOff()
{
    ledIntensityState = 0;
    ledColorState = RgbColor(0);
}

void Button::SetBaseColor(RgbColor col, uint8_t intensity)
{
    baseColor = col;
    baseIntensity = intensity;
    if (ledIntensityState > 0 && ledIntensityState < accentIntensity)
    {
        LedBase();
    }
}

void Button::SetAccentColor(RgbColor col, uint8_t intensity)
{
    accentColor = col;
    accentIntensity = intensity;
    if (ledIntensityState > baseIntensity)
    {
        LedAccent();
    }
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