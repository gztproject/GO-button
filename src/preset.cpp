#include "preset.h"

#pragma region Public

Preset::Preset(uint8_t id)
{
    Preset::id = id;
    name[0] = '\0';
    mode = UNKNOWN;
    color = black;
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

bool Preset::SetActions(Action *actions)
{
    for (uint8_t i = 0; i < NUM_BUTTONS; i++)
    {
        Preset::actions[i] = actions[i];
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

size_t Preset::GetActions(Action *actions)
{
    size_t i;
    for (i = 0; i < NUM_BUTTONS; i++)
    {
        actions[i] = Preset::actions[i];
    }
    return i;
}

#pragma endregion Public
#pragma region Private

void Preset::clearActions()
{
    for (uint8_t i = 0; i < NUM_BUTTONS; i++)
    {
        actions[i] = emptyAction;
    }
}

#pragma endregion Private