#include "Keypad.h"

#pragma region Public

Keypad::Keypad() : strip(NUM_BUTTONS, LED_PIN)
{
    activePreset = SETTINGS::GetActivePreset();

    Button buttons[NUM_BUTTONS] = {
        Button(BTN_0_PIN, black, 0, emptyCallback, emptyCallback),
        Button(BTN_1_PIN, black, 0, emptyCallback, emptyCallback),
        Button(BTN_2_PIN, black, 0, emptyCallback, emptyCallback),
        Button(BTN_3_PIN, black, 0, emptyCallback, emptyCallback),
        Button(BTN_4_PIN, black, 0, emptyCallback, emptyCallback)};

    presetSelected = false;
}

bool Keypad::Init()
{
    // this resets all the neopixels to an off state
    strip.Begin();

    for (uint8_t i = 0; i < NUM_BUTTONS; i++)
    {
        buttons[i].Tick();
        Preset p = SETTINGS::GetPreset(i);
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
    while (!presetSelected && time - start < SETTINGS::GetPresetSelectTime() * 1000)
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

bool Keypad::SelectPreset(Preset preset)
{
    activePreset = preset;

    Action actions[NUM_PRESETS];
    activePreset.GetActions(actions);

    for (uint8_t i = 0; i < NUM_BUTTONS; i++)
    {
        buttons[i].FlashOff();
        buttons[i].SetCallbacks(actions[i].callbackOn, actions[i].callbackOff);
        buttons[i].SetBaseColor(actions[i].baseColor, actions[i].baseIntensity);
        buttons[i].SetAccentColor(actions[i].accentColor, actions[i].accentIntensity);
    }
    buttons[activePreset.GetId()].Flash(5, 100);

    switch (activePreset.GetMode())
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

void Keypad::Tick()
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

#pragma endregion Public

#pragma region Private

bool Keypad::setKeyboardMode()
{
    Keyboard.begin();
    return true;
}

bool Keypad::setHidMode()
{
    Consumer.begin();
    return true;
}

bool Keypad::setMidiMode()
{
    return true;
}

void Keypad::midiNoteOn(uint8_t channel, uint8_t pitch, uint8_t velocity)
{
    if (MidiUSB.connectionAvailable() == 0)
    {
        midiEventPacket_t noteOn = {0x09, uint8_t(0x90 | channel), pitch, velocity};
        MidiUSB.sendMIDI(noteOn);
    }
    MidiUSB.flush();
}

void Keypad::midiNoteOff(uint8_t channel, uint8_t pitch, uint8_t velocity)
{
    if (MidiUSB.connectionAvailable() == 0)
    {
        midiEventPacket_t noteOff = {0x08, uint8_t(0x80 | channel), pitch, velocity};
        MidiUSB.sendMIDI(noteOff);
    }
    MidiUSB.flush();
}

void Keypad::midiControlChange(uint8_t channel, uint8_t control, uint8_t value)
{
    if (MidiUSB.connectionAvailable() == 0)
    {
        midiEventPacket_t event = {0x0B, uint8_t(0xB0 | channel), control, value};
        MidiUSB.sendMIDI(event);
    }
    MidiUSB.flush();
}

#pragma endregion Private