#ifndef BUTTON_H
#define BUTTON_H

#include <stdio.h>
#include <Arduino.h>
#include <NeoPixelBus.h>

#define DEBOUNCE_TIME 10
#define MIN_PRESS 5

class Button
{
public:
    Button();
    Button(uint8_t btn, RgbColor col, uint8_t ledInt, void (*callbackOn)(void), void (*callbackOff)(void));
    Button(uint8_t btn, RgbColor bCol, uint8_t bInt, RgbColor aCol, uint8_t aInt, void (*callbackOn)(void), void (*callbackOff)(void));
    void SetCallbacks(void (*callbackOn)(void), void (*callbackOff)(void));
    void Tick();

    /**
     * Turns the button's LED on at high brightness
     */
    void LedOn() { LedAccent(); }
    /**
     * Turns the button's LED on at specified brightness
     */
    void LedOn(RgbColor color, uint8_t intensity);
    /**
     * Turns the button's LED to low intensity.
     */
    void LedDimm();
    void LedBase();
    void LedAccent();
    /**
     * Turns the button's LED off.
     */
    void LedOff();

    void SetBaseColor(RgbColor col, uint8_t intensity);
    void SetAccentColor(RgbColor col, uint8_t intensity);

    RgbColor GetLedColorState() { return ledColorState; };

    /**
     * Flashes the button's LED at high intensity at 0,5s intervals until turned off.
     */
    void Flash() { Flash(500); }
    /**
     * Flashes the button's LED at high intensity at specified speed until turned off.
     */
    void Flash(uint16_t speed) { Flash(0, speed, accentIntensity); }
    /**
     * Flashes the button's LED at high intensity at specified speed for number of times (0->idefinite).
     */
    void Flash(uint8_t number, uint16_t speed) { Flash(number, speed, accentIntensity); }
    /**
     * Flashes the button's LED at specified intensity and speed for number of times (0->idefinite).
     */
    void Flash(uint8_t number, uint16_t speed, uint8_t intensity);

    /**
     * Turn the flashing off.
     */
    void FlashOff();

private:
    uint8_t btnPin;

    RgbColor baseColor;
    RgbColor accentColor;

    uint8_t baseIntensity;
    uint8_t accentIntensity;

    RgbColor ledColorState;
    uint8_t ledIntensityState = 0;

    bool oldState = false;

    unsigned long lastChangeMillis = 0;
    void (*actionOn)(void);
    void (*actionOff)(void);

    // LED stuff
    unsigned long lastFlash = 0;
    /**
     * Current flashing time in ms. 0 means flashing is off.
     */
    uint16_t flashingSpeed = 0;
    uint8_t flashIntensity, flashes, flashCount = 0;
};

#endif