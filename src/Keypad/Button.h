/**
 * @file Button.h
 * @author Gašper Doljak (info@gzt.si)
 * @brief All the vars and logic that a single button needs
 * @version 2.0.1
 * @date 2023-06-21
 *
 *
 */
#ifndef BUTTON_H
#define BUTTON_H

#include <stdio.h>
#include "Preset.h"


/// @brief Time in ms to wait out the bouncing. After an initial state change subsequent changes will be ignored for this period.
#define DEBOUNCE_TIME 10

typedef enum _KeyActions : uint8_t
{
    ON,
    OFF,
    PRESS
} KeyActions;

class Button
{
public:
    /**
     * @brief Construct a new Button object
     *
     */
    Button();

    /**
     * @brief Construct a new Button object
     *
     * @param _id Button ID - should be sequential number between 0 - NUM_BUTTONS
     * @param pin Button pin
     * @param col Button color - both base and accent
     * @param ledInt Button LED intensity - both base and accent
     * @param _callback Button callback function
     * 
     * @sa Button::SetCallback(_callback)
     */
    Button(uint8_t _id, uint8_t pin, RgbColor col, uint8_t ledInt, void (*_callback)(uint8_t i, KeyActions action));

    /**
     * @brief Construct a new Button object
     *
     * @param _id Button ID - should be sequential number between 0 - NUM_BUTTONS
     * @param pin Button pin
     * @param bCol Unscaled base color
     * @param bInt Base LED intensity
     * @param aCol Unscaled accent color
     * @param aInt Accent LED intensity
     * @param _callback Button callback function
     * 
     * @sa Button::SetCallback(_callback)
     */
    Button(uint8_t _id, uint8_t pin, RgbColor bCol, uint8_t bInt, RgbColor aCol, uint8_t aInt, void (*_callback)(uint8_t i, KeyActions action));

    /**
     * @brief Set the Callback object
     *
     * @param _callback A callback function to call on button change
     */
    void SetCallback(void (*_callback)(uint8_t i, KeyActions action));

    /**
     * @brief A non-blocking task that does all the work. It should be called every 5ms -> MIN ( DEBOUNCE_TIME, MIN_PRESS )
     *
     */
    void Tick();

    /**
     * @brief Turns the button's LED on with accent color
     * @deprecated Use LedAccent() in the future.
     *
     */
    void LedOn() { LedAccent(); }

    /**
     * @brief Turns the button's LED on at specified brightness
     *
     * @param color Unscaled RgbColor
     * @param intensity Intensity that will be used to scale-dimm the color
     */
    void LedOn(RgbColor color, uint8_t intensity);

    /**     
     * @brief Turns the button's LED on with base color
     * @deprecated Use LedBase() in the future.
     *
     */
    void LedDimm() { LedBase(); }

    /**
     * @brief Turns the button's LED to base color.
     *
     */
    void LedBase();

    /**
     * @brief Turns the button's LED to accent color.
     *
     */
    void LedAccent();

    /**
     * @brief Turns the button's LED off.
     *
     */
    void LedOff();

    /**
     * @brief Set the button base color - on during standby
     *
     * @param col Unscaled RgbColor
     * @param intensity Intensity that will be used to scale-dimm the color
     */
    void SetBaseColor(RgbColor col, uint8_t intensity);

    /**
     * @brief Set the button accent color - for flashing and during keypresses
     *
     * @param col Unscaled RgbColor
     * @param intensity Intensity that will be used to scale-dimm the color
     */
    void SetAccentColor(RgbColor col, uint8_t intensity);

    /**
     * @brief Get the current LED color state
     *
     * @return RgbColor
     */
    RgbColor GetLedColorState() { return ledColorState; };

    /**
     * @brief Flashes the button's LED at high intensity at 0,5s intervals until turned off.
     */
    void Flash() { Flash(500); }

    /**
     * @brief Flashes the button's LED at high intensity at specified speed until turned off.
     *
     * @param speed Flashing period in ms
     */
    void Flash(uint16_t speed) { Flash(0, speed, accentIntensity); }

    /**
     * @brief Flashes the button's LED at high intensity at specified speed for number of times.
     *
     * @param number Number of flashes (0->until turned off)
     * @param speed Flashing period in ms
     */
    void Flash(uint8_t number, uint16_t speed) { Flash(number, speed, accentIntensity); }

    /**
     * @brief Flashes the button's LED at specified intensity and speed for number of times.
     *
     * @param number Number of flashes (0->until turned off)
     * @param speed Flashing period in ms
     * @param intensity Custom HI intensity
     */
    void Flash(uint8_t number, uint16_t speed, uint8_t intensity);

    /**
     * @brief Turn the flashing off.
     */
    void FlashOff();

private:
    uint8_t id;
    uint8_t btnPin;

    RgbColor baseColor;
    RgbColor accentColor;

    uint8_t baseIntensity;
    uint8_t accentIntensity;

    RgbColor ledColorState;
    uint8_t ledIntensityState = 0;

    bool oldState = false;

    unsigned long lastChangeMillis = 0;
    void (*callback)(uint8_t i, KeyActions action);

    // LED stuff
    unsigned long lastFlash = 0;
    /**
     * Current flashing time in ms. 0 means flashing is off.
     */
    uint16_t flashingSpeed = 0;
    uint8_t flashIntensity, flashes, flashCount = 0;
};

#endif