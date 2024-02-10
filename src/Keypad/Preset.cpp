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
    color = RgbColor(0);
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

bool Preset::SetName(const char *buf, size_t size)
{
    if (size >= PRESET_NAME_SIZE)
        return false;

    uint8_t i;
    for (i = 0; i < PRESET_NAME_SIZE; i++)
    {
        if (i < size)
            name[i] = buf[i];
        else
            name[i] = 0;
    }

    return true;
}

void Preset::SetMode(HwMode _mode)
{
    mode = _mode;

    clearActions();
}

bool Preset::SetButtons(BtnPreset *_btnPresets)
{
    for (uint8_t i = 0; i < NUM_BUTTONS; i++)
    {
        btnPresets[i] = _btnPresets[i];
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

size_t Preset::GetButtons(BtnPreset *_btnPresets)
{
    size_t i;
    for (i = 0; i < NUM_BUTTONS; i++)
    {
        _btnPresets[i] = btnPresets[i];
    }
    return i;
}

bool Preset::Save()
{
    int address = EEPROM_START_ADDRESS + (id * PRESET_EEPROM_LENGTH);

#if defined(PRESET_DEBUG) & PRESET_DEBUG > 0
    Serial.print("\tID: ");
    Serial.println(id);
    Serial.print("\tStarting W address: ");
    Serial.println(address);
#endif

    EEPROM.update(address++, id);
    for (uint8_t i = 0; i < PRESET_NAME_SIZE; i++)
    {
        EEPROM.update(address++, name[i]);
    }
    EEPROM.update(address++, mode);

    for (uint8_t i = 0; i < NUM_BUTTONS; i++)
    {
#if defined(PRESET_DEBUG) & PRESET_DEBUG > 0
        Serial.print("\t\tBTN ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(btnPresets[i].key);
#endif
        EEPROM.update(address++, highByte(btnPresets[i].key));
        EEPROM.update(address++, lowByte(btnPresets[i].key));
        EEPROM.update(address++, btnPresets[i].baseColor.R);
        EEPROM.update(address++, btnPresets[i].baseColor.G);
        EEPROM.update(address++, btnPresets[i].baseColor.B);
        EEPROM.update(address++, btnPresets[i].accentColor.R);
        EEPROM.update(address++, btnPresets[i].accentColor.G);
        EEPROM.update(address++, btnPresets[i].accentColor.B);
        EEPROM.update(address++, btnPresets[i].baseIntensity);
        EEPROM.update(address++, btnPresets[i].accentIntensity);
    }

    EEPROM.update(address++, color.R);
    EEPROM.update(address++, color.G);
    EEPROM.update(address++, color.B);
    EEPROM.update(address++, intensity);

#if defined(PRESET_DEBUG) & PRESET_DEBUG > 0
    Serial.print("\tLast W address: ");
    Serial.println(address - 1);

    Serial.print("\tW length: ");
    Serial.println((address - 1) - (EEPROM_START_ADDRESS + (id * PRESET_EEPROM_LENGTH)));
#endif
    return true;
}

bool Preset::Recall()
{
    uint16_t address = EEPROM_START_ADDRESS + (id * PRESET_EEPROM_LENGTH);
#if defined(PRESET_DEBUG) & PRESET_DEBUG > 0
    Serial.print("\tID: ");
    Serial.println(id);
    Serial.print("\tStarting R address: ");
    Serial.println(address);
#endif

    uint8_t _id = EEPROM.read(address++);

    if (_id != id)
    {
        // Using ID as control value
        return false;
    }

    id = _id;

    for (uint8_t i = 0; i < PRESET_NAME_SIZE; i++)
    {
        name[i] = EEPROM.read(address++);
    }
    mode = (HwMode)EEPROM.read(address++);

    for (uint8_t i = 0; i < NUM_BUTTONS; i++)
    {
        btnPresets[i].key = EEPROM.read(address++) << 8;
        btnPresets[i].key |= EEPROM.read(address++);
#if defined(PRESET_DEBUG) & PRESET_DEBUG > 0
        Serial.print("\t\tBTN ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(btnPresets[i].key);
#endif
        btnPresets[i].baseColor.R = EEPROM.read(address++);
        btnPresets[i].baseColor.G = EEPROM.read(address++);
        btnPresets[i].baseColor.B = EEPROM.read(address++);
        btnPresets[i].accentColor.R = EEPROM.read(address++);
        btnPresets[i].accentColor.G = EEPROM.read(address++);
        btnPresets[i].accentColor.B = EEPROM.read(address++);
        btnPresets[i].baseIntensity = EEPROM.read(address++);
        btnPresets[i].accentIntensity = EEPROM.read(address++);
    }

    color.R = EEPROM.read(address++);
    color.G = EEPROM.read(address++);
    color.B = EEPROM.read(address++);
    intensity = EEPROM.read(address++);

#if defined(PRESET_DEBUG) & PRESET_DEBUG > 0
    Serial.print("\tLast R address: ");
    Serial.println(address - 1);

    Serial.print("\tR length: ");
    Serial.println((address - 1) - (EEPROM_START_ADDRESS + (id * PRESET_EEPROM_LENGTH)));
#endif
    return true;
}

size_t Preset::Serialize(byte *buffer)
{
    uint8_t idx = 0;

    buffer[idx++] = id;

    for (uint8_t i = 0; i < PRESET_NAME_SIZE; i++)
    {
        buffer[idx++] = name[i];
    }
    buffer[idx++] = mode;

    for (uint8_t i = 0; i < NUM_BUTTONS; i++)
    {
        buffer[idx++] = highByte(btnPresets[i].key);
        buffer[idx++] = lowByte(btnPresets[i].key);
        buffer[idx++] = btnPresets[i].baseColor.R;
        buffer[idx++] = btnPresets[i].baseColor.G;
        buffer[idx++] = btnPresets[i].baseColor.B;
        buffer[idx++] = btnPresets[i].accentColor.R;
        buffer[idx++] = btnPresets[i].accentColor.G;
        buffer[idx++] = btnPresets[i].accentColor.B;
        buffer[idx++] = btnPresets[i].baseIntensity;
        buffer[idx++] = btnPresets[i].accentIntensity;
    }

    buffer[idx++] = color.R;
    buffer[idx++] = color.G;
    buffer[idx++] = color.B;
    buffer[idx++] = intensity;

    return idx;
}

bool Preset::Deserialize(byte *buffer, size_t size)
{
    if (size < PRESET_EEPROM_LENGTH)
        return false;

    size_t idx = 0;

    if (id != buffer[idx++])
        return false;

    for (uint8_t i = 0; i < PRESET_NAME_SIZE; i++)
    {
        name[i] = buffer[idx++];
    }
    mode = (HwMode)buffer[idx++];

    for (uint8_t i = 0; i < NUM_BUTTONS; i++)
    {
        btnPresets[i].key = buffer[idx++] << 8;
        btnPresets[i].key |= buffer[idx++];
#if defined(PRESET_DEBUG) & PRESET_DEBUG > 0
        Serial.print("\t\tBTN ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(btnPresets[i].key);
#endif
        btnPresets[i].baseColor.R = buffer[idx++];
        btnPresets[i].baseColor.G = buffer[idx++];
        btnPresets[i].baseColor.B = buffer[idx++];
        btnPresets[i].accentColor.R = buffer[idx++];
        btnPresets[i].accentColor.G = buffer[idx++];
        btnPresets[i].accentColor.B = buffer[idx++];
        btnPresets[i].baseIntensity = buffer[idx++];
        btnPresets[i].accentIntensity = buffer[idx++];
    }

    color.R = buffer[idx++];
    color.G = buffer[idx++];
    color.B = buffer[idx++];
    intensity = buffer[idx++];
    
    return true;
}

#pragma endregion Public
#pragma region Private

void Preset::clearActions()
{
    for (uint8_t i = 0; i < NUM_BUTTONS; i++)
    {
        btnPresets[i] = emptyBtnPreset;
    }
}

// From https://docs.arduino.cc/learn/programming/eeprom-guide#eeprom-iteration
// unsigned long Preset::eeprom_crc(void)
// {

//     const unsigned long crc_table[16] = {
//         0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
//         0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
//         0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
//         0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
//     };

//     unsigned long crc = ~0L;

//     for (int index = 0; index < EEPROM.length(); ++index)
//     {
//         crc = crc_table[(crc ^ EEPROM[index]) & 0x0f] ^ (crc >> 4);
//         crc = crc_table[(crc ^ (EEPROM[index] >> 4)) & 0x0f] ^ (crc >> 4);
//         crc = ~crc;
//     }

//     return crc;
// }

#pragma endregion Private