#######################
Developer documentation
#######################

Serial API documentation
========================

.. attention:: 
   Serial API is available from the version 2.1.0 on.

Serial port configuraton
------------------------

The device exposes a serial port (e.g. /dev/ttyACM0) that communicates at 115200 baud 8-N-1.

Serial API
----------
All communication is in HEX. Commands should be sent as HEX values and responses are returned the same.

.. error::
   Any other than specified responses represent an error and should be ignored. 
   Possible other responses include (but are not limited to):

   - UNKNOWN COMMAND
   - INVALID PRESET
   - ERROR

GetVersion (0x10)
""""""""""""""""""
This method returns the current FW version installed on the keypad in 3-byte HEX format (see :ref:`version_structure`).

Example
~~~~~~~

=========   ==========================    ==========================
Direction   Payload (HEX)                 Explanation
=========   ==========================    ==========================
Send        10                            GetVersion command
Receive     [:ref:`version_structure`]    Version number
=========   ==========================    ==========================

GetDefaultPreset (0x20)
""""""""""""""""""""""""
Returns the current default preset number.

Example
~~~~~~~

=========   =============  ==========================
Direction   Payload (HEX)  Explanation
=========   =============  ==========================
Send        20             GetDefaultPreset command
Receive     01             Default preset is 1
=========   =============  ==========================

SetDefaultPreset (0x21)
""""""""""""""""""""""""
Sets a new default preset. 
Returns the newly set default preset number.

Example
~~~~~~~

=========   =============  ==========================
Direction   Payload (HEX)  Explanation
=========   =============  ==========================
Send        21 00          Set default preset to 0
Receive     00             Default preset is set to 0
=========   =============  ==========================

GetPreset (0x30)
""""""""""""""""
Returns the desired preset data (see :ref:`preset_structure`).

Example
~~~~~~~

=========   ===============================  ==========================
Direction   Payload (HEX)                    Explanation
=========   ===============================  ==========================
Send        30 00                            Get preset 0 data
Receive     [:ref:`preset_structure`]        Preset 0 data (71b)
=========   ===============================  ==========================

SetPreset (0x31)
""""""""""""""""""
Sets the desired preset data to a new value.
Returns the new desired preset data (see :ref:`preset_structure`).

Example
~~~~~~~

=========   =====================================  ============================
Direction   Payload (HEX)                          Explanation
=========   =====================================  ============================
Send        31 00 [:ref:`preset_structure`]        Set preset 0 data (2b + 71b)
Receive     [:ref:`preset_structure`]              Preset 0 data (71b)
=========   =====================================  ============================