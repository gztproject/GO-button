#include "SerialAPI.h"

namespace SerialAPI
{
#pragma region private function declarations
    void handleAPI(SerialCommands command, uint8_t argument, byte *payload, size_t size);

    void saveNewPresetFromSerial(uint8_t presetNumber, byte *presetData, size_t size);
    void dumpPresetOverSerial(uint8_t presetNumber);

    void setNewDefaultPreset(uint8_t presetNumber);

#pragma endregion private function declarations

#pragma region public functions
    void Init()
    {
        Serial.begin(115200);
    }

    void HandleSerial()
    {
        if (Serial.available() > 0)
        {
            byte buffer[SERIAL_RX_BUFFER_SIZE];
            size_t size = Serial.readBytesUntil('\n', buffer, SERIAL_RX_BUFFER_SIZE);

            byte command = buffer[0];
            byte arg = 0;
            byte payload[size];
            uint8_t i = 0;

            if (size > 1)
            {
                arg = buffer[1];
                if (size > 2)
                {
                    for (i = 0; i < size - 2; i++)
                    {
                        byte b = buffer[i + 2];
                        if (b == '\n' || b == '\r')
                        {
                            i -= 1;
                            break;
                        }
                        payload[i] = b;
                    }
                }
            }
            handleAPI((SerialCommands)command, arg, payload, i);
        }
    }

    void handleAPI(SerialCommands command, uint8_t argument, byte *payload, size_t size)
    {
        switch (command)
        {
        case SerialCommands::GetVersion:
            Serial.write(FW_version, 3);
            break;
        case SerialCommands::GetPreset:
            dumpPresetOverSerial(argument);
            break;
        case SerialCommands::SetPreset:
            saveNewPresetFromSerial(argument, payload, size);
            break;
        case SerialCommands::GetDefaultPreset:
            Serial.write(Keypad::GetDefaultPreset());
            break;
        case SerialCommands::SetDefaultPreset:
            setNewDefaultPreset(argument);
            break;
        default:
            Serial.println("UNKNOWN COMMAND");
            break;
        }
    }
#pragma endregion public functions

#pragma region private functions

    void dumpPresetOverSerial(uint8_t presetNumber)
    {
        byte buf[SERIAL_TX_BUFFER_SIZE];
        size_t size = Keypad::GetPreset(presetNumber)->Serialize(buf);

        Serial.write(buf, size);
    }

    void saveNewPresetFromSerial(uint8_t presetNumber, byte *presetData, size_t size)
    {
        if (Keypad::GetPreset(presetNumber)->Deserialize(presetData, size))
        {
            Keypad::GetPreset(presetNumber)->Save();
            Keypad::SelectPreset(presetNumber);
            dumpPresetOverSerial(presetNumber);
        }
        else
            Serial.println("ERROR");
    }

    void setNewDefaultPreset(uint8_t presetNumber)
    {
        if (presetNumber >= NUM_PRESETS)
        {
            Serial.println("INVALID PRESET");
            return;
        }
        Keypad::SetDefaultPreset(presetNumber);
        Serial.write(Keypad::GetDefaultPreset());
    }

#pragma endregion private functions
}