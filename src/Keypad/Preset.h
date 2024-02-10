/**
 * @file Preset.h
 * @author Gašper Doljak (info@gzt.si)
 * @brief A Preset defines the behaviour of the keypad (mode, keys, colors...)
 * @version 2.0.1
 * @date 2023-06-21
 *
 *
 */
#ifndef _PRESET_H
#define _PRESET_H

#include <stdio.h>
#include <NeoPixelBus.h>
#include <EEPROM.h>

#include "config.h"

#define PRESET_NAME_SIZE 16
#define EEPROM_START_ADDRESS 0
//Id + Name + mode + 5 * (KeyH + KeyL + Rb + Gb + Bb + Ra + Ga + Ba + INTb + INTa) + R + G + B + INT
#define PRESET_EEPROM_LENGTH (1 + PRESET_NAME_SIZE + 1 + (NUM_BUTTONS * 10) + 4)

typedef enum _HwMode : uint8_t
{
    UNKNOWN = 00,
    KB_MODE = 10,
    MIDI_MODE = 20,
    HID_MODE = 30
} HwMode;

typedef struct _BtnPreset
{
    uint16_t key;
    RgbColor baseColor;
    RgbColor accentColor;
    uint8_t baseIntensity;
    uint8_t accentIntensity;
} BtnPreset;


#define RED RgbColor(255, 0, 0)
#define GREEN RgbColor(0, 255, 0)
#define BLUE RgbColor(0, 0, 255)
#define YELLOW RgbColor(255, 255, 0)
#define PURPLE RgbColor(255, 0, 255)
#define CYAN RgbColor(0, 255, 255)
#define WHITE RgbColor(255)
#define BLACK RgbColor(0)

class Preset
{
public:
    /**
     * @brief Construct a new Preset
     *
     */
    Preset() { Preset(0); };

    /**
     * @brief Construct a new Preset with ID
     *
     * @param _id ID of the preset. Should be sequential number between 0 - NUM_PRESETS
     */
    Preset(uint8_t _id);

    /**
     * @brief Construct a new Preset object
     *
     * @param _id ID of the preset. Should be sequential number between 0 - NUM_PRESETS
     * @param _mode Keyboard mode (KB/HID/MIDI)
     * @param btnPresets A pointer to an array of button presets @sa #btnPreset
     * @param _color An unscaled preset color
     * @param _intensity A preset color intensity
     */
    Preset(uint8_t _id, HwMode _mode, BtnPreset *btnPresets, RgbColor _color, uint8_t _intensity);

    /**
     * @brief Set the Name of the preset
     *
     * @param buf Character buffer
     * @param size Length of the name - should be smaller than PRESET_NAME_SIZE
     * @return bool - False if name is too long
     */
    bool SetName(const char *buf, size_t size);

    /**
     * @brief Set the KB mode of the preset
     *
     * @param _mode Keyboard mode (KB/HID/MIDI)
     */
    void SetMode(HwMode _mode);

    /**
     * @brief Set the Buttons object
     *
     * @param _btnPresets An array of BtnPreset objects. Size should be NUM_BTNS.
     * @return true
     */
    bool SetButtons(BtnPreset *_btnPresets);

    /**
     * @brief Set the Color object
     *
     * @param _color Unscaled preset color
     * @param _intensity Preset LED intensity
     */
    void SetColor(RgbColor _color, uint8_t _intensity);

    /**
     * @brief Get the Id object
     *
     * @return uint8_t
     */
    uint8_t GetId() { return id; };

    /**
     * @brief Get the Name object
     *
     * @param buf Char buffer to write to. Should be minimally PRESET_NAME_SIZE big.
     * @return size_t
     */
    size_t GetName(char *buf);

    /**
     * @brief Get the Mode object
     *
     * @return HwMode KB mode of the preset
     */
    HwMode GetMode() { return mode; };

    /**
     * @brief Get the Buttons object
     *
     * @param _btnPresets Buffer array to write to. Should be NUM_BTNS big.
     * @return size_t
     */
    size_t GetButtons(BtnPreset *_btnPresets);

    /**
     * @brief Get the Color object
     *
     * @return RgbColor
     */
    RgbColor GetColor() { return color; };

    /**
     * @brief Get the Intensity object
     *
     * @return uint8_t
     */
    uint8_t GetIntensity() { return intensity; };

    /**
     * @brief Saves the current preset to EEPROM. Make sure ID is set before calling this.
     * 
     * @return bool Operation was successful      
     */
    bool Save();

     /**
     * @brief Recalls the current preset from EEPROM. Make sure ID is set before calling this.
     * 
     * @return bool Operation was successful      
     */
    bool Recall();

    size_t Serialize( byte* buffer );
    bool Deserialize( byte* buffer , size_t size);

private:
    uint8_t id;
    char name[PRESET_NAME_SIZE];
    HwMode mode;
    BtnPreset btnPresets[NUM_BUTTONS];

    RgbColor color;
    uint8_t intensity;

    BtnPreset emptyBtnPreset = {
        .key = 0x00,
        .baseColor = RgbColor(0),
        .accentColor = RgbColor(0),
        .baseIntensity = 0,
        .accentIntensity = 0};

    /**
     * @brief Clears all the button presets
     *
     */
    void clearActions();
};

#endif