#ifndef _SERIAL_API_H
#define _SERIAL_API_H

#include <stdio.h>
#include <Arduino.h>

#include "config.h"
#include "globals.h"
#include "version.h"

namespace SerialAPI
{
    typedef enum _serialCommands : uint8_t
    {
        GetVersion = 0x10,
        GetDefaultPreset = 0x20,
        SetDefaultPreset = 0x21,
        GetPreset = 0x30,    
        SetPreset = 0x31,
    } SerialCommands;

    void Init();    
    void HandleSerial();
    

}
#endif