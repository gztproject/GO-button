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

#define KEY_CHORD_RESET 0x1F
#define KEY_CHORD_SET_DEFAULT 0x10

/**
 * @brief A namespace "simulating" static class
 *
 */
namespace Keypad
{

    /**
     * @brief Initializes the keypad with presets and sets a default one
     *
     * @return bool - true if initialization was successful and preset selected.
     */
    bool Init();

    /**
     * @brief Sets the default preset and saves it to EEPROM.
     *
     * @param preset
     * @return true
     * @return false
     */
    bool SetDefaultPreset(uint8_t preset);

    /**
     * @brief Get the Default Preset number
     *
     * @return uint8_t
     */
    uint8_t GetDefaultPreset();

    /**
     * @brief Get the pointer to the desired preset 
     * 
     * @param preset 
     * @return Preset 
     */
    Preset* GetPreset(uint8_t preset);

    /**
     * @brief Selects a preset
     *
     * @param preset Preset number to select
     * @return bool true if successful
     */
    bool SelectPreset(uint8_t preset);

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