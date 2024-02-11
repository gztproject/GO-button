#####################
Codelists
#####################

Enumerators
===================

.. _keypad_modes:

Keypad modes
-------------

=========== ===== ==========================================
Keypad mode Value Explanation
=========== ===== ==========================================
UNKNOWN     0x00  Mode is not set
KB_MODE     0x10  Keyboard mode
MIDI_MODE   0x20  MIDI mode
HID_MODE    0x30  HID mode (special keys and media control)
OFF_MODE    0xFF  Preset is inactive (off)
=========== ===== ==========================================


.. _key_functions:

Key functions
--------------

MIDI mode
""""""""""""""

HID mode
""""""""""""""

Power controls
~~~~~~~~~~~~~~~~~
==============================   =======
Function                         Code
==============================   =======
CONSUMER_POWER	                 0x0030
CONSUMER_SLEEP                   0x0032
CONSUMER_BRIGHTNESS_UP           0x006F
CONSUMER_BRIGHTNESS_DOWN         0x0070
CONSUMER_SCREENSAVER             0x019e
==============================   =======

Media controls
~~~~~~~~~~~~~~~~~
===================  =======
Function             Code
===================  =======
MEDIA_RECORD         0x00B2
MEDIA_FAST_FORWARD   0x00B3
MEDIA_REWIND         0x00B4
MEDIA_NEXT           0x00B5
MEDIA_PREVIOUS       0x00B6
MEDIA_STOP           0x00B7
MEDIA_PLAY_PAUSE     0x00CD
MEDIA_PAUSE          0x00B0
MEDIA_VOLUME_MUTE    0x00E2
MEDIA_VOLUME_UP      0x00E9
MEDIA_VOLUME_DOWN    0x00EA
===================  =======

Program launchers
~~~~~~~~~~~~~~~~~
==========================================   =======
Function                                     Code
==========================================   =======
CONSUMER_PROGRAMMABLE_BUTTON_CONFIGURATION   0x0182
CONSUMER_CONTROL_CONFIGURATION               0x0183
CONSUMER_EMAIL_READER	                     0x018A
CONSUMER_CALCULATOR	                         0x0192
CONSUMER_EXPLORER	                         0x0194
==========================================   =======

Browser
~~~~~~~~~~~~~~~~~
==============================   =======
Function                         Code
==============================   =======
CONSUMER_BROWSER_HOME	         0x0223
CONSUMER_BROWSER_BACK	         0x0224
CONSUMER_BROWSER_FORWARD	     0x0225
CONSUMER_BROWSER_REFRESH	     0x0227
CONSUMER_BROWSER_BOOKMARKS	     0x022A
==============================   =======

Other HID codes
~~~~~~~~~~~~~~~~~
There are about 360 more codes including OSC, custom keys, games, etc. A complete list is available on GitHub (ConsumerAPI_) or in USB specification_.


.. _ConsumerAPI: https://github.com/NicoHood/HID/blob/b16be57caef4295c6cd382a7e4c64db5073647f7/src/HID-APIs/ConsumerAPI.h#L70
.. _specification: https://usb.org/sites/default/files/hut1_2.pdf

KEYBOARD mode
"""""""""""""

Letters and numbers
~~~~~~~~~~~~~~~~~
===================  =======
Function             Code
===================  =======
KEY_A                0x0004
KEY_B                0x0005
KEY_C                0x0006
KEY_D                0x0007
KEY_E                0x0008
KEY_F                0x0009
KEY_G                0x0010
KEY_H                0x0011
KEY_I                0x0012
KEY_J                0x0013
KEY_K                0x0014
KEY_L                0x0015
KEY_M                0x0016
KEY_N                0x0017
KEY_O                0x0018
KEY_P                0x0019
KEY_Q                0x0020
KEY_R                0x0021
KEY_S                0x0022
KEY_T                0x0023
KEY_U                0x0024
KEY_V                0x0025
KEY_W                0x0026
KEY_X                0x0027
KEY_Y                0x0028
KEY_Z                0x0029
KEY_1                0x0030
KEY_2                0x0031
KEY_3                0x0032
KEY_4                0x0033
KEY_5                0x0034
KEY_6                0x0035
KEY_7                0x0036
KEY_8                0x0037
KEY_9                0x0038
KEY_0                0x0039
===================  =======

Numpad
~~~~~~~~~~~~~~~~~
===================  =======
Function             Code
===================  =======
KEYPAD_DIVIDE        0x0054
KEYPAD_MULTIPLY      0x0055
KEYPAD_SUBTRACT      0x0056
KEYPAD_ADD           0x0057
KEYPAD_ENTER         0x0058
KEYPAD_1             0x0059
KEYPAD_2             0x005A
KEYPAD_3             0x005B
KEYPAD_4             0x005C
KEYPAD_5             0x005D
KEYPAD_6             0x005E
KEYPAD_7             0x005F
KEYPAD_8             0x0060
KEYPAD_9             0x0061
KEYPAD_0             0x0062
KEYPAD_DOT           0x0063
===================  =======

Function/other keys
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
===================  =======
Function             Code
===================  ======= 
KEY_ENTER            0x0040
KEY_ESC              0x0041
KEY_BACKSPACE        0x0042
KEY_TAB              0x0043
KEY_SPACE            0x0044
KEY_MINUS            0x0045
KEY_EQUAL            0x0046
KEY_LEFT_BRACE       0x0047
KEY_RIGHT_BRACE      0x0048
KEY_BACKSLASH        0x0049
KEY_NON_US_NUM       0x0050
KEY_SEMICOLON        0x0051
KEY_QUOTE            0x0052
KEY_TILDE            0x0053
KEY_COMMA            0x0054
KEY_PERIOD           0x0055
KEY_SLASH            0x0056
KEY_CAPS_LOCK        0x0039
KEY_F1               0x003A
KEY_F2               0x003B
KEY_F3               0x003C
KEY_F4               0x003D
KEY_F5               0x003E
KEY_F6               0x003F
KEY_F7               0x0040
KEY_F8               0x0041
KEY_F9               0x0042
KEY_F10              0x0043
KEY_F11              0x0044
KEY_F12              0x0045
KEY_PRINTSCREEN      0x0046
KEY_SCROLL_LOCK      0x0047
KEY_PAUSE            0x0048
KEY_INSERT           0x0049
KEY_HOME             0x004A
KEY_PAGE_UP          0x004B
KEY_DELETE           0x004C
KEY_END              0x004D
KEY_PAGE_DOWN        0x004E
KEY_RIGHT_ARROW      0x004F
KEY_LEFT_ARROW       0x0050
KEY_DOWN_ARROW       0x0051
KEY_UP_ARROW         0x0052
KEY_NUM_LOCK         0x0053    
KEY_NON_US           0x0064    
KEY_MENU             0x0065
===================  ======= 

Complete list is available on GitHub: KeyLayouts_.

.. _KeyLayouts: https://github.com/NicoHood/HID/blob/b16be57caef4295c6cd382a7e4c64db5073647f7/src/KeyboardLayouts/ImprovedKeylayouts.h


Data structures
=================

.. _version_structure:

Version structure
-------------------
+----------+----------+---------+
| Byte(-s) |  Field   | Example |
+==========+==========+=========+
|    0     |  Major   |  0x02   |
+----------+----------+---------+
|    1     |  Minor   |  0x01   |
+----------+----------+---------+
|    2     | Revision |  0x00   |
+----------+----------+---------+


.. _preset_structure:

Preset structure
----------------------

+-------------+-------+-------------------------+--------------------------------------------------------+
| Byte(-s)    | Field | Example (HEX)           | Explanation                                            |
+=============+=======+=========================+========================================================+
|    0        |   id  |   00                    | Preset ID (should be the same as preset number)        |
+-------------+-------+-------------------------+--------------------------------------------------------+
|    1-16     |  name | 4d 49 44 49 00 00 00 00 | Preset name (16 chars,                                 |
|             |       |                         |                                                        |
|             |       | 00 00 00 00 00 00 00 00 | 0-padded to the right)                                 |
+-------------+-------+-------------------------+--------------------------------------------------------+
| 17          | mode  |   20                    | Preset mode (see :ref:`keypad_modes`)                  |
+-------------+-------+-------------------------+-------+------------------------------------------------+
| 18:19       | Key   |  00 2f                  | Key 0 | Function (see :ref:`key_functions`)            |
+-------------+-------+-------------------------+       +------------------------------------------------+
|       20:22 | Cb    |   00 00 FF              |       | Base color R G B                               |
+-------------+-------+-------------------------+       +------------------------------------------------+
|       23:25 | Ca    |   00 00 FF              |       | Accent color R G B                             |
+-------------+-------+-------------------------+       +------------------------------------------------+
|         26  | CbI   |   33                    |       | Base color intensity                           |
+-------------+-------+-------------------------+       +------------------------------------------------+
|         27  | CaI   |   AD                    |       | Accent color intensity                         |
+-------------+-------+-------------------------+-------+------------------------------------------------+
| 28:29       | Key   |  00 30                  | Key 1 | Function (see :ref:`key_functions`)            |
+-------------+-------+-------------------------+       +------------------------------------------------+
|       30:32 | Cb    |   00 00 FF              |       | Base color R G B                               |
+-------------+-------+-------------------------+       +------------------------------------------------+
|       33:35 | Ca    |   00 00 FF              |       | Accent color R G B                             |
+-------------+-------+-------------------------+       +------------------------------------------------+
|         36  | CbI   |   33                    |       | Base color intensity                           |
+-------------+-------+-------------------------+       +------------------------------------------------+
|         37  | CaI   |   AD                    |       | Accent color intensity                         |
+-------------+-------+-------------------------+-------+------------------------------------------------+
|       38:39 | Key   |  00 32                  | Key 2 | Function (see :ref:`key_functions`)            |
+-------------+-------+-------------------------+       +------------------------------------------------+
|       40:42 | Cb    |   FF FF 00              |       | Base color R G B                               |
+-------------+-------+-------------------------+       +------------------------------------------------+
|       43:45 | Ca    |   FF FF 00              |       | Accent color R G B                             |
+-------------+-------+-------------------------+       +------------------------------------------------+
|         46  | CbI   |   33                    |       | Base color intensity                           |
+-------------+-------+-------------------------+       +------------------------------------------------+
|         47  | CaI   |   AD                    |       | Accent color intensity                         |
+-------------+-------+-------------------------+-------+------------------------------------------------+
|        48:49| Key   |  00 34                  | Key 3 | Function (see :ref:`key_functions`)            |
+-------------+-------+-------------------------+       +------------------------------------------------+
|       50:52 | Cb    |   FF 00 00              |       | Base color R G B                               |
+-------------+-------+-------------------------+       +------------------------------------------------+
|       53:55 | Ca    |   FF 00 00              |       | Accent color R G B                             |
+-------------+-------+-------------------------+       +------------------------------------------------+
|         56  | CbI   |   33                    |       | Base color intensity                           |
+-------------+-------+-------------------------+       +------------------------------------------------+
|         57  | CaI   |   AD                    |       | Accent color intensity                         |
+-------------+-------+-------------------------+-------+------------------------------------------------+
|   58:59     | Key   |  00 35                  | Key 4 | Function (see :ref:`key_functions`)            |
+-------------+-------+-------------------------+       +------------------------------------------------+
|       60:62 | Cb    |   00 FF 00              |       | Base color R G B                               |
+-------------+-------+-------------------------+       +------------------------------------------------+
|       63:65 | Ca    |   00 FF 00              |       | Accent color R G B                             |
+-------------+-------+-------------------------+       +------------------------------------------------+
|         66  | CbI   |   33                    |       | Base color intensity                           |
+-------------+-------+-------------------------+       +------------------------------------------------+
|         67  | CaI   |   AD                    |       | Accent color intensity                         |
+-------------+-------+-------------------------+-------+------------------------------------------------+
|       68:70 | Cp    |   FF FF 00              |         Preset color R G B                             |
+-------------+-------+-------------------------+--------------------------------------------------------+
|       71    | CaI   |   AD                    |         Preset color intensity                         |
+-------------+-------+-------------------------+--------------------------------------------------------+