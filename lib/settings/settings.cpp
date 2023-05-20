#include "settings.h"
#include <avr/eeprom.h>

namespace SETTINGS
{
    settingsContainer defaults;    
    SettingsState state = SettingsState::New;
    settingsContainer settings;
    uint16_t address;
    uint16_t calculateChecksum(settingsContainer settings);
    EEPROMpacket readPacket(void);
    bool writePacket(EEPROMpacket packet);
    void serializeByte(uint8_t code, uint8_t byte, bool print(const char *), bool await(void));
    void serializeWord(uint8_t code, uint16_t word, bool print(const char *), bool await(void));
    void serializeHexLine(uint8_t code, uint8_t *line, uint8_t length, bool print(const char *), bool await(void));

    uint8_t parseByte(char *data);
    uint16_t parseWord(char *data);
    bool parseArray(char *data, uint8_t *bytes, size_t len);

    size_t size;

    void formatHex(uint8_t byte, char *ch);
    void formatByte(uint8_t byte, char *ch);
    void formatWord(uint16_t dbl, char *ch);

    void init(uint16_t addr)
    {
        init(addr, defaults);
    }

    void init(uint16_t addr, settingsContainer def)
    {
        if (state != SettingsState::New && state != SettingsState::Failed)
            return;

        EEPROMpacket packet = {0};
        size = sizeof(packet);
        address = addr;
        packet = readPacket();
        settings = packet.settings;
        uint16_t checksum = calculateChecksum(settings);

        if (checksum == packet.checksum)
        {
            state = SettingsState::Retained;
            return;
        }

        settings = def;
        packet = {.settings = settings, .checksum = calculateChecksum(settings)};

        if (writePacket(packet))
        {
            state = SettingsState::Fresh;
        }
        else
        {
            state = SettingsState::Failed;
        }
    }

    // GETTERS
    settingsContainer getSettings()
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
        {
            return {0};
        }
        return settings;
    }

    uint16_t getEepromAddress(void) { return address; }

    SettingsState getState(void) { return state; }

    // MB+WRITE={"0":"10","1":"300","2":"0","32":"1","33":"0","34":"0123456789ABCDEF","35":"FEDCBA9876543210","36":"0123456789ABCDEFFEDCBA9876543210","37":"01234567","38":"0123456789ABCDEF0123456789ABCDEF","39":"FEDCBA98765432100123456789ABCDEF","48":"5D67114D","49":"5683","64":"1","65":"3","66":"240","67":"3","68":"300","69":"300"}
    bool SetFromString(char *pair)
    {
        char *tmp;
        char val[32];
        char *saveptr;
        unsigned int u;
        settingCode code;

        tmp = strtok_r(pair, ":", &saveptr);
        strncpy(val, &tmp[1], strlen(tmp) - 1);
        val[strlen(tmp) - 2] = '\0';
        sscanf(val, "%u", &u);
        code = (settingCode)u;

        tmp = strtok_r(NULL, ":", &saveptr);
        strncpy(val, &tmp[1], strlen(tmp) - 1);
        val[strlen(tmp) - 2] = '\0';

        uint8_t arr[24] = {0};
        switch (code)
        {
        default:
            break;
        }
        return false;
    }

    uint8_t parseByte(char *data)
    {
        unsigned int value = 0;
        sscanf(data, "%u", &value);
        return (uint8_t)value;
    }

    uint16_t parseWord(char *data)
    {
        unsigned int value = 0;
        sscanf(data, "%u", &value);
        return (uint16_t)value;
    }

    void printJson(bool print(const char *), bool await(void))
    {
        // print("{");
        // serializeWord(settingCode::autoInitTime, settings.autoInitTime, print, await);

        // print(",");
        // serializeWord(settingCode::measInterval, settings.measureInterval, print, await);

        // print("}");

        // MB+GET={"0":"10","1":"300","2":"0","32":"1","33":"0","34":"0123456789ABCDEF","35":"FEDCBA9876543210","36":"0123456789ABCDEFFEDCBA9876543210","37":"01234567","38":"0123456789ABCDEF0123456789ABCDEF","39":"FEDCBA98765432100123456789ABCDEF","48":"5D67114D","49":"5683","64":"1","65":"3","66":"240","67":"3","68":"300","69":"300"}

        while (!await)
            ;
    }

    // Private methods
    uint16_t calculateChecksum(settingsContainer data)
    {

        uint16_t crc = 0; // starting value as you like, must be the same before each calculation
        const char *p = (const char *)&data;
        for (uint16_t i = 0; i < sizeof(data); i++) // for each byte in data
        {
            crc += *p++; // update the crc value
        }
        return ~crc;
    }

    EEPROMpacket readPacket(void)
    {
        EEPROMpacket packet = {0};
#if defined(__AVR_AVR128DA64__)
        ReadEepromBlock(address, &packet, size);
#else
        eeprom_read_block(&packet, &address, size);
#endif
        return packet;
    }

    bool writePacket(EEPROMpacket packet)
    {
        char *p = reinterpret_cast<char *>(&packet);
#if defined(__AVR_AVR128DA64__)
        if (WriteEepromBlock(address, p, size) != nvmctrl_status_t::NVM_OK)
            return false;
#else
        eeprom_write_block(p, &address, size);
#endif
        return true;
    }

    void serializeByte(uint8_t code, uint8_t byte, bool print(const char *), bool await(void))
    {
        while (!await())
            ;
        char idx[4];
        formatByte(code, idx);
        print("\"");
        print(idx);
        print("\":\"");
        formatByte(byte, idx);
        print(idx);
        print("\"");
    }

    void serializeWord(uint8_t code, uint16_t word, bool print(const char *), bool await(void))
    {
        while (!await())
            ;
        char idx[6];
        formatByte(code, idx);
        print("\"");
        print(idx);
        print("\":\"");
        formatWord(word, idx);
        print(idx);
        print("\"");
    }

    void serializeHexLine(uint8_t code, uint8_t *line, uint8_t length, bool print(const char *), bool await(void))
    {
        while (!await())
            ;
        char idx[4];
        formatByte(code, idx);
        print("\"");
        print(idx);
        print("\":\"");
        for (uint8_t i = 0; i < length; i++)
        {
            formatHex(line[i], idx);
            print(idx);
        }
        print("\"");
    }

} // end of namespace SETTINGS