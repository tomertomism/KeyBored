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
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/usb/class/hid.h>
#include <zephyr/drivers/gpio.h>

struct key_mapping_t{
  uint8_t keycode[2];
  uint8_t report_idx[2];
  const int16_t led_idx;
  const int16_t led_x_position_in_1_8U;
  const int16_t led_y_position_in_1_8U;
  uint8_t single_brt;
  void (*on_set)(void);
  void (*on_reset)(void);
} __packed;

#define GPIO_SPEC_FROM_CHILD(child_id) GPIO_DT_SPEC_GET(child_id, gpios),

static const struct gpio_dt_spec kb_rows[] = {
  DT_FOREACH_CHILD_STATUS_OKAY(DT_NODELABEL(row_scan_array), GPIO_SPEC_FROM_CHILD)
};

static const struct gpio_dt_spec kb_cols[] = {
  DT_FOREACH_CHILD_STATUS_OKAY(DT_NODELABEL(col_scan_array), GPIO_SPEC_FROM_CHILD)
};

extern struct key_mapping_t keymap[ARRAY_SIZE(kb_rows)][ARRAY_SIZE(kb_cols) + 2];
