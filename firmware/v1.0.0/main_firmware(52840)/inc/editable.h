/* Copyright 2026 Nok Hin, Siu

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#pragma once

#include <stdint.h>

static const uint8_t hid_report_desc[] = {
  0x05, 0x01,                     //  Usage Page (Generic Desktop)
  0x09, 0x06,                     //  Usage (Keyboard)
  0xA1, 0x01,                     //  Collection (Application)

  0x85, 0x01,                     //  Report ID 1

  0x05, 0x07,                     //  Usage Page (Key Codes)
  0x19, 0xe0,                     //  Usage Minimum (Keyboard LeftControl)
  0x29, 0xe7,                     //  Usage Maximum (Keyboard Right GUI)
  0x15, 0x00,                     //  Logical Minimum (0)
  0x25, 0x01,                     //  Logical Maximum (1)
  0x75, 0x01,                     //  Report Size (1 bit)
  0x95, 0x08,                     //  Report Count (8 bits)
  0x81, 0x02,                     //  Input (Data, Variable, Absolute)

  0x05, 0x08,                     //  Usage Page (LEDs)
  0x19, 0x01,                     //  Usage Minimum (Num Lock)
  0x29, 0x05,                     //  Usage Maximum (Kana)
  0x75, 0x01,                     //  Report Size (1)
  0x95, 0x05,                     //  Report Count (5)
  0x91, 0x02,                     //  Output (Data, Variable, Absolute)
  0x75, 0x03,                     //  Report Size (3)
  0x95, 0x01,                     //  Report Count (1)
  0x91, 0x03,                     //  Output (Constant, Variable, Absolute)

  0x05, 0x07,                     //  Usage Page (Key codes)
  0x19, 0x00,                     //  Usage Minimum (Reserved (no event indicated))
  0x29, 0xff,                     //  Usage Maxium (Reserved)a
  0x75, 0x08,                     //  Report Size (8)
  0x95, 0x06,                     //  Report Count (6)
  0x81, 0x00,                     //  Input (Data, Array)
  0xc0,                           //  End collection

  0x05, 0x0C,                     //  Usage Page (Consumer Devices)
  0x09, 0x01,                     //  Usage (Consumer Control)
  0xA1, 0x01,                     //  Collection (Application)

  0x85, 0x02,                     //  Report ID 2

  0x19, 0x00,                     //  Usage Minimum (Reserved (no event indicated))
  0x29, 0xFF,                     //  Usage Maxium (Reserved)
  0x75, 0x08,                     //  Report Size (8)
  0x95, 0x01,                     //  Report Count (1)
  0x81, 0x00,                     //  Input (Data, Ary, Abs)

  0xc0                            //  End collections
};

enum kb_report_id1_idx {
  KB1_REPORT_ID = 0,
  KB1_MOD_KEY,
  KB1_KEY_CODE1,
  KB1_KEY_CODE2,
  KB1_KEY_CODE3,
  KB1_KEY_CODE4,
  KB1_KEY_CODE5,
  KB1_KEY_CODE6,
  KB1_REPORT_COUNT
};

enum kb_report_id2_idx {
  KB2_REPORT_ID = 0,
  KB2_KEY_CODE1,
  KB2_REPORT_COUNT,
};

#define NUMBER_OF_REPORTS 2

/*==kb_cb====================================================================================*/
#define KB_EP_00_REPORT_SIZE 2

/*==kb_bt_cb=================================================================================*/
#define BT_GATT_REPORTS_SERVICE \
  BT_GATT_DESCRIPTOR(BT_UUID_HIDS_REPORT_REF, BT_GATT_PERM_READ, read_report, NULL, &input[0]), \
  BT_GATT_DESCRIPTOR(BT_UUID_HIDS_REPORT_REF, BT_GATT_PERM_READ, read_report, NULL, &input[1])

#define BT_PUSH_REPORTS_PRECHECK !(!report[0] || report[0] <= ARRAY_SIZE(input))

/*==kb_scan==================================================================================*/
#define KB_SCAN_KEY_CB \
  keymap[5][ 1].on_set = kb_scan_fn_gui_set;\
  keymap[5][10].on_set = kb_scan_fn_set;\
  keymap[5][13].on_set = kb_scan_fn_arrow_lt_set;\
  keymap[5][14].on_set = kb_scan_fn_arrow_dn_set;\
  keymap[5][15].on_set = kb_scan_fn_arrow_rt_set;\
  keymap[4][14].on_set = kb_scan_fn_arrow_up_set;\
  keymap[0][11].on_set = kb_scan_fn_f9_set;\
  keymap[0][12].on_set = kb_scan_fn_f10_set;\
  keymap[0][13].on_set = kb_scan_fn_f11_set;\
  keymap[0][14].on_set = kb_scan_fn_f12_set;

void kb_scan_fn_gui_set(void);
void kb_scan_fn_gui_reset(void);
void kb_scan_fn_set(void);
void kb_scan_fn_reset(void);
void kb_scan_fn_arrow_up_set(void);
void kb_scan_fn_arrow_dn_set(void);
void kb_scan_fn_arrow_lt_set(void);
void kb_scan_fn_arrow_rt_set(void);
void kb_scan_fn_arrow_all_reset(void);
void kb_scan_fn_f9_set(void);
void kb_scan_fn_f10_set(void);
void kb_scan_fn_f11_set(void);
void kb_scan_fn_f12_set(void);
void kb_scan_fn_f_all_reset(void);

/*==kb_led===================================================================================*/
#define use_1
//#define use_2

#define KB_LED_MODE_ARRAY \
  kb_led_mode_0, \
  kb_led_mode_1, \
  kb_led_mode_2, \
  kb_led_mode_3, \
  kb_led_mode_4

void kb_led_mode_0(void);
void kb_led_mode_1(void);
void kb_led_mode_2(void);
void kb_led_mode_3(void);
void kb_led_mode_4(void);