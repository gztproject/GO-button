/**
 * @file Keypad.h
 * @author Gašper Doljak (info@gzt.si)
 * @brief A Keypad "static" class (using namespace) that connects buttons and presets
 * @version 2.0.1
 * @date 2023-06-21
 *
 *
 */
#ifndef _KEYPAD_H
#define _KEYPAD_H

#include "config.h"
#include "Button.h"
#include "Preset.h"
#include <HID-Project.h>
#include <MIDIUSB.h>

#define LED_UPDATE_INTERVAL 50

/**
 * @brief A namespace "simulating" static class
 *
 */
namespace Keypad
{
    /**
     * @brief Set the LEDs, regardless of the keyboard state
     *
     * @param c0
     * @param c1
     * @param c2
     * @param c3
     * @param c4
     */
    void SetLeds(RgbColor c0, RgbColor c1, RgbColor c2, RgbColor c3, RgbColor c4);

    /**
     * @brief Set the selected LED, regardless of the keyboard state
     * 
     * @param c4
     */
    void SetLed(uint8_t led, RgbColor c);

    /**
     * @brief Set the LEDs, regardless of the keyboard state
     *
     * @param c
     */
    void SetAllLeds(RgbColor c);

    /**
     * @brief Initializes the keypad with presets and sets a default one
     *
     * @param pres A pointer to an array of presets
     * @param defaultId An ID of the preset to be selected if no user input
     * @param presetTimeout A timeout in ms after which a default preset is selected
     * @return bool - true if initialization was successful and preset selected.
     */
    bool Init(Preset *pres, uint8_t defaultId, uint16_t presetTimeout);

    /**
     * @brief Selects a preset
     *
     * @param preset Preset to select
     * @return bool true if successful
     */
    bool SelectPreset(Preset preset);

    /**
     * @brief A non-blocking task that does all the work - calls Button.Tick() and updates the colors every LED_UPDATE_INTERVAL ms.
     *
     */
    void Tick();

    /**
     * @brief A callback function that will select a preset using KEYPAD::SelectPreset().
     *
     * @param btn A button that was pressed
     * @param action Button action (ON/OFF)
     */
    void PresetCallback(uint8_t btn, KeyActions action);

    /**
     * @brief A callback function that will actually call the appropriate action (depending on the Preset KB mode) with a button defined in the preset.
     *
     * @param btn A button that was pressed
     * @param action Button action (ON/OFF)
     */
    void KeypadAction(uint8_t btn, KeyActions action);
}
#endif