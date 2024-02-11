###########
User manual
###########

Quick start guide
=================
There is really no special setup needed.

+---+---+
| 1 | 2 |
+---+---+
| 3 | 4 |
+---+---+
|   5   |
+-------+

Normal operation
---------------------
To start using the GO button plug it to the computer using a USB C cable. The keypad does not require any special drivers or setup.

Preset selection
""""""""""""""""""
After startup all 5 buttons will light up for 3 seconds. During this time you can select 1 of the 5 presets corresponding to each button.
Out of the box, the presets are as follows:

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

To reset the keypad to factory defaults, see :ref:`factory reset`



Default preset
---------------------
If during the 3 seconds after startup no button is pressed, the device will select the default preset. Out of the box, this is #1. 
To change the default preset, see :ref:`default_preset`


Operation modes
=================
There are 3 supported operation modes that can be selected using presets:

MIDI mode
---------------------
Channel: 0
Velocity: 127

Keyboard mode
---------------------
Simulates normal keyboard keystrokes (key up/down).

HID mode
---------------------
Simulates special keys, such as multimedia control or application shortcut keys.

The list of all supported codes is available in the `library repository <https://github.com/NicoHood/HID/blob/master/src/HID-APIs/ConsumerAPI.h>`_.

Presets
=================

.. _default_preset:

Default preset
-------------------

.. attention::
  This guide is for the latest FW revision (>= 2.1.0 2024-02-11). If behaviour on your device is different see :ref:`firmware_update`.

Out of the box, the default preset is set to be #1.

To change it, follow this steps:

#. Unplug the device

#. Press the button #5 while the device is unplugged

#. Plug in the device while holding the button

#. When all the buttons light up white, release the button

#. Wait for the buttons to change color to preset colors (3s)

#. Press the button to select it as the desired default preset

#. Buttons will flash and turn green to confirm the new selection (3s)

#. The device will resume the normal start up with the new default preset

.. _factory reset:

Factory reset
-------------------
.. attention::
  This guide is for the latest FW version (>= 2.1.0 2024-02-11). If behaviour on your device is different see :ref:`firmware_update`.

To factory reset the device, follow this steps:

#. Unplug the device

#. Press all 5 buttons at once while the device is unplugged

#. Plug in the device while holding the buttons

#. When all the buttons light up red, release the buttons

#. Wait for the device to reset (3s)

#. The device will resume the normal start up with factory defaults.

.. note::
  If the LEDs turn white at step 4 and then flash green after 3s, you have an older version of the firmware.

  This operation will perform both reset and set the new default preset tat the same time.
  
  In this case repeat the steps 1-4 but press and hold the desired default preset button immediately after releasing the buttons.
  Keep holding it until the LEDs flash green.