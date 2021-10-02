#ifndef BUTTON_H
#define BUTTON_H

#include <stdio.h>
#include <Arduino.h>

#define LED_LOW_INT 20
#define LED_HIGH_INT 70

#define DEBOUNCE_TIME 50
#define MIN_PRESS 15

class Button
{
public:
    Button(uint8_t btn, uint8_t led, void (*callback)(void));
    void Tick();

private:
    uint8_t btnPin;
    uint8_t ledPin;
    bool btnPressed = false;
    bool btnReleased = true;
    uint32_t lastPressMillis = 0;
    void (*action)(void);
};

#endif