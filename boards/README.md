# Custom Embedded Boards

PlatformIO has pre-built settings for many popular embedded boards. The list of these boards is available as a web page at PlatformIO Boards Explorer or through the CLI command pio boards.

Custom boards can also be defined from scratch or by overriding settings of existing boards. All data is declared using the JSON syntax via associative array name/value pairs.

## Contents

* [Sources](#sources)
* [Board definition](#board-definition)
	* [JSON Structure](#json-structure)
	* [Installation](#installation)
	* [Examples](#examples)
	* [File locations](#file-locations)
* [Custom Variant](#custom-variant)

## Sources {#sources}

* [https://docs.platformio.org/en/stable/platforms/creating_board.html](https://docs.platformio.org/en/stable/projectconf/sections/platformio/options/directory/boards_dir.html#projectconf-pio-boards-dir)
* [https://docs.platformio.org/en/stable/projectconf/sections/platformio/options/directory/boards_dir.html#projectconf-pio-boards-dir](https://docs.platformio.org/en/stable/projectconf/sections/platformio/options/directory/boards_dir.html#projectconf-pio-boards-dir)
* [https://github.com/platformio/platform-atmelsam/pull/74](https://docs.platformio.org/en/stable/projectconf/sections/platformio/options/directory/boards_dir.html#projectconf-pio-boards-dir)


## Board definition {#board-definition}

### JSON Structure {#json-structure}

The key fields are:

* build data is handed over to the Development Platforms and Frameworks builders
* frameworks is the list with supported Frameworks. Each working environment for each project that uses the board will choose one of the frameworks declared here.
* platform name of Development Platforms
* upload upload settings which depend on the platform

For details, see existing boards as examples, available under `.platformio/platforms/*/boards/.`

``` json
{
  "build": {
    "extra_flags": "-DHELLO_PLATFORMIO",
    "f_cpu": "16000000L",
    "hwids": [
      [
        "0x1234",
        "0x0013"
      ],
      [
        "0x4567",
        "0x0013"
      ]
    ],
    "mcu": "%MCU_TYPE_HERE%"
  },
  "frameworks": ["%LIST_WITH_SUPPORTED_FRAMEWORKS%"],
  "platforms": ["%LIST_WITH_COMPATIBLE_PLATFORMS%"],
  "name": "My Test Board",
  "upload": {
    "maximum_ram_size": 2048,
    "maximum_size": 32256
  },
  "url": "http://example.com",
  "vendor": "MyCompany"
}
```

### Installation {#installation}

* Create boards directory in core_dir if it doesn’t exist.
* Create `myboard.json` file in this boards directory.
* Search available boards via pio boards command. You should see myboard board.

Now, you can use myboard for the board option in `platformio.ini` (Project Configuration File).

#### Note

You can have custom boards per project. In this case, please put your board’s JSON files to [boards_dir](#file-locations).

### Examples {#examples}

Please take a look at the source code of PlatformIO Development Platforms and navigate to boards folder of the repository.

### File locations {#file-locations}

The location of project-specific board definitions. Each project may choose a suitable directory name. The default value is boards, meaning a “boards” directory located in the root of the project.

By default, PlatformIO looks for boards in this order:

* Project boards_dir (as defined by this setting)
* Global `core_dir/boards`
* Development platform `core_dir/platforms/*/boards.`

This option can also be configured by the global environment variable PLATFORMIO_BOARDS_DIR.

## Custom Variant {#custom-variant}

In case a board needs to override the `arduino-pins.h` file, it should be put in a folder with the variant name.
In this case it is configured to look for the custom variant in a subfolder of the `boards` directory:


``` ini
;platformio.ini

[env:gztgobutton]
...
board_build.variant = customVariantName
board_build.variants_dir = boards
...
```
See [GitHub](https://github.com/platformio/platform-atmelsam/pull/74):

How about if we add support for board_build.variants_dir? Then you can create in your project custom_variants and later use in platformio.ini:

``` ini
;platformio.ini

[env:myenv]
...
board_build.variant = feather0custom
board_build.variants_dir = custom_variants
...
```

It will automatically find custom_variants in a project root. A relative path is OK.