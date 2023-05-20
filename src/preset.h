#ifndef _PRESET_H
#define _PRESET_H

#include <stdio.h>
#include <NeoPixelBus.h>

#include "config.h"

#define PRESET_NAME_SIZE 128

typedef enum _HwModes : uint8_t
{
    UNKNOWN = 00,
    KB_MODE = 10,
    MIDI_MODE = 20,
    HID_MODE = 30
} HwModes;

typedef struct _Action
{
    void (*callbackOn)(void);
    void (*callbackOff)(void);
    RgbColor baseColor;
    RgbColor accentColor;
    uint8_t baseIntensity;
    uint8_t accentIntensity;
} Action;

RgbColor red(255, 0, 0);
RgbColor green(0, 255, 0);
RgbColor blue(0, 0, 255);
RgbColor yellow(255, 255, 0);
RgbColor purple(255, 0, 255);
RgbColor cyan(0, 255, 255);
RgbColor white(255);
RgbColor black(0);

void emptyCallback(){};

class Preset
{
public:
    Preset(uint8_t id);
    bool SetName(char *buf, size_t size);
    void SetMode(HwModes mode);
    bool SetActions(Action *actions);

    uint8_t GetId() { return id; };
    size_t GetName(char *buf);
    HwModes GetMode() { return mode; };
    size_t GetActions(Action *actions);

private:
    uint8_t id;
    char name[PRESET_NAME_SIZE];
    HwModes mode;
    Action actions[NUM_BUTTONS];

    Action emptyAction = {
        .callbackOn = emptyCallback,
        .callbackOff = emptyCallback,
        .baseColor = black,
        .accentColor = black,
        .baseIntensity = 0,
        .accentIntensity = 0};

    void clearActions();
};

#endif