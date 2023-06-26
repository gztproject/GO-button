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
    uint8_t activePreset;
    BtnPreset btnPresets[NUM_BUTTONS];

    NeoPixelBus<NeoGrbFeature, NeoWs2812Method> strip(NUM_BUTTONS, LED_PIN);

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

    bool Init(Preset *pres, uint8_t defaultId, uint16_t presetTimeout)
    {
        for (uint8_t i = 0; i < NUM_PRESETS; i++)
        {
            presets[i] = pres[i];
        }
        activePreset = defaultId;

        buttons[0] = Button(0, BTN_0_PIN, BLACK, 0, PresetCallback);
        buttons[1] = Button(1, BTN_1_PIN, BLACK, 0, PresetCallback);
        buttons[2] = Button(2, BTN_2_PIN, BLACK, 0, PresetCallback);
        buttons[3] = Button(3, BTN_3_PIN, BLACK, 0, PresetCallback);
        buttons[4] = Button(4, BTN_4_PIN, BLACK, 0, PresetCallback);

        presetSelected = false;
        // this resets all the neopixels to an off state
        strip.Begin();

        for (uint8_t i = 0; i < NUM_BUTTONS; i++)
        {
            buttons[i].Tick();
            Preset p = presets[i];
            buttons[i].SetBaseColor(p.GetColor(), p.GetIntensity());
            buttons[i].SetAccentColor(p.GetColor(), p.GetIntensity());
            strip.SetPixelColor(i, buttons[i].GetLedColorState());
            buttons[i].Flash();
        }
        strip.Show();

        start = millis();
        lastMillis = millis();
        time = millis();

        // For the first 3 second
        while (!presetSelected && time - start < presetTimeout)
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

    bool SelectPreset(Preset preset)
    {
#if defined(KEYPAD_DEBUG) & KEYPAD_DEBUG > 0
        Serial.print("Setting preset ");
        char buf[16];
        preset.GetName(buf);
        Serial.print(buf);
        Serial.println(": ");
#endif

        activePreset = preset.GetId();
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
            Preset p = presets[btn];
            presetSelected = SelectPreset(p);
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
        default:
            break;
        }
    }

#pragma endregion Public

#pragma region Private

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
#pragma endregion Private
}
