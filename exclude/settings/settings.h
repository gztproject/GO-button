#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <string.h>
#include <stdio.h>
#include "config.h"

#include "Keypad/preset.h"
// namespace simulira "static class"
namespace SETTINGS
{

    typedef enum _SettingsState : uint8_t
    {
        New = 0x00,
        Fresh = 0x01,
        Retained = 0x02,
        Failed = 0x03
    } SettingsState;

    typedef enum _settingCode : uint8_t
    {
        useLastAsDefault,
        defaultPreset,
        lastPreset,
        presets,
        presetSelectTime,
    } settingCode;

    struct settingsContainer
    {
        bool useLastAsDefault;
        uint8_t defaultPreset;
        uint8_t lastPreset;
        Preset presets[NUM_PRESETS];
        uint8_t presetSelectTime;        
    };

    struct EEPROMpacket
    {
        settingsContainer settings;
        uint16_t checksum;
    };

    void init(uint16_t addr);
    void init(uint16_t addr, settingsContainer def);

    SettingsState getState(void);
    uint16_t getEepromAddress(void);

    settingsContainer getSettings(void);

// #pragma region GETTERS
    Preset GetActivePreset();
    Preset GetPreset(uint8_t id);
    uint8_t GetPresetSelectTime();
// #pragma endregion GETTERS

// #pragma region SETTERS
    bool SetDefaultPreset(uint8_t id);
    bool SetPresetSelectTime(uint8_t time);
// #pragma endregion SETTERS

    /**
     * Prints a JSON array of settings to UART.
     * Structure:
     *  {
     *      "code":"value",
     *      "0":"1",
     *      "1":"2",
     *      "2":"3"
     *  }
     *
     */
    void printJson(bool print(const char *), bool await(void));

    /**
     * Try to parse and set the setting from JSON pair ("code":"value").
     *
     * Returns true if successful.
     * Returns false if parsing wasn't successful or setting code isn't valid.
     */
    bool SetFromString(char *pair);

} // end of namespace SETTINGS

#endif
