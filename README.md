![](https://github.com/tomertomism/custom-layout-wireless-keyboard/img/1.png)

#KeyBored, KeyBored in DFU, KeyBored in BLE

Left-side Knob is a must for me, cuz i think it is totally not efficient when I use the knob while only left hand is used.
(This project is still in development and may contain bugs and defects)

## 83-key Keyboard with Left-side knob design and Keychron-like layout.

Self-designed keyboard and firmware, using STM32F105RCT6 or L475RCT6 as the main MCU.
 - 1000Hz scan rate (L475), 250Hz (F105)
 - left-side knob design
 - WS2812 addressable LED

### - Controlboard

This keyboard uses seperate design to minimise my development cost.

| v0              | v1          |
| ----------------- | ---------------------- |
| using F105RCT6, no BLE  | using L475RCT6, BLE supported by ESP32-c3  |

The USB function development is done, thjis KeyBored is fully functional in USB connection.

 - BLE
 using ESP32C3FH4;
 ESP firmware is still under development, fimware upgrade feature will be available in the future.

### - DFU

You can upgrade the main MCU firmware through DFU by using [STM32CubeProg](https://www.st.com/en/development-tools/stm32cubeprog.html).
(I'm still working on the L475, cuz it doesn't work with F105's code).

```flow
st=>start: Power On
cond=>condition: "Enter" button pressed?
e=>end: Go main firmware
op=>end: Go DFU

st->cond
cond(no)->e
cond(yes)->op
```

### - Addressable LED

Only Static and Breathing presented, other effects will add later.
