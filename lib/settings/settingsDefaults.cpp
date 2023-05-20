#include "settings.h"

namespace SETTINGS
{
    /*****************************************
     ** DEFAULT SETTINGS
     *****************************************
     */
    settingsContainer defaults =
        {
            .useLastAsDefault = false,
            .defaultPreset = 0,
            .lastPreset = 0,
            .presets = {Preset(0), Preset(1), Preset(2), Preset(3), Preset(4)},
            .presetSelectTime = 3
        };
}