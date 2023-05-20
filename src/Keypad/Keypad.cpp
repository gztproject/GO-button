#include "Keypad.h"

Keypad::Keypad() : strip(NUM_BUTTONS, LED_PIN)
{
    activePreset = SETTINGS::GetActivePreset();
    Button buttons[NUM_BUTTONS] = {
        Button(BTN_0_PIN, black, emptyCallback, emptyCallback),
        Button(BTN_1_PIN, black, emptyCallback, emptyCallback),
        Button(BTN_2_PIN, black, emptyCallback, emptyCallback),
        Button(BTN_3_PIN, black, emptyCallback, emptyCallback),
        Button(BTN_4_PIN, black, emptyCallback, emptyCallback)};
}