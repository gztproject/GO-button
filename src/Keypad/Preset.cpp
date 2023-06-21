/**
 * @file Preset.cpp
 * @author Gašper Doljak (info@gzt.si)
 * @brief A Preset class that defines the keypad behaviour
 * @version 2.0.1
 * @date 2023-06-21
 * 
 * 
 */
#include "Preset.h"

#pragma region Public

Preset::Preset(uint8_t _id)
{
    id = _id;
    name[0] = '\0';
    mode = UNKNOWN;
    color = BLACK;
    intensity = 0;
    clearActions();
}

Preset::Preset(uint8_t _id, HwMode _mode, BtnPreset *btnPresets, RgbColor _color, uint8_t _intensity)
{
    id = _id;
    name[0] = '\0';
    mode = _mode;
    color = _color;
    intensity = _intensity;

    SetButtons(btnPresets);
}

bool Preset::SetName(char *buf, size_t size)
{
    if (size >= PRESET_NAME_SIZE)
        return false;

    uint8_t i;
    for (i = 0; i < size; i++)
    {
        name[i] = buf[i];
    }
    name[i] = '\0';
    
    return true;
}

void Preset::SetMode(HwMode _mode)
{
    mode = _mode;

    clearActions();
}

bool Preset::SetButtons(BtnPreset *btnPresets)
{
    for (uint8_t i = 0; i < NUM_BUTTONS; i++)
    {
        btnPreset[i] = btnPresets[i];
    }
    return true;
}

void Preset::SetColor(RgbColor _color, uint8_t _intensity)
{
    color = _color;
    intensity = _intensity;
}

size_t Preset::GetName(char *buf)
{
    size_t i;
    for (i = 0; i < PRESET_NAME_SIZE; i++)
    {
        buf[i] = name[i];
        if (name[i] == '\0')
            return i;
    }
    return i;
}

size_t Preset::GetButtons(BtnPreset *btnPresets)
{
    size_t i;
    for (i = 0; i < NUM_BUTTONS; i++)
    {
        btnPresets[i] = btnPreset[i];
    }
    return i;
}

#pragma endregion Public
#pragma region Private

void Preset::clearActions()
{
    for (uint8_t i = 0; i < NUM_BUTTONS; i++)
    {
        btnPreset[i] = emptyBtnPreset;
    }
}

#pragma endregion Private