#include "settings.h"

namespace SETTINGS
{
    SettingsState state;
    settingsContainer settings;
    uint16_t calculateChecksum(settingsContainer settings);
    bool writePacket(EEPROMpacket packet);

#pragma region GETTERS
    Preset GetActivePreset()
    {
        if(settings.useLastAsDefault)
            return settings.presets[settings.lastPreset];

        return settings.presets[settings.defaultPreset];
    }

    Preset GetPreset(uint8_t id)
    {
        return settings.presets[id];
    }

    uint8_t GetPresetSelectTime()
    {
        return settings.presetSelectTime;
    }

#pragma endregion GETTERS

#pragma region SETTERS
    bool SetUseLastAsDefault(bool ulad)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.useLastAsDefault == ulad)
            return false;

        settings.useLastAsDefault = ulad;
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }

    bool SetDefaultPreset(uint8_t id)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.defaultPreset == id)
            return false;

        settings.defaultPreset = id;
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }

    bool SetLastPreset(uint8_t id)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.lastPreset == id)
            return false;

        settings.lastPreset = id;
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }

    bool SetPresets(Preset *arr)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.presets == arr)
            return false;

        memcpy(settings.presets, &arr[0], sizeof(settings.presets));
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }
#pragma endregion SETTERS
}