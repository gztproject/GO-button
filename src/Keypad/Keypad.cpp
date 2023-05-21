#include "Keypad.h"

#include <NeoPixelBus.h>

namespace Keypad
{

    // #pragma region private defs

    unsigned long start;
    unsigned long lastMillis;
    unsigned long time;
    bool presetSelected;
    Button buttons[NUM_BUTTONS];
    Preset presets[NUM_PRESETS];
    uint8_t activePreset;
    BtnPreset btnPresets[NUM_BUTTONS];

    NeoPixelBus<NeoGrbFeature, NeoWs2812Method> strip(NUM_BUTTONS, LED_PIN);

    bool setKeyboardMode();
    bool setHidMode();
    bool setMidiMode();

    void keyboardAction(uint8_t key, KeyActions action);
    void hidAction(ConsumerKeycode key, KeyActions action);
    void midiAction(uint8_t key, KeyActions action);

    void midiNoteOn(byte channel, byte pitch, byte velocity);
    void midiNoteOff(byte channel, byte pitch, byte velocity);
    void midiControlChange(byte channel, byte control, byte value);

    // #pragma endregion private defs

    // #pragma region Public

    bool Init(Preset *pres)
    {
        for (uint8_t i = 0; i < NUM_PRESETS; i++)
        {
            presets[i] = pres[i];
        }
        activePreset = 0; // SETTINGS::GetActivePreset();

        Button buttons[NUM_BUTTONS] = {
            Button(0, BTN_0_PIN, BLACK, 0, PresetCallback),
            Button(1, BTN_1_PIN, BLACK, 0, PresetCallback),
            Button(2, BTN_2_PIN, BLACK, 0, PresetCallback),
            Button(3, BTN_3_PIN, BLACK, 0, PresetCallback),
            Button(4, BTN_4_PIN, BLACK, 0, PresetCallback)};

        presetSelected = false;
        // this resets all the neopixels to an off state
        strip.Begin();

        for (uint8_t i = 0; i < NUM_BUTTONS; i++)
        {
            buttons[i].Tick();
            Preset p = presets[i]; // SETTINGS::GetPreset(i);
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
        // while (!presetSelected && time - start < SETTINGS::GetPresetSelectTime() * 1000)
        while (!presetSelected && time - start < 3000)
        {
            time = millis();

            for (uint8_t i = 0; i < NUM_BUTTONS; i++)
            {
                buttons[i].Tick();
                strip.SetPixelColor(i, buttons[i].GetLedColorState());
            }

            if (time % 50 == 0 && lastMillis != time)
            {
                // Serial.println(time);
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
        activePreset = preset.GetId();
        presets[activePreset].GetButtons(btnPresets);

        for (uint8_t i = 0; i < NUM_BUTTONS; i++)
        {
            buttons[i].FlashOff();
            buttons[i].SetCallback(KeypadAction);
            buttons[i].SetBaseColor(btnPresets[i].baseColor, btnPresets[i].baseIntensity);
            buttons[i].SetAccentColor(btnPresets[i].accentColor, btnPresets[i].accentIntensity);
        }
        buttons[presets[activePreset].GetId()].Flash(5, 100);

        switch (presets[activePreset].GetMode())
        {
        case HwModes::KB_MODE:
            return setKeyboardMode();
            break;
        case HwModes::HID_MODE:
            return setHidMode();
            break;
        case HwModes::MIDI_MODE:
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

        for (uint8_t i = 0; i < 5; i++)
        {
            buttons[i].Tick();
            strip.SetPixelColor(i, buttons[i].GetLedColorState());
        }

        if (time % 50 == 0 && lastMillis != time)
        {
            // Serial.println(time);
            strip.Show();
            lastMillis = time;
        }
    }

    void PresetCallback(uint8_t btn, KeyActions action)
    {
        if (action == KeyActions::ON)
        {
            Preset p = presets[btn];//SETTINGS::GetPreset(btn);
            presetSelected = SelectPreset(p);
        }
    }

    void KeypadAction(uint8_t btn, KeyActions action)
    {
        uint16_t key = btnPresets[btn].key;

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

    // #pragma endregion Public

    // #pragma region Private

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
}

// #pragma endregion Private