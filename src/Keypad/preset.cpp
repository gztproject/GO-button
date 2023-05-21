#include "preset.h"

// #pragma region Public

Preset::Preset(uint8_t id)
{
    Preset::id = id;
    name[0] = '\0';
    mode = UNKNOWN;
    color = BLACK;
    clearActions();
}

bool Preset::SetName(char *buf, size_t size)
{
    if (size > PRESET_NAME_SIZE)
        return false;

    for (uint8_t i = 0; i < size; i++)
    {
        name[i] = buf[i];
    }
    return true;
}

void Preset::SetMode(HwModes mode)
{
    Preset::mode = mode;

    clearActions();
}

bool Preset::SetButtons(BtnPreset *btnPresets)
{
    for (uint8_t i = 0; i < NUM_BUTTONS; i++)
    {
        Preset::btnPreset[i] = btnPreset[i];
    }
    return true;
}

void Preset::SetColor(RgbColor color, uint8_t intensity)
{
    Preset::color = color;
    Preset::intensity = intensity;
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

size_t Preset::GetButtons(BtnPreset *BtnPreset)
{
    size_t i;
    for (i = 0; i < NUM_BUTTONS; i++)
    {
        btnPreset[i] = Preset::btnPreset[i];
    }
    return i;
}

// #pragma endregion Public
// #pragma region Private

void Preset::clearActions()
{
    for (uint8_t i = 0; i < NUM_BUTTONS; i++)
    {
        btnPreset[i] = emptyBtnPreset;
    }
}

// #pragma endregion Private