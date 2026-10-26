# Latest version (v1.0.0)

- Please be reminded, a [J-Link Debugger](https://www.segger.com/products/debug-probes/j-link/) is needed to flash the MCU if you are not using any [DKs by nordic](https://www.nordicsemi.com/Products/Wireless/Bluetooth-Low-Energy/Development-hardware). You should have one before proceed.

  ## IDE environment

- First, download and open the [project folder](main_firmware(52840)/) using [VS Code](https://code.visualstudio.com/) with the [nRF Connect for VS Code](https://www.nordicsemi.com/Products/Development-tools/nRF-Connect-for-VS-Code) extension and installed SDK [NCS v3.2.2](https://github.com/nrfconnect/sdk-nrf/releases/tag/v3.2.2).

- Second, download the [board .dts folder](tomertomism/), and put in inside ( ./ncs/v3.2.2/zephyr/boards/ ).

- Third, add your own Build Configuration, e.g. optimisation level, and click "Generate and Build".

- Last, click "Flash".

  ## Firmware structure

- Firmware [entry](main_firmware(52840)/src/main.c#L39)

- Matrix scan [thread](main_firmware(52840)/src/kb_scan.c#L141)

- ARGB [thread](main_firmware(52840)/src/kb_led.c#L57)