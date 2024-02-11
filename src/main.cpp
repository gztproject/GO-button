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
    if (millis() - lastApiMillis > 1000)
    {
        lastApiMillis = millis();
        SerialAPI::HandleSerial();
    }
}