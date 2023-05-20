#define NUM_BUTTONS 5

#define BTN_0_PIN 17  //PB0
#define BTN_1_PIN 23  //PF0
#define BTN_2_PIN 3   //PD0
#define BTN_3_PIN 4   //PD4
#define BTN_4_PIN 30  //PD5

#define LED_PIN 16  // PB2 - MOSI

#define BTN_0_COL blue
#define BTN_1_COL blue
#define BTN_2_COL yellow
#define BTN_3_COL red
#define BTN_4_COL green

#define BTN_0_CHAR KEY_UP_ARROW
#define BTN_1_CHAR KEY_DOWN_ARROW
#define BTN_2_CHAR 'P'
#define BTN_3_CHAR KEY_ESC
#define BTN_4_CHAR ' '

#define MIDI_CH 0
#define MIDI_VELOCITY 127

#define BTN_0_NOTE 47
#define BTN_1_NOTE 48
#define BTN_2_NOTE 50
#define BTN_3_NOTE 52
#define BTN_4_NOTE 53

#define BTN_0_HID MEDIA_VOL_DOWN
#define BTN_1_HID MEDIA_VOL_UP
#define BTN_2_HID MEDIA_PREVIOUS
#define BTN_3_HID MEDIA_NEXT
#define BTN_4_HID MEDIA_PLAY_PAUSE

#define DEFAULT_MODE SETTINGS::Modes::HID_MODE