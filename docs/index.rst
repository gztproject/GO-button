.. GZT-GO documentation master file, created by
   sphinx-quickstart on Mon Jan 22 09:24:08 2024.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

GZT-GO Button Documentation
==================================

.. toctree::
   :maxdepth: 2
   :caption: Contents:

Quick start guide
=================

Normal operation
---------------------
To start using the GO button plug it to the computer using a USB C cable. The keypad does not require any special drivers or setup.

Preset selection
""""""""""""""""""
After startup all 5 buttons will light up for 3 seconds. During this time you can select 1 of the 5 presets corresponding to each button:

+---+---+
| 1 | 2 |
+---+---+
| 3 | 4 |
+-------+
|   5   |
+-------+

Out of the box, the presets are as follows:

//Preset 0
#define P0_COL GREEN
#define P0_NAME "MIDI"
#define P0_MODE HwMode::MIDI_MODE
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
#define P1_MODE HwMode::KB_MODE
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
#define P2_MODE HwMode::HID_MODE
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

#. **MIDI mode** (channel: 0, velocity: 127)

   #. Note on/off 47 (Blue)
      
   #. Note on/off 48 (Blue)
      
   #. Note on/off 50 (Yellow)
      
   #. Note on/off 52 (Red)      

   #. Note on/off 53 (Green)            

#. **Keyboard mode**

   #. Up arrow (Blue)

   #. Down arrow (Blue)

   #. P (Yellow)

   #. ESC (Red)

   #. SPACE (Green)

#. **HID mode**

   #. Volume up (Blue)

   #. Volume down (Blue)

   #. Previous (<<) (Green)

   #. Next (>>) (Green)

   #. Play/Pause (Red)

#. **HID mode** Same as #3

#. **HID mode** Same as #3



Default preset
---------------------
If during the 3 seconds after startup no button is pressed, the device will select the default preset. Out of the box, this is #1. 
To change the default preset, see :ref:`default_preset`


Operation modes
=================

MIDI mode
---------------------

Keayboard mode
---------------------

HID mode
---------------------

Presets
=================

.. _default_preset:

Default preset
-------------------

Out of the box, the default preset is set to be #1.
To change it, follow this steps:

#. Unplug the device

#. Press all 5 buttons at once while the device is unplugged

#. Plug in the device while holding the buttons

#. When all the buttons light up white, release the buttons

#. Wait for the buttons to change color to blue (3s)

#. Press the button to select it as the desired default preset

#. Buttons will turn green and flash to confirm the new selection

#. The device will resume the normal start up with the new default preset

