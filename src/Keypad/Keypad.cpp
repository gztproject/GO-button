/**
 * @file Keypad.cpp
 * @author Gašper Doljak (info@gzt.si)
 * @brief A Keypad "static class"
 * @version 2.0.1
 * @date 2023-06-21
 *
 *
 */
#include "Keypad.h"
#include <NeoPixelBus.h>

namespace Keypad
{
#pragma region private defs

    unsigned long start;
    unsigned long lastMillis;
    unsigned long time;
    bool presetSelected;
    Button buttons[NUM_BUTTONS];
    Preset presets[NUM_PRESETS];
    Preset EmptyPreset = Preset();
    uint8_t defaultPreset;
    uint8_t activePreset;
    BtnPreset btnPresets[NUM_BUTTONS];

    NeoPixelBus<NeoGrbFeature, NeoWs2812Method> strip(NUM_BUTTONS, LED_PIN);

    /**
     * @brief Reads the presets from EEPROMand loads them in RAM
     *
     * @return true
     * @return false
     */
    bool readPresets();

    /**
     * @brief Resets all the presets to "factory defaults" - defined in config.h
     *
     */
    void resetPresets();

    /**
     * @brief Checks the keypad if any of the predefined special key combos are pressed and take the appropriate action
     *
     */
    void checkChords();

    /**
     * @brief Sets the Default Preset from user input from the keypad.
     *
     */
    void readDefaultPreset();

    /**
     * @brief Set the LEDs, regardless of the keyboard state
     *
     * @param c0
     * @param c1
     * @param c2
     * @param c3
     * @param c4
     */
    void SetLeds(RgbColor c0, RgbColor c1, RgbColor c2, RgbColor c3, RgbColor c4);

    /**
     * @brief Set the selected LED, regardless of the keyboard state
     *
     * @param c4
     */
    void SetLed(uint8_t led, RgbColor c);

    /**
     * @brief Set the LEDs, regardless of the keyboard state
     *
     * @param c
     */
    void SetAllLeds(RgbColor c);

    /**
     * @brief Set keypad mode to KB
     *
     * @return true
     */
    bool setKeyboardMode();

    /**
     * @brief Set keypad mode to HID
     *
     * @return true
     */
    bool setHidMode();

    /**
     * @brief Set keypad mode to MIDI
     *
     * @return true
     */
    bool setMidiMode();

    /**
     * @brief Set keypad mode to Off
     *
     * @return true
     */
    bool setOffMode();

    /**
     * @brief Sends the keystroke to the host
     *
     * @param key A key to send
     * @param action ON/OFF/PRESS
     */
    void keyboardAction(uint8_t key, KeyActions action);

    /**
     * @brief Sends the HID keystroke to the host
     *
     * @param key A key to send
     * @param action ON/OFF/PRESS
     */
    void hidAction(ConsumerKeycode key, KeyActions action);

    /**
     * @brief Calls the MIDI wrapper to send the note
     *
     * @param key A note to send
     * @param action ON/OFF
     */
    void midiAction(uint8_t key, KeyActions action);

    /**
     * @brief Actually sends the _noteOn_
     *
     * @param channel MIDI channel
     * @param pitch MIDI note (pitch)
     * @param velocity MIDI velocity
     */
    void midiNoteOn(byte channel, byte pitch, byte velocity);

    /**
     * @brief Actually sends the _noteOff_
     *
     * @param channel MIDI channel
     * @param pitch MIDI note (pitch)
     * @param velocity MIDI velocity
     */
    void midiNoteOff(byte channel, byte pitch, byte velocity);

    /**
     * @brief Actually sends the _controlChange_
     *
     * @param channel MIDI channel
     * @param control MIDI control
     * @param value MIDI control value
     */
    void midiControlChange(byte channel, byte control, byte value);

#pragma endregion private defs

#pragma region Public

    bool Init()
    {
        // Setup buttons
        buttons[0] = Button(0, BTN_0_PIN, BLACK, 0, PresetCallback);
        buttons[1] = Button(1, BTN_1_PIN, BLACK, 0, PresetCallback);
        buttons[2] = Button(2, BTN_2_PIN, BLACK, 0, PresetCallback);
        buttons[3] = Button(3, BTN_3_PIN, BLACK, 0, PresetCallback);
        buttons[4] = Button(4, BTN_4_PIN, BLACK, 0, PresetCallback);

        // Read presets from EEPROM
        if (!readPresets())
        {
            resetPresets();
        }

        // Read the default preset from EEPROM and set relate vars to known states
        defaultPreset = EEPROM.read(EEPROM_START_ADDRESS + (NUM_PRESETS * PRESET_EEPROM_LENGTH) + 1);
        activePreset = defaultPreset;
        presetSelected = false;

        // this resets all the neopixels to an off state
        strip.Begin();

        checkChords();

        for (uint8_t i = 0; i < NUM_BUTTONS; i++)
        {
            buttons[i].Tick();
            Preset p = presets[i];
            if (p.GetMode() != OFF_MODE && p.GetMode() != UNKNOWN)
            {
                buttons[i].SetBaseColor(p.GetColor(), p.GetIntensity());
                buttons[i].SetAccentColor(p.GetColor(), p.GetIntensity());
                strip.SetPixelColor(i, buttons[i].GetLedColorState());
                buttons[i].Flash();
            }
        }
        strip.Show();

        start = millis();
        lastMillis = millis();
        time = millis();

        // For the first 3 second
        while (!presetSelected && time - start < DEFAULT_TIMEOUT)
        {
            time = millis();

            for (uint8_t i = 0; i < NUM_BUTTONS; i++)
            {
                buttons[i].Tick();
            }

            if (time % LED_UPDATE_INTERVAL == 0 && lastMillis != time)
            {
                for (uint8_t i = 0; i < NUM_BUTTONS; i++)
                {
                    strip.SetPixelColor(i, buttons[i].GetLedColorState());
                }
                strip.Show();
                lastMillis = time;
            }
        }

        if (!presetSelected)
        {
            presetSelected = SelectPreset(activePreset);
        }

        for (uint8_t i = 0; i < NUM_BUTTONS; i++)
        {
            buttons[i].LedDimm();
            strip.SetPixelColor(i, buttons[i].GetLedColorState());
        }

        strip.Show();

        return presetSelected;
    }

    bool SetDefaultPreset(uint8_t preset)
    {
        if (preset >= NUM_PRESETS)
            return false;

        if (presets[preset].GetMode() == OFF_MODE || presets[preset].GetMode() == UNKNOWN)
            return false;

        defaultPreset = preset;

        EEPROM.update((EEPROM_START_ADDRESS + (NUM_PRESETS * PRESET_EEPROM_LENGTH) + 1), defaultPreset);
        return true;
    }

    uint8_t GetDefaultPreset()
    {
        return defaultPreset;
    }

    Preset* GetPreset(uint8_t preset)
    {        
        if (preset > NUM_PRESETS)
            return &EmptyPreset;
        return &presets[preset];
    }

    void SetLeds(RgbColor c0, RgbColor c1, RgbColor c2, RgbColor c3, RgbColor c4)
    {
        strip.SetPixelColor(0, c0);
        strip.SetPixelColor(1, c1);
        strip.SetPixelColor(2, c2);
        strip.SetPixelColor(3, c3);
        strip.SetPixelColor(4, c4);
        strip.Show();
    }

    void SetAllLeds(RgbColor c)
    {
        SetLeds(c, c, c, c, c);
    }

    void SetLed(uint8_t led, RgbColor c)
    {
        strip.SetPixelColor(led, c);
        strip.Show();
    }

    bool SelectPreset(uint8_t preset)
    {
#if defined(KEYPAD_DEBUG) & KEYPAD_DEBUG > 0
        Serial.print("Setting preset ");        
        Serial.print(preset);
        Serial.println(": ");
#endif
        activePreset = preset;
        presets[activePreset].GetButtons(btnPresets);

        for (uint8_t i = 0; i < NUM_BUTTONS; i++)
        {
#if defined(KEYPAD_DEBUG) & KEYPAD_DEBUG > 0
            Serial.print("\tB");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(btnPresets[i].key);
#endif
            buttons[i].FlashOff();
            buttons[i].SetCallback(KeypadAction);
            buttons[i].SetBaseColor(btnPresets[i].baseColor, btnPresets[i].baseIntensity);
            buttons[i].SetAccentColor(btnPresets[i].accentColor, btnPresets[i].accentIntensity);
            buttons[i].LedBase();
        }

        buttons[activePreset].Flash(5, 100);

        switch (presets[activePreset].GetMode())
        {
        case HwMode::KB_MODE:
            return setKeyboardMode();
            break;
        case HwMode::HID_MODE:
            return setHidMode();
            break;
        case HwMode::MIDI_MODE:
            return setMidiMode();
            break;
        case HwMode::OFF_MODE:
            return setOffMode();
            break;
        default:
            return false;
            break;
        }
    }

    void Tick()
    {
        time = millis();

        for (uint8_t i = 0; i < NUM_BUTTONS; i++)
        {
            buttons[i].Tick();
        }

        if (time % LED_UPDATE_INTERVAL == 0 && lastMillis != time)
        {
            for (uint8_t i = 0; i < NUM_BUTTONS; i++)
            {
                strip.SetPixelColor(i, buttons[i].GetLedColorState());
            }
            strip.Show();
            lastMillis = time;
        }
    }

    void PresetCallback(uint8_t btn, KeyActions action)
    {
        if (action == KeyActions::ON)
        {
            presetSelected = SelectPreset(btn);
        }
    }

    void KeypadAction(uint8_t btn, KeyActions action)
    {
        uint16_t key = btnPresets[btn].key;

#if defined(KEYPAD_DEBUG) & KEYPAD_DEBUG > 0
        Serial.print("Callback: BTN");
        Serial.print(btn);
        Serial.print(": ");
        Serial.print(key);
        Serial.print(" -> ");
        Serial.println(action);
#endif

        switch (presets[activePreset].GetMode())
        {
        case KB_MODE:
            keyboardAction(key, action);
            break;
        case HID_MODE:
            hidAction(static_cast<ConsumerKeycode>(key), action);
            break;
        case MIDI_MODE:
            midiAction(key, action);
            break;
        case OFF_MODE:
            break;
        default:
            break;
        }
    }

#pragma endregion Public

#pragma region Private

    void checkChords()
    {
        uint8_t chord = (!digitalRead(BTN_4_PIN) << 4) | (!digitalRead(BTN_3_PIN) << 3) | (!digitalRead(BTN_2_PIN) << 2) | (!digitalRead(BTN_1_PIN) << 1) | (!digitalRead(BTN_0_PIN) << 0);

        switch (chord)
        {
        case KEY_CHORD_SET_DEFAULT:
            Keypad::SetAllLeds(RgbColor(255));
            delay(3000);
            readDefaultPreset();
            break;
        case KEY_CHORD_RESET:
#if defined(SERIAL_DEBUG) & SERIAL_DEBUG > 0
            Serial.println("RESETTING!");
#endif
            Keypad::SetAllLeds(RgbColor(255, 0, 0));
            resetPresets();
            delay(3000);
            break;
        default:
            break;
        }
    }

    bool setKeyboardMode()
    {
        Keyboard.begin();
        return true;
    }

    bool setHidMode()
    {
        Consumer.begin();
        return true;
    }

    bool setMidiMode()
    {
        return true;
    }

    bool setOffMode()
    {
        return true;
    }

    void keyboardAction(uint8_t key, KeyActions action)
    {
        switch (action)
        {
        case ON:
            Keyboard.press(key);
            break;
        case OFF:
            Keyboard.release(key);
            break;
        case PRESS:
            Keyboard.write(key);
            break;
        default:
            break;
        }
    }

    void hidAction(ConsumerKeycode key, KeyActions action)
    {
        switch (action)
        {
        case ON:
            Consumer.press(key);
            break;
        case OFF:
            Consumer.release(key);
            break;
        case PRESS:
            Consumer.write(key);
            break;
        default:
            break;
        }
    }
    void midiAction(uint8_t key, KeyActions action)
    {
        switch (action)
        {
        case ON:
            midiNoteOn(MIDI_CH, key, MIDI_VELOCITY);
            break;
        case OFF:
            midiNoteOff(MIDI_CH, key, MIDI_VELOCITY);
            break;
        default:
            break;
        }
    }

    void midiNoteOn(uint8_t channel, uint8_t pitch, uint8_t velocity)
    {
        if (MidiUSB.connectionAvailable() == 0)
        {
            midiEventPacket_t noteOn = {0x09, uint8_t(0x90 | channel), pitch, velocity};
            MidiUSB.sendMIDI(noteOn);
        }
        MidiUSB.flush();
    }

    void midiNoteOff(uint8_t channel, uint8_t pitch, uint8_t velocity)
    {
        if (MidiUSB.connectionAvailable() == 0)
        {
            midiEventPacket_t noteOff = {0x08, uint8_t(0x80 | channel), pitch, velocity};
            MidiUSB.sendMIDI(noteOff);
        }
        MidiUSB.flush();
    }

    void midiControlChange(uint8_t channel, uint8_t control, uint8_t value)
    {
        if (MidiUSB.connectionAvailable() == 0)
        {
            midiEventPacket_t event = {0x0B, uint8_t(0xB0 | channel), control, value};
            MidiUSB.sendMIDI(event);
        }
        MidiUSB.flush();
    }

    void readDefaultPreset()
    {
        for (uint8_t i = 0; i < NUM_PRESETS; i++)
        {
            SetLed(i, presets[i].GetColor());
        }

        bool selection = false;

        while (!selection)
        {
            uint8_t keys = 0 | (!digitalRead(BTN_4_PIN) << 4) | (!digitalRead(BTN_3_PIN) << 3) | (!digitalRead(BTN_2_PIN) << 2) | (!digitalRead(BTN_1_PIN) << 1) | (!digitalRead(BTN_0_PIN) << 0);
            if (keys > 0)
            {
                for (uint8_t i = 0; i < NUM_PRESETS; i++)
                {
                    if ((keys >> i) == 1)
                    {
                        selection = SetDefaultPreset(i);
                        break;
                    }
                }
            }
        }

        SetAllLeds(RgbColor(0));

        for (uint8_t i = 0; i < 5; i++)
        {
            delay(100);
            for (uint8_t i = 0; i < NUM_PRESETS; i++)
            {
                if (i == defaultPreset)
                    Keypad::SetLed(i, RgbColor(0));
                else
                    Keypad::SetLed(i, presets[i].GetColor());
            }
            delay(100);
            for (uint8_t i = 0; i < NUM_PRESETS; i++)
            {
                Keypad::SetLed(i, presets[i].GetColor());
            }
        }
        delay(100);
        Keypad::SetAllLeds(RgbColor(0, 255, 0));
        delay(3000);
    }

    bool readPresets()
    {
        bool success = false;
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
                success |= false;
                presets[i] = Preset(i);
                continue;
            }
            success |= true;
#if defined(SERIAL_DEBUG) & SERIAL_DEBUG > 0
            char buf[PRESET_NAME_SIZE];
            p.GetName(buf);
            Serial.print(buf);
            Serial.println(" -> OK.");
#endif
            presets[i] = p;
        }
        return success;
    }

    void resetPresets()
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

#if defined(SERIAL_DEBUG) & SERIAL_DEBUG > 0
        Serial.println("Saving default preset");
#endif
        EEPROM.update(EEPROM_START_ADDRESS + (NUM_PRESETS * PRESET_EEPROM_LENGTH) + 1, DEFAULT_PRESET);

        if (!readPresets())
        {
#if defined(SERIAL_DEBUG) & SERIAL_DEBUG > 0
            Serial.println("GENERAL ERROR!");
#endif
            return;
        }
    }
#pragma endregion Private
}
