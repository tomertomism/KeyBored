# KeyBored in USB/BLE

I want my keyboard to have a knob at the left-side, so that I can use it with one hand only while I am in gaming. However, most keyboards on the market currently have it on the right side, which is kind of inconvenience.

我希望我的鍵盤的旋鈕是在左側，這樣玩遊戲時就能單手操作。但是市面上大多數鍵盤的旋鈕都在右側，這對我來說有點不方便。

## 83-key Keychron-like layout with Left-side knob.

<p align="center"><img src="./img/1.png" width="60%"></p>

### - Control board

#### --- Portal to [firmware](firmware/v1.0.0/)

I separate the keyboard into two parts, control board and matrix board, to minimise my cost while iterating the control board.

為了在迭代控制板時最大限度地降低成本，我將鍵盤分為控制板和矩陣板兩部分。

And because of separated design, you can use this control board and build your own matrix

由於採用了分離式設計，您可以使用此控制板建立自己的矩陣。


|     | v0 (deprecated) | v1.0.0 (latest)     |
| --- | --------------- | ------------------- |
| MCU | STM32F105RCT6   | NRF52840-QIAA       |
| USB | ✓ (250Hz)       | ✓  (1000Hz)         |
| BT  | ✗               | ✓  (100Hz)          |
| DFU | ✓               | still working...    |

### - Addressable LED

|Definition        |
| ---------------- |
|Sc - single colour|
|Sk - single key   |
|Fc - full colour  |
|Fm - full matrix  |

|                 |       | v0 (deprecated) | v1.0.0 (latest)     |
| --------------- | ----- | --------------- | ------------------- |
| Static          | Sc Fm | ✓               | still working...    |
| Breathing       | Sc Fm | ✓               | still working...    |
| Flowing Rainbow | Fc Fm | ✗               | ✓                   |
| Fade Out        | Sc Sk | ✗               | ✓                   |
