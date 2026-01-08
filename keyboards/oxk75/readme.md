# oxk75

Custom modified 75% layout keyboard with custom matrix scanning, RGB matrix, knob, screen, and trimode connection.

Features:
* Custom Matrix scanning — Japanese duplex scanning algorithm
* RGB Matrix — Per-key RGB lighting, with an rgb led reserved for the caps indicator
* Knob (WIP) — 24 position knob with switch (used for mute)
* Screen (WIP) — 2.25 inch 76x284 SPI TFT bar LCD display using ST7789 driver
* Trimode Connection (WIP) — SPI to slave wireless MCU module.


* Keyboard Maintainer: [oxkDev](https://github.com/oxkDev)
* Hardware Supported: WeAct Blackpill (STM32F411CEU6) (possibly other MCUs with the same form factor)
* Hardware Availability: [Aliexpress Official WeAct Studio Store](https://www.aliexpress.com/item/1005001456186625.html)

Make example for this keyboard (after setting up your build environment):

    make oxk75:default

Flashing example for this keyboard:

    make oxk75:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the key at (0,0) in the matrix (usually the top left key or Escape) and plug in the keyboard
* **Physical reset button**: Hold the BOOT button on the Blackpill and briefly press the RESET button, then release the BOOT button after 0.5s.
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available
