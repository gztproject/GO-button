#include <Arduino.h>

#include "Keypad/Keypad.h"

Preset presets[NUM_PRESETS];
void setPresets();

void setup()
{
#if defined(SERIAL_DEBUG) & SERIAL_DEBUG > 0
    Serial.begin(115200);
    delay(5000);
#endif

    pinMode(BTN_0_PIN, INPUT_PULLUP);
    pinMode(BTN_1_PIN, INPUT_PULLUP);
    pinMode(BTN_2_PIN, INPUT_PULLUP);
    pinMode(BTN_3_PIN, INPUT_PULLUP);
    pinMode(BTN_4_PIN, INPUT_PULLUP);

    uint8_t def_preset = EEPROM.read(EEPROM_START_ADDRESS + (NUM_PRESETS * PRESET_EEPROM_LENGTH) + 1);

    if (!digitalRead(BTN_0_PIN) && !digitalRead(BTN_1_PIN) && !digitalRead(BTN_2_PIN) && !digitalRead(BTN_3_PIN) && !digitalRead(BTN_4_PIN))
    {
#if defined(SERIAL_DEBUG) & SERIAL_DEBUG > 0
        Serial.println("RESETTING!");
#endif
        Keypad::SetAllLeds(RgbColor(255));
        setPresets();

        delay(3000);

        Keypad::SetAllLeds(RgbColor(0));

        if (!digitalRead(BTN_0_PIN) && digitalRead(BTN_1_PIN) && digitalRead(BTN_2_PIN) && digitalRead(BTN_3_PIN) && digitalRead(BTN_4_PIN))
            def_preset = 0;
        else if (digitalRead(BTN_0_PIN) && !digitalRead(BTN_1_PIN) && digitalRead(BTN_2_PIN) && digitalRead(BTN_3_PIN) && digitalRead(BTN_4_PIN))
            def_preset = 1;
        else if (digitalRead(BTN_0_PIN) && digitalRead(BTN_1_PIN) && !digitalRead(BTN_2_PIN) && digitalRead(BTN_3_PIN) && digitalRead(BTN_4_PIN))
            def_preset = 2;
        else if (digitalRead(BTN_0_PIN) && digitalRead(BTN_1_PIN) && digitalRead(BTN_2_PIN) && !digitalRead(BTN_3_PIN) && digitalRead(BTN_4_PIN))
            def_preset = 3;
        else if (digitalRead(BTN_0_PIN) && digitalRead(BTN_1_PIN) && digitalRead(BTN_2_PIN) && digitalRead(BTN_3_PIN) && !digitalRead(BTN_4_PIN))
            def_preset = 4;

        EEPROM.update((EEPROM_START_ADDRESS + (NUM_PRESETS * PRESET_EEPROM_LENGTH) + 1), def_preset);
    }

    bool anySuccess = false;
    for (uint8_t i = 0; i < NUM_PRESETS; i++)
    {
        Preset p = Preset(i);
#if defined(SERIAL_DEBUG) & SERIAL_DEBUG > 0
        Serial.print("Reading preset ");
        Serial.println(i);
#endif
        if (!p.Recall())
        {
#if defined(SERIAL_DEBUG) & SERIAL_DEBUG > 0
            Serial.println("Reading from EEPROM faied.");
#endif
            anySuccess |= false;
            presets[i] = Preset(i);
            continue;
        }
        anySuccess |= true;
#if defined(SERIAL_DEBUG) & SERIAL_DEBUG > 0
        char buf[PRESET_NAME_SIZE];
        p.GetName(buf);
        Serial.print(buf);
        Serial.println(" -> OK.");
#endif
        presets[i] = p;
    }

    if (!anySuccess)
    {
        setPresets();
        for (uint8_t i = 0; i < NUM_PRESETS; i++)
        {
            Preset p = Preset(i);
#if defined(SERIAL_DEBUG) & SERIAL_DEBUG > 0
            Serial.print("Rereading preset ");
            Serial.print(i);
#endif
            if (!p.Recall())
            {
#if defined(SERIAL_DEBUG) & SERIAL_DEBUG > 0
                Serial.println("Rereading from EEPROM faied.");
#endif
                anySuccess |= false;
                presets[i] = Preset(i);
                continue;
            }
            anySuccess |= true;
#if defined(SERIAL_DEBUG) & SERIAL_DEBUG > 0
            char buf[PRESET_NAME_SIZE];
            p.GetName(buf);
            Serial.print(buf);
            Serial.println(" -> OK.");
#endif
            presets[i] = p;
        }
#if defined(SERIAL_DEBUG) & SERIAL_DEBUG > 0
        if (!anySuccess)
        {
            Serial.println("GENERAL ERROR!");
        }
#endif
    }

    Keypad::Init(presets, def_preset, 3000);
}

void loop()
{
    Keypad::Tick();
}

void setPresets()
{
    BtnPreset btnPresets[NUM_BUTTONS];

#if defined(SERIAL_DEBUG) & SERIAL_DEBUG > 0
    Serial.println("Setting presets from defaults");
#endif
    presets[0] = Preset(0);
    presets[0].SetColor(P0_COL, LED_HIGH_INT);
    presets[0].SetName(P0_NAME, 4);
    presets[0].SetMode(P0_MODE);

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
#if defined(SERIAL_DEBUG) & SERIAL_DEBUG > 0
    Serial.println("Saving preset 0");
#endif
    presets[0].Save();

    presets[1] = Preset(1);
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
#if defined(SERIAL_DEBUG) & SERIAL_DEBUG > 0
    Serial.println("Saving preset 1");
#endif
    presets[1].Save();

    presets[2] = Preset(2);
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
#if defined(SERIAL_DEBUG) & SERIAL_DEBUG > 0
    Serial.println("Saving preset 2");
#endif
    presets[2].Save();

    presets[3] = Preset(3);
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
#if defined(SERIAL_DEBUG) & SERIAL_DEBUG > 0
    Serial.println("Saving preset 3");
#endif
    presets[3].Save();

    presets[4] = Preset(4);
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
#if defined(SERIAL_DEBUG) & SERIAL_DEBUG > 0
    Serial.println("Saving preset 4");
#endif
    presets[4].Save();
}
