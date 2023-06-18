#define NUM_BUTTONS 5
#define NUM_PRESETS NUM_BUTTONS

#define BTN_0_PIN 17  //PB0
#define BTN_1_PIN 23  //PF0
#define BTN_2_PIN 3   //PD0
#define BTN_3_PIN 4   //PD4
#define BTN_4_PIN 30  //PD5

#define LED_PIN 16  // PB2 - MOSI

#define LED_LOW_INT 51
#define LED_HIGH_INT 173

#define MIDI_CH 0
#define MIDI_VELOCITY 127

//Preset 0
#define P0_COL GREEN
#define P0_NAME "MIDI"
#define P0_MODE HwModes::MIDI_MODE
#define P0B0_KEY 47
#define P0B0_COL BLUE
#define P0B1_KEY 48
#define P0B1_COL BLUE
#define P0B2_KEY 50
#define P0B2_COL YELLOW
#define P0B3_KEY 52
#define P0B3_COL RED
#define P0B4_KEY 53
#define P0B4_COL GREEN


//Preset 1
#define P1_COL RED
#define P1_NAME "KEYBOARD"
#define P1_MODE HwModes::KB_MODE
#define P1B0_KEY KEY_UP_ARROW
#define P1B0_COL BLUE
#define P1B1_KEY KEY_DOWN_ARROW
#define P1B1_COL BLUE
#define P1B2_KEY 'P'
#define P1B2_COL YELLOW
#define P1B3_KEY KEY_ESC
#define P1B3_COL RED
#define P1B4_KEY ' '
#define P1B4_COL GREEN

//Preset 2
#define P2_COL PURPLE
#define P2_NAME "HID"
#define P2_MODE HwModes::HID_MODE
#define P2B0_KEY MEDIA_VOL_DOWN
#define P2B0_COL BLUE
#define P2B1_KEY MEDIA_VOL_UP
#define P2B1_COL BLUE
#define P2B2_KEY MEDIA_PREVIOUS
#define P2B2_COL GREEN
#define P2B3_KEY MEDIA_NEXT
#define P2B3_COL GREEN
#define P2B4_KEY MEDIA_PLAY_PAUSE
#define P2B4_COL RED

//Preset 3
#define P3_COL PURPLE
#define P3_NAME "HID"
#define P3_MODE HwModes::HID_MODE
#define P3B0_KEY MEDIA_VOL_DOWN
#define P3B0_COL BLUE
#define P3B1_KEY MEDIA_VOL_UP
#define P3B1_COL BLUE
#define P3B2_KEY MEDIA_PREVIOUS
#define P3B2_COL GREEN
#define P3B3_KEY MEDIA_NEXT
#define P3B3_COL GREEN
#define P3B4_KEY MEDIA_PLAY_PAUSE
#define P3B4_COL RED

//Preset 4
#define P4_COL PURPLE
#define P4_NAME "HID"
#define P4_MODE HwModes::HID_MODE
#define P4B0_KEY MEDIA_VOL_DOWN
#define P4B0_COL BLUE
#define P4B1_KEY MEDIA_VOL_UP
#define P4B1_COL BLUE
#define P4B2_KEY MEDIA_PREVIOUS
#define P4B2_COL GREEN
#define P4B3_KEY MEDIA_NEXT
#define P4B3_COL GREEN
#define P4B4_KEY MEDIA_PLAY_PAUSE
#define P4B4_COL RED

#define DEFAULT_MODE SETTINGS::Modes::HID_MODE