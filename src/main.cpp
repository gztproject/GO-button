#include <Arduino.h>

#include "Keypad/Keypad.h"
#include "SerialAPI/SerialAPI.h"

unsigned long lastApiMillis = 0;

void setup()
{
    SerialAPI::Init();

    Keypad::Init();
}

void loop()
{
    Keypad::Tick();
    if (millis() - lastApiMillis > 500)
    {
        lastApiMillis = millis();
        SerialAPI::HandleSerial();
    }
}