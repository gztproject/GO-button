// **************************************************************
// **** AlfaCen MainBoard, nastavitve
// ****
// **** Tu se nahahjo predvsem FW nastavitve (časi, timeouti, naslovi ipd.), ki se med reseti shranjujejo v EEPROM.
// **** Ob zagonu jih preberem v globalne spremenljivke in po potrebi posodobim v EEPROM.
// **** HW konstante so v config.h.
// ****
// **** Martin Trampuš, jan. 2021
// **** Gašper Doljak, jan. 2021
// ****
// **** Alfa Proxima d.o.o.
// **************************************************************
#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <string.h>
#include <stdio.h>
#include "config.h"

#include "preset.h"
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
        defaultMode = 0x00, // Mode 1 - 5

    } settingCode;

    struct settingsContainer
    {
        bool useLastAsDefault;
        Preset defaultPreset;        
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

    Preset GetActivePreset();

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

    // Setters
    bool SetDefaultPreset(Preset preset);

    /**
     * Try to parse and set the setting from JSON pair ("code":"value").
     *
     * Returns true if successful.
     * Returns false if parsing wasn't successful or setting code isn't valid.
     */
    bool SetFromString(char *pair);

} // end of namespace SETTINGS

#endif
