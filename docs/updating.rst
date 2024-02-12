.. _firmware_update:

#####################
Firmware updates
#####################

.. attention::
    If updating from a version <2.1.0, you should perform a :ref:`factory reset` after the update.

Web uploader
=============

.. attention::
    Online updater only supports Chrome browser at the moment.

The easiest way to update the device is using the online web updater tool_.



Process
----------

- Download the latest :download:`firmware.hex <_static/firmware-2.1.0.hex>` file.
- Plug the device to the computer
- Choose the firmware file using the webpage dialog
- Click the Reset! button and select the **GZT GO Button** from the list.
- Within 5s click the Upload! button and select the **Pro Micro 5v** from the list.


.. _tool: https://testbox.gzt.si/avr109/

Manual update
=============

Prerequisites
-------------

- avrdude_

.. _avrdude: https://github.com/avrdudes/avrdude

Process
----------

Linux
^^^^^^

- Download the latest :download:`firmware.hex <_static/firmware-2.1.0.hex>`  file.
- Plug the device to the computer
- Open the terminal and copy/type the following commands:
    - Reset the device in upload mode: ``stty -F /dev/ttyACM0 1200``
    - Upload the actual firmware: ``avrdude -p atmega32u4 -c avr109 -b 57600 -D -P /dev/ttyACM0 -U flash:w:/path/to/firmware.hex:i``

.. note::
    If the second command is unsuccessful you propably waited too long between the commands. After the first command the device will wait in upload mode for 5s and then restart back to normal operation.

Windows
^^^^^^^

Working on it. It should be pretty similar to the Linux process but exact commands may vary.


.. _GoButton: https://github.com/gztproject/GO-button/releases/latest