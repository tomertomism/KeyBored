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

#define KB_SCAN_FN_PRESSED 0x01
#define KB_SCAN_CAPS_LOCKED 0x02
#define KB_SCAN_SCROLL_LOCKED 0x06
#define KB_SCAN_LED_LOCKED 0x08

typedef struct {
  const uint8_t polling_ms;
  uint8_t value;
  uint8_t media_row;
  uint8_t media_col;
  uint8_t is_qdec_ready;
  int8_t (*push_report)(uint8_t *report, uint8_t size, k_ticks_t delay);
}__packed(kb_scan_cfg_t);

extern kb_scan_cfg_t scan_cfg1;

void kb_set_row(uint8_t idx);
uint8_t kb_get_col(uint8_t *report, uint8_t *id1_offset, uint8_t row_idx, kb_scan_cfg_t *cfg);
uint8_t translateMODkey(uint8_t code);
int8_t kb_report_push_usb(uint8_t *report, uint8_t size, k_ticks_t delay);
int8_t kb_report_push_ble(uint8_t *report, uint8_t size, k_ticks_t delay);
void kb_run_scan(void);