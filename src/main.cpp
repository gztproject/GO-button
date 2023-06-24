#include <Arduino.h>

#include "Keypad/Keypad.h"

Preset presets[NUM_PRESETS];
void setPresets();

void setup()
{
    Serial.begin(115200);
    delay(5000);
    bool anySuccess = false;
    for (uint8_t i = 0; i < NUM_PRESETS; i++)
    {
        Preset p = Preset(i);
        if (!p.Recall())
        {
            Serial.print("Reading preset ");
            Serial.print(i);
            Serial.println(" from EEPROM faied.");
            anySuccess |= false;
            presets[i] = Preset(i);
            continue;
        }
        anySuccess |= true;
        presets[i] = p;
    }

    if (!anySuccess)
    {
        setPresets();
        for (uint8_t i = 0; i < NUM_PRESETS; i++)
        {
            Preset p = Preset(i);
            if (!p.Recall())
            {
                Serial.print("Rereading preset ");
                Serial.print(i);
                Serial.println(" from EEPROM faied.");
                anySuccess |= false;
                presets[i] = Preset(i);
                continue;
            }
            anySuccess |= true;
            presets[i] = p;
        }
        if (!anySuccess)
        {
            Serial.println("GENERAL ERROR!");
        }
    }
    Keypad::Init(presets, 0, 3000);
}

void loop()
{
    Keypad::Tick();
}

void setPresets()
{
    Serial.println("Setting presets from defaults");
    presets[0].SetColor(P0_COL, LED_HIGH_INT);
    presets[0].SetName(P0_NAME, 4);
    presets[0].SetMode(P0_MODE);
    BtnPreset btnPresets[NUM_BUTTONS];

    btnPresets[0] = {
        .key = P0B0_KEY,
        .baseColor = P0B0_COL,
        .accentColor = P0B0_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};
    btnPresets[1] = {
        .key = P0B1_KEY,
        .baseColor = P0B1_COL,
        .accentColor = P0B1_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};
    btnPresets[2] = {
        .key = P0B2_KEY,
        .baseColor = P0B2_COL,
        .accentColor = P0B2_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};
    btnPresets[3] = {
        .key = P0B3_KEY,
        .baseColor = P0B3_COL,
        .accentColor = P0B3_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};
    btnPresets[4] = {
        .key = P0B4_KEY,
        .baseColor = P0B4_COL,
        .accentColor = P0B4_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};

    presets[0].SetButtons(btnPresets);
    Serial.println("Saving preset 0");
    presets[0].Save();

    presets[1].SetColor(P1_COL, LED_HIGH_INT);
    presets[1].SetName(P1_NAME, 8);
    presets[1].SetMode(P1_MODE);

    btnPresets[0] = {
        .key = P1B0_KEY,
        .baseColor = P1B0_COL,
        .accentColor = P1B0_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};
    btnPresets[1] = {
        .key = P1B1_KEY,
        .baseColor = P1B1_COL,
        .accentColor = P1B1_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};
    btnPresets[2] = {
        .key = P1B2_KEY,
        .baseColor = P1B2_COL,
        .accentColor = P1B2_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};
    btnPresets[3] = {
        .key = P1B3_KEY,
        .baseColor = P1B3_COL,
        .accentColor = P1B3_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};
    btnPresets[4] = {
        .key = P1B4_KEY,
        .baseColor = P1B4_COL,
        .accentColor = P1B4_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};

    presets[1].SetButtons(btnPresets);
    Serial.println("Saving preset 1");
    presets[1].Save();

    presets[2].SetColor(P2_COL, LED_HIGH_INT);
    presets[2].SetName(P2_NAME, 3);
    presets[2].SetMode(P2_MODE);

    btnPresets[0] = {
        .key = P2B0_KEY,
        .baseColor = P2B0_COL,
        .accentColor = P2B0_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};
    btnPresets[1] = {
        .key = P2B1_KEY,
        .baseColor = P2B1_COL,
        .accentColor = P2B1_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};
    btnPresets[2] = {
        .key = P2B2_KEY,
        .baseColor = P2B2_COL,
        .accentColor = P2B2_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};
    btnPresets[3] = {
        .key = P2B3_KEY,
        .baseColor = P2B3_COL,
        .accentColor = P2B3_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};
    btnPresets[4] = {
        .key = P2B4_KEY,
        .baseColor = P2B4_COL,
        .accentColor = P2B4_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};

    presets[2].SetButtons(btnPresets);
    Serial.println("Saving preset 2");
    presets[2].Save();

    presets[3].SetColor(P3_COL, LED_HIGH_INT);
    presets[3].SetName(P3_NAME, 3);
    presets[3].SetMode(P3_MODE);

    btnPresets[0] = {
        .key = P3B0_KEY,
        .baseColor = P3B0_COL,
        .accentColor = P3B0_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};
    btnPresets[1] = {
        .key = P3B1_KEY,
        .baseColor = P3B1_COL,
        .accentColor = P3B1_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};
    btnPresets[2] = {
        .key = P3B2_KEY,
        .baseColor = P3B2_COL,
        .accentColor = P3B2_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};
    btnPresets[3] = {
        .key = P3B3_KEY,
        .baseColor = P3B3_COL,
        .accentColor = P3B3_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};
    btnPresets[4] = {
        .key = P3B4_KEY,
        .baseColor = P3B4_COL,
        .accentColor = P3B4_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};

    presets[3].SetButtons(btnPresets);
    Serial.println("Saving preset 3");
    presets[3].Save();

    presets[4].SetColor(P4_COL, LED_HIGH_INT);
    presets[4].SetName(P4_NAME, 4);
    presets[4].SetMode(P4_MODE);

    btnPresets[0] = {
        .key = P4B0_KEY,
        .baseColor = P4B0_COL,
        .accentColor = P4B0_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};
    btnPresets[1] = {
        .key = P4B1_KEY,
        .baseColor = P4B1_COL,
        .accentColor = P4B1_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};
    btnPresets[2] = {
        .key = P4B2_KEY,
        .baseColor = P4B2_COL,
        .accentColor = P4B2_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};
    btnPresets[3] = {
        .key = P4B3_KEY,
        .baseColor = P4B3_COL,
        .accentColor = P4B3_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};
    btnPresets[4] = {
        .key = P4B4_KEY,
        .baseColor = P4B4_COL,
        .accentColor = P4B4_COL,
        .baseIntensity = LED_LOW_INT,
        .accentIntensity = LED_HIGH_INT};

    presets[4].SetButtons(btnPresets);
    Serial.println("Saving preset 4");
    presets[4].Save();
}
