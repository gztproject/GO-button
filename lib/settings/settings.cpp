// **************************************************************
// **** AlfaCen MainBoard, nastavitve
// ****
// **** Tu se nahahjo predvsem FW nastavitve (časi, timeouti, naslovi ipd.), ki se med reseti shranjujejo v EEPROM.
// **** Ob zagonu jih preberem v globalne spremenljivke in po potrebi posodobim v EEPROM.
// **** HW konstante so v config.h.
// ****
// **** Gašper Doljak, feb. 2021
// ****
// **** Alfa Proxima d.o.o.
// **************************************************************
#ifndef _SETTINGS_C
#define _SETTINGS_C

#include "settings.h"
#include "../stations.h"
#include "ui/formatters.h"
#include "../stringTools.h"

#if defined(__AVR_AVR128DA64__)
#include <avr/pgmspace.h>
#include "../util/protected_io.h"
#else
#include <avr/eeprom.h>
#endif

namespace SETTINGS
{
    /*****************************************
     ** DEFAULT SETTINGS
     *****************************************
     */
    settingsContainer defaults =
        {
            .autoInitTime = AUTO_INIT_TIME,
            .measureInterval = MEASURE_INTERVAL,

            .defaultCommMode = DEFAULT_COMM,
            .e78ProtocolVersion = E78_PROTOCOL_VERSION,
            .loraJoinMode = LORA_JOIN_MODE,
            .loraClass = LORA_CLASS,

            // LoRa OTAA
            /**
             * DEV_EUI:
             *
             * 8E D8 F6 ED XX YY ZZ ZZ
             *  |  |  |  |  |  |  |  |
             *  |  |  |  |  |  |  --------> Serijska številka naprave (00 01, 00 02, ...)
             *  |  |  |  |  |  -----------> Podverzija naprave (zaenkrat vedno 00)
             *  |  |  |  |  --------------> Tip naprave (Glej šifrant)
             *  --------------------------> Prefiks, vedno 8E D8 F6 ED (razlaga spodaj)
             */
            .devEUI = DEV_EUI,
            /**
             * APP_EUI:
             *
             * 24 91 A7 8E A0 00 XX YY
             *  |  |  |  |  |  |  |  |
             *  |  |  |  |  |  |  |  -----> ID aplikacije
             *  |  |  |  |  |  |  --------> Tip naprave (Glej šifrant)
             *  --------------------------> Prefiks, 24 91 A7 8E A0 00 (razlaga spodaj)
             */
            .appEUI = APP_EUI,
            .appKey = APP_KEY,

            // LoRa ABP
            .devAddr = DEV_ADDR,
            .netSKey = NWK_SKEY,
            .appSKey = APP_SKEY,

            // NB-IOT (Using CoAP port)
            .socketAddr = NB_SOCKET_ADDR,
            .socketPort = NB_SOCKET_PORT,

            // board timeouts
            .boardMaxResponseTime = 1,
            .boardMaxInitTime = 25 * 60, // HUM needs 20 minutes...
            .boardMaxMeasureTime = 25 * 60,
            .boardMaxReadingTime = 10,

            // Board driver
            .boardDriverMaxWarmupTime = 5 * 60,
            .boardDriverMaxMeasureTime = 20 * 60, // HUM needs 20 minutes...

            // RF driver
            .RFMaxInitTime = 120,
            .RFMaxSendingTime = 40};

#if defined(__AVR_AVR128DA64__)
    /** Datatype for flash address */
    typedef uint32_t flash_adr_t;

    /** Datatype for EEPROM address */
    typedef uint16_t eeprom_adr_t;

    /** Datatype for return status of NVMCTRL operations */
    typedef enum
    {
        NVM_OK = 0,    ///< NVMCTRL free, no write ongoing.
        NVM_ERROR = 1, ///< NVMCTRL operation retsulted in error
        NVM_BUSY = 2,  ///< NVMCTRL busy, write ongoing.
    } nvmctrl_status_t;

    void ReadEepromBlock(eeprom_adr_t eeprom_adr, EEPROMpacket *data, size_t size);

    nvmctrl_status_t WriteEepromBlock(eeprom_adr_t eeprom_adr, char *data, size_t size);

    bool IsEepromReady();
#else
#endif

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

    CommMode getDefaultCommMode(void) { return settings.defaultCommMode; }
    E78ProtocolVersion getE78ProtocolVersion(void) { return settings.e78ProtocolVersion; }
    LoRaJoin getLoRaJoinMode(void) { return settings.loraJoinMode; }
    LoRaClass getLoRaClass(void) { return settings.loraClass; }

    uint16_t getAutoInitTime(void) { return settings.autoInitTime; }

    uint16_t getMeasureIntervalTicks(void) { return settings.measureInterval * _SECONDS_TO_TICKS; }
    uint16_t getMeasureIntervalTime(void) { return settings.measureInterval; }

    uint16_t getBoardMaxResponseTicks(void) { return settings.boardMaxResponseTime * _SECONDS_TO_TICKS; } // 1s
    uint16_t getBoardMaxResponseTime(void) { return settings.boardMaxResponseTime; }

    uint16_t getBoardMaxInitTicks(void) { return settings.boardMaxInitTime * _SECONDS_TO_TICKS; } // 5s
    uint16_t getBoardMaxInitTime(void) { return settings.boardMaxInitTime; }

    uint16_t getBoardMaxMeasureTicks(void) { return settings.boardMaxMeasureTime * _SECONDS_TO_TICKS; } // 4min
    uint16_t getBoardMaxMeasureTime(void) { return settings.boardMaxMeasureTime; }

    uint16_t getBoardMaxReadingTicks(void) { return settings.boardMaxReadingTime * _SECONDS_TO_TICKS; } // 3s
    uint16_t getBoardMaxReadingTime(void) { return settings.boardMaxReadingTime; }

    uint16_t getBoardDriverMaxWarmupTicks(void) { return settings.boardDriverMaxWarmupTime * _SECONDS_TO_TICKS; } // 5min
    uint16_t getBoardDriverMaxWarmupTime(void) { return settings.boardDriverMaxWarmupTime; }

    uint16_t getBoardDriverMaxMeasureTicks(void) { return (settings.boardDriverMaxMeasureTime + 300) * _SECONDS_TO_TICKS; } // 5min
    uint16_t getBoardDriverMaxMeasureTime(void) { return settings.boardDriverMaxMeasureTime + 300; }

    uint16_t getBoardDriverMaxInitTicks(void) { return (settings.boardMaxInitTime + 300) * _SECONDS_TO_TICKS; }
    uint16_t getBoardDriverMaxInitTime(void) { return (settings.boardMaxInitTime + 300); }

    uint16_t getBoardDriverMaxReadingTicks(void) { return (settings.boardMaxReadingTime + 300) * _SECONDS_TO_TICKS; }
    uint16_t getBoardDriverMaxReadingTime(void) { return (settings.boardMaxReadingTime + 300); }

    uint16_t getRFMaxInitTicks(void) { return settings.RFMaxInitTime * _SECONDS_TO_TICKS; }
    uint16_t getRFMaxInitTime(void) { return settings.RFMaxInitTime; }

    uint16_t getRFMaxSendingTicks(void) { return settings.RFMaxSendingTime * _SECONDS_TO_TICKS; }
    uint16_t getRFMaxSendingTime(void) { return settings.RFMaxSendingTime; }

    void getDevEUI(uint8_t *eui) { eui = settings.devEUI; }
    void getDevEUI(char *eui)
    {
        for (uint8_t i = 0; i < 8; i++)
        {
            char idx[3];
            Text::formatHex(settings.devEUI[i], idx);
            eui[i * 2] = idx[0];
            eui[(i * 2) + 1] = idx[1];
        }
        eui[16] = '\0';
    }

    void getAppEUI(uint8_t *eui) { eui = settings.appEUI; }
    void getAppEUI(char *eui)
    {
        for (uint8_t i = 0; i < 8; i++)
        {
            char idx[3];
            Text::formatHex(settings.appEUI[i], idx);
            eui[i * 2] = idx[0];
            eui[(i * 2) + 1] = idx[1];
        }
        eui[16] = '\0';
    }

    void getAppKey(uint8_t *key) { key = settings.appKey; }
    void getAppKey(char *key)
    {
        for (uint8_t i = 0; i < 16; i++)
        {
            char idx[3];
            Text::formatHex(settings.appKey[i], idx);
            key[i * 2] = idx[0];
            key[(i * 2) + 1] = idx[1];
        }
        key[32] = '\0';
    }

    void getDevAddr(uint8_t *addr) { addr = settings.devAddr; }
    void getDevAddr(char *addr)
    {
        for (uint8_t i = 0; i < 4; i++)
        {
            char idx[3];
            Text::formatHex(settings.devAddr[i], idx);
            addr[i * 2] = idx[0];
            addr[(i * 2) + 1] = idx[1];
        }
        addr[8] = '\0';
    }

    void getAppSKey(uint8_t *key) { key = settings.appSKey; }
    void getAppSKey(char *key)
    {
        for (uint8_t i = 0; i < 16; i++)
        {
            char idx[3];
            Text::formatHex(settings.appSKey[i], idx);
            key[i * 2] = idx[0];
            key[(i * 2) + 1] = idx[1];
        }
        key[32] = '\0';
    }

    void getNetSKey(uint8_t *key) { key = settings.netSKey; }
    void getNetSKey(char *key)
    {
        for (uint8_t i = 0; i < 16; i++)
        {
            char idx[3];
            Text::formatHex(settings.netSKey[i], idx);
            key[i * 2] = idx[0];
            key[(i * 2) + 1] = idx[1];
        }
        key[32] = '\0';
    }

    uint16_t getSocketPort() { return settings.socketPort; }

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
            // byte
        case settingCode::defaultCommMode:
            return setDefaultCommMode((CommMode)parseByte(val));
            break;
        case settingCode::loraJoinMode:
            return setLoRaJoinMode((LoRaJoin)parseByte(val));
            break;
        case settingCode::e78ProtocolVersion:
            return setE78ProtocolVersion((E78ProtocolVersion)parseByte(val));
            break;
        case settingCode::loraClass:
            return setLoRaClass((LoRaClass)parseByte(val));
            break;

            // word
        case settingCode::autoInitTime:
            return setAutoInitTime(parseWord(val));
            break;
        case settingCode::measInterval:
            return setMeasInterval(parseWord(val));
            break;
        case settingCode::socketPort:
            return setSocketPort(parseWord(val));
            break;
        case settingCode::boardMaxResponseTime:
            return setBoardMaxResponseTime(parseWord(val));
            break;
        case settingCode::boardMaxInitTime:
            return setBoardMaxInitTime(parseWord(val));
            break;
        case settingCode::boardMaxMeasureTime:
            return setBoardMaxMeasureTime(parseWord(val));
            break;
        case settingCode::boardMaxReadingTime:
            return setBoardMaxReadingTime(parseWord(val));
            break;
        case settingCode::boardDriverMaxWarmupTime:
            return setBoardDriverMaxWarmupTime(parseWord(val));
            break;
        case settingCode::boardDriverMaxMeasureTime:
            return setBoardDriverMaxMeasureTime(parseWord(val));
            break;
        case settingCode::rfMaxInitTime:
            return setRFMaxInitTime(parseWord(val));
            break;
        case settingCode::rfMaxSendingTime:
            return setRFMaxSendingTime(parseWord(val));
            break;

            // byte array
        case settingCode::devEUI:
            if (STRTOOLS::hexToByteArray(val, arr, sizeof(settings.devEUI)))
                return setDevEUI(arr);
            else
                return false;
            break;
        case settingCode::appEUI:
            if (STRTOOLS::hexToByteArray(val, arr, sizeof(settings.appEUI)))
                return setAppEUI(arr);
            else
                return false;
            break;
        case settingCode::appKey:
            if (STRTOOLS::hexToByteArray(val, arr, sizeof(settings.appKey)))
                return setAppKey(arr);
            else
                return false;
            break;
        case settingCode::devAddr:
            if (STRTOOLS::hexToByteArray(val, arr, sizeof(settings.devAddr)))
                return setDevAddr(arr);
            else
                return false;
            break;
        case settingCode::netSKey:
            if (STRTOOLS::hexToByteArray(val, arr, sizeof(settings.netSKey)))
                return setNetSKey(arr);
            else
                return false;
            break;
        case settingCode::appSKey:
            if (STRTOOLS::hexToByteArray(val, arr, sizeof(settings.appSKey)))
                return setAppSKey(arr);
            else
                return false;
            break;
        case settingCode::socketAddr:
            if (STRTOOLS::hexToByteArray(val, arr, sizeof(settings.socketAddr)))
                return setSocketAddr(arr);
            else
                return false;
            break;

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

    /*
        void printJson(bool print(const char *), bool await(void))
        {
            print("{");
            serializeWord(settingCode::autoInitTime, settings.autoInitTime, print, await);

            print(",");
            serializeWord(settingCode::measInterval, settings.measureInterval, print, await);

            print(",");
            serializeByte(settingCode::defaultCommMode, settings.defaultCommMode, print, await);

            print(",");
            serializeByte(settingCode::loraJoinMode, settings.loraJoinMode, print, await);
            
            print(",");
            serializeByte(settingCode::e78ProtocolVersion, settings.e78ProtocolVersion, print, await);

            print(",");
            serializeByte(settingCode::loraClass, settings.loraClass, print, await);

            print(",");
            serializeHexLine(settingCode::devEUI, settings.devEUI, 8, print, await);

            print(",");
            serializeHexLine(settingCode::appEUI, settings.appEUI, 8, print, await);

            print(",");
            serializeHexLine(settingCode::appKey, settings.appKey, 16, print, await);

            print(",");
            serializeHexLine(settingCode::devAddr, settings.devAddr, 4, print, await);

            print(",");
            serializeHexLine(settingCode::netSKey, settings.netSKey, 16, print, await);

            print(",");
            serializeHexLine(settingCode::appSKey, settings.appSKey, 16, print, await);

            print(",");
            serializeHexLine(settingCode::socketAddr, settings.socketAddr, 4, print, await);

            print(",");
            serializeWord(settingCode::socketPort, settings.socketPort, print, await);

            print(",");
            serializeWord(settingCode::boardMaxResponseTime, settings.boardMaxResponseTime, print, await);

            print(",");
            serializeWord(settingCode::boardMaxInitTime, settings.boardMaxInitTime, print, await);

            print(",");
            serializeWord(settingCode::boardMaxMeasureTime, settings.boardMaxMeasureTime, print, await);

            print(",");
            serializeWord(settingCode::boardMaxReadingTime, settings.boardMaxReadingTime, print, await);

            print(",");
            serializeWord(settingCode::boardDriverMaxWarmupTime, settings.boardDriverMaxWarmupTime, print, await);

            print(",");
            serializeWord(settingCode::boardDriverMaxMeasureTime, settings.boardDriverMaxMeasureTime, print, await);

            print(",");
            serializeWord(settingCode::rfMaxInitTime, settings.RFMaxInitTime, print, await);

            print(",");
            serializeWord(settingCode::rfMaxSendingTime, settings.RFMaxSendingTime, print, await);

            print("}");

            //MB+GET={"0":"10","1":"300","2":"0","32":"1","33":"0","34":"0123456789ABCDEF","35":"FEDCBA9876543210","36":"0123456789ABCDEFFEDCBA9876543210","37":"01234567","38":"0123456789ABCDEF0123456789ABCDEF","39":"FEDCBA98765432100123456789ABCDEF","48":"5D67114D","49":"5683","64":"1","65":"3","66":"240","67":"3","68":"300","69":"300"}

            while (!await)
                ;
        }
    */

    // SETTERS
    bool setDefaultCommMode(CommMode mode)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.defaultCommMode == mode)
            return false;

        settings.defaultCommMode = mode;
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }

    bool setLoRaJoinMode(LoRaJoin mode)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.loraJoinMode == mode)
            return false;

        settings.loraJoinMode = mode;
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }

    bool setE78ProtocolVersion(E78ProtocolVersion protocolVersion)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.e78ProtocolVersion == protocolVersion)
            return false;

        settings.e78ProtocolVersion = protocolVersion;
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }

    bool setLoRaClass(LoRaClass lcl)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.loraClass == lcl)
            return false;

        settings.loraClass = lcl;
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }

    bool setAutoInitTime(uint16_t time)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.autoInitTime == time)
            return false;

        settings.autoInitTime = time;
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }
    bool setMeasInterval(uint16_t time)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.measureInterval == time)
            return false;

        settings.measureInterval = time;
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }
    bool setBoardMaxResponseTime(uint16_t time)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.boardMaxResponseTime == time)
            return false;

        settings.boardMaxResponseTime = time;
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }
    bool setBoardMaxInitTime(uint16_t time)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.boardMaxInitTime == time)
            return false;

        settings.boardMaxInitTime = time;
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }
    bool setBoardMaxMeasureTime(uint16_t time)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.boardMaxMeasureTime == time)
            return false;

        settings.boardMaxMeasureTime = time;
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }
    bool setBoardMaxReadingTime(uint16_t time)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.boardMaxReadingTime == time)
            return false;

        settings.boardMaxReadingTime = time;
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }
    bool setBoardDriverMaxWarmupTime(uint16_t time)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.boardDriverMaxWarmupTime == time)
            return false;

        settings.boardDriverMaxWarmupTime = time;
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }
    bool setBoardDriverMaxMeasureTime(uint16_t time)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.boardDriverMaxMeasureTime == time)
            return false;

        settings.boardDriverMaxMeasureTime = time;
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }
    bool setRFMaxInitTime(uint16_t time)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.RFMaxInitTime == time)
            return false;

        settings.RFMaxInitTime = time;
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }
    bool setRFMaxSendingTime(uint16_t time)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.RFMaxSendingTime == time)
            return false;

        settings.RFMaxSendingTime = time;
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }
    bool setSocketPort(uint16_t port)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.socketPort == port)
            return false;

        settings.socketPort = port;
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }

    bool setDevEUI(uint8_t *arr)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.devEUI == arr)
            return false;

        memcpy(settings.devEUI, &arr[0], sizeof(settings.devEUI));
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }
    bool setAppEUI(uint8_t *arr)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.appEUI == arr)
            return false;

        memcpy(settings.appEUI, &arr[0], sizeof(settings.appEUI));
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }
    bool setAppKey(uint8_t *arr)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.appKey == arr)
            return false;

        memcpy(settings.appKey, &arr[0], sizeof(settings.appKey));
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }
    bool setDevAddr(uint8_t *arr)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.devAddr == arr)
            return false;

        memcpy(settings.devAddr, &arr[0], sizeof(settings.devAddr));
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }
    bool setNetSKey(uint8_t *arr)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.netSKey == arr)
            return false;

        memcpy(settings.netSKey, &arr[0], sizeof(settings.netSKey));
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }
    bool setAppSKey(uint8_t *arr)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.appSKey == arr)
            return false;

        memcpy(settings.appSKey, &arr[0], sizeof(settings.appSKey));
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
    }
    bool setSocketAddr(uint8_t *arr)
    {
        if (state != SettingsState::Fresh && state != SettingsState::Retained)
            return false;
        if (settings.socketAddr == arr)
            return false;

        memcpy(settings.socketAddr, &arr[0], sizeof(settings.socketAddr));
        EEPROMpacket packet = {.settings = settings, .checksum = calculateChecksum(settings)};
        return writePacket(packet);
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
        Text::formatByte(code, idx);
        print("\"");
        print(idx);
        print("\":\"");
        Text::formatByte(byte, idx);
        print(idx);
        print("\"");
    }

    void serializeWord(uint8_t code, uint16_t word, bool print(const char *), bool await(void))
    {
        while (!await())
            ;
        char idx[6];
        Text::formatByte(code, idx);
        print("\"");
        print(idx);
        print("\":\"");
        Text::formatWord(word, idx);
        print(idx);
        print("\"");
    }

    void serializeHexLine(uint8_t code, uint8_t *line, uint8_t length, bool print(const char *), bool await(void))
    {
        while (!await())
            ;
        char idx[4];
        Text::formatByte(code, idx);
        print("\"");
        print(idx);
        print("\":\"");
        for (uint8_t i = 0; i < length; i++)
        {
            Text::formatHex(line[i], idx);
            print(idx);
        }
        print("\"");
    }

#if defined(__AVR_AVR128DA64__)
    /**
     * \brief Read a block from eeprom
     *
     * \param[in] eeprom_adr The byte-address in eeprom to read from
     * \param[in] data Buffer to place read data into
     *
     * \return Nothing
     */
    void ReadEepromBlock(eeprom_adr_t eeprom_adr, EEPROMpacket *data, size_t size)
    {
        // Read operation will be stalled by hardware if any write is in progress
        memcpy(data, (uint8_t *)(EEPROM_START + eeprom_adr), size);
    }

    /**
     * \brief Write a block to eeprom
     *
     * \param[in] eeprom_adr The byte-address in eeprom to write to
     * \param[in] data The buffer to write
     *
     * \return Status of write operation
     */
    nvmctrl_status_t WriteEepromBlock(eeprom_adr_t eeprom_adr, char *data, size_t size)
    {
        char *write = (char *)(EEPROM_START + eeprom_adr);

        /* Wait for completion of previous write */
        while (NVMCTRL.STATUS & (NVMCTRL_EEBUSY_bm | NVMCTRL_FBUSY_bm))
            ;
        /* Program the EEPROM with desired value(s) */
        ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_EEERWR_gc);

        do
        {
            /* Write byte to EEPROM */
            *write++ = *data++;
            size--;
        } while (size != 0);

        /* Clear the current command */
        ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_NONE_gc);

        if (NVMCTRL.STATUS & NVMCTRL_ERROR_gm)
        {
            return NVM_ERROR;
        }
        else
        {
            return NVM_OK;
        }
    }

    /**
     * \brief Check if the EEPROM can accept data to be read or written
     *
     * \return The status of EEPROM busy check
     * \retval false The EEPROM can not receive data to be read or written
     * \retval true The EEPROM can receive data to be read or written
     */
    bool IsEepromReady()
    {
        if (NVMCTRL.STATUS & NVMCTRL_EEBUSY_bm)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
#endif

} // end of namespace SETTINGS
#endif