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

typedef struct _BtnPreset
{
    uint16_t key;
    RgbColor baseColor;
    RgbColor accentColor;
    uint8_t baseIntensity;
    uint8_t accentIntensity;
} BtnPreset;

// RgbColor red(255, 0, 0);
// RgbColor green(0, 255, 0);
// RgbColor blue(0, 0, 255);
// RgbColor yellow(255, 255, 0);
// RgbColor purple(255, 0, 255);
// RgbColor cyan(0, 255, 255);
// RgbColor white(255);
// RgbColor black(0);

#define RED RgbColor(255,0,0)
#define GREEN RgbColor (0,255,0)
#define BLUE RgbColor (0,0,255)
#define YELLOW RgbColor (255,255,0)
#define PURPLE RgbColor (255,0,255)
#define CYAN RgbColor (0,255,255)
#define WHITE RgbColor (255)
#define BLACK RgbColor (0)


class Preset
{
public:
    Preset() { Preset(0); };
    Preset(uint8_t id);
    Preset(uint8_t id, HwModes mode, BtnPreset *btnPresets, RgbColor color, uint8_t intensity);
    //bool SetName(char *buf, size_t size);
    void SetMode(HwModes mode);
    bool SetButtons(BtnPreset *btnPresets);
    void SetColor(RgbColor color, uint8_t intensity);
    uint8_t GetId() { return id; };
    //size_t GetName(char *buf);
    HwModes GetMode() { return mode; };
    size_t GetButtons(BtnPreset *btnPresets);
    RgbColor GetColor() { return color; };
    uint8_t GetIntensity() { return intensity; };

private:
    uint8_t id;
    char name[PRESET_NAME_SIZE];
    HwModes mode;
    BtnPreset btnPreset[NUM_BUTTONS];

    RgbColor color;
    uint8_t intensity;

    BtnPreset emptyBtnPreset = {
        .key = 0x00,
        .baseColor = BLACK,
        .accentColor = BLACK,
        .baseIntensity = 0,
        .accentIntensity = 0};

    void clearActions();
};

#endif