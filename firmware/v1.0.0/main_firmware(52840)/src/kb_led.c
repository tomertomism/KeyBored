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

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2s.h>

#include "../inc/keymap_lut.h"
#include "../inc/kb_led.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(kb_led, LOG_LEVEL_INF);

K_THREAD_DEFINE(kb_run_led_id, 1024, kb_run_led, NULL, NULL, NULL, 12, 0, 2000);

static void (*const kb_led_mode[])(void) = {KB_LED_MODE_ARRAY};

#define ANGLE_SEED_MAX (LUT_SIZE * 160) - 1

static const enum kb_led_brightness brt_lut[] = {
  KB_LED_BRT_0,
  KB_LED_BRT_1,
  KB_LED_BRT_2,
  KB_LED_BRT_3,
  KB_LED_BRT_4,
  KB_LED_BRT_5,
  KB_LED_BRT_6,
};

kb_led_t led_cfg1 = {
  .mode = 1,
  .speed_delay = 50,
  .hue_angle_seed = 0,
  .brightness_idx = 1,
};

static const struct device *led_i2s = DEVICE_DT_GET(DT_NODELABEL(led_strip));

const uint16_t led_strip_size = DT_PROP(DT_NODELABEL(led_strip), chain_length);

struct nvs_fs nvs_cfg1 = {
  .flash_device = FIXED_PARTITION_DEVICE(storage_partition),
  .offset = FIXED_PARTITION_OFFSET(storage_partition),
};

void kb_run_led() {
	struct flash_pages_info info;
  LOG_INF("%d, %d", sizeof(*led_cfg1.colour), sizeof(led_cfg1));
  int8_t ret = 0;
  ret = device_is_ready(led_i2s);
  if (!ret) {
    LOG_ERR("led_strip is in error, %d", ret);
    return;
  }
  LOG_INF("led_strip is ready");

	if (!device_is_ready(nvs_cfg1.flash_device)) {
		LOG_ERR("Flash device %s is not ready", nvs_cfg1.flash_device->name);
		return;
	}

  ret = flash_get_page_info_by_offs(nvs_cfg1.flash_device, nvs_cfg1.offset, &info);
	if (ret) {
		LOG_ERR("Unable to get page info, %d", ret);
		return;
	}
	nvs_cfg1.sector_size = info.size;
	nvs_cfg1.sector_count = FIXED_PARTITION_SIZE(storage_partition) / info.size;

  ret = nvs_mount(&nvs_cfg1);
	if (ret) {
		LOG_ERR("Flash Init failed, %d", ret);
		return;
	}
	LOG_INF("partition init good");

  //LOG_INF("%d %d %d", brt_lut[led_cfg1.brightness_idx], led_cfg1.mode, led_cfg1.speed_delay);

  while(1){
    k_ticks_t next_wake = k_uptime_ticks();
    
    if (kb_led_mode[led_cfg1.mode]) kb_led_mode[led_cfg1.mode]();

    ret = led_strip_update_rgb(led_i2s, led_cfg1.colour, led_strip_size);
    if (ret) {
      LOG_ERR("led_strip update error, %d", ret);
      return;
    }
    //LOG_INF("led sent, %llu", k_uptime_ticks() - next_wake);
    k_ticks_t ticks_to_wait = next_wake + k_ms_to_ticks_floor64(led_cfg1.speed_delay) - k_uptime_ticks();
    if (ticks_to_wait > 0) {
        k_sleep(K_TICKS(ticks_to_wait));
    }
    //LOG_INF("%d %d %d", brt_lut[led_cfg1.brightness_idx], led_cfg1.mode, led_cfg1.speed_delay);
  }
}

void kb_led_brt_inc(void) {
  if (led_cfg1.brightness_idx < (ARRAY_SIZE(brt_lut) - 1)) led_cfg1.brightness_idx++;
}

void kb_led_brt_dec(void) {
  if (led_cfg1.brightness_idx > 0) led_cfg1.brightness_idx--;
}

void kb_led_spd_inc(void) {
  if (led_cfg1.speed_delay > 30) led_cfg1.speed_delay -= 10;
}

void kb_led_spd_dec(void) {
  if (led_cfg1.speed_delay < 150) led_cfg1.speed_delay += 10;
}

void kb_led_mod_inc(void) {
  if (led_cfg1.mode < (ARRAY_SIZE(kb_led_mode) - 1)) led_cfg1.mode++;
}

void kb_led_mod_dec(void) {
  if (led_cfg1.mode > 0) led_cfg1.mode--;
}

void kb_led_hue_inc(void) {
  if (led_cfg1.hue_angle_seed < ANGLE_SEED_MAX) led_cfg1.hue_angle_seed += 10;
}

void kb_led_hue_dec(void) {
  if (led_cfg1.hue_angle_seed > 10) led_cfg1.hue_angle_seed -= 10;
}

__weak void kb_led_mode_0() {
  for (uint8_t i = 0; i < ARRAY_SIZE(kb_rows); i++) {
    for (uint8_t j = 0; j < ARRAY_SIZE(kb_cols); j++) {
      if (keymap[i][j].led_idx > -1) {
        led_cfg1.colour[keymap[i][j].led_idx].r = 0;
        led_cfg1.colour[keymap[i][j].led_idx].g = 0;
        led_cfg1.colour[keymap[i][j].led_idx].b = 0;
      }
    }
  }
}

__weak void kb_led_mode_1() {
  if (led_cfg1.hue_angle_seed++ >= ANGLE_SEED_MAX) led_cfg1.hue_angle_seed = 0;

  for (uint8_t i = 0; i < ARRAY_SIZE(kb_rows); i++) {
    for (uint8_t j = 0; j < ARRAY_SIZE(kb_cols); j++) {
      if (keymap[i][j].led_idx > -1) {
        uint16_t idx = (led_cfg1.hue_angle_seed + 
                       (keymap[i][j].led_x_position_in_1_8U * 10) + 
                       (keymap[i][j].led_y_position_in_1_8U * 10)) % LUT_SIZE;
        led_cfg1.colour[keymap[i][j].led_idx].r = hue2r[idx] * brt_lut[led_cfg1.brightness_idx] / 100;
        led_cfg1.colour[keymap[i][j].led_idx].g = hue2g[idx] * brt_lut[led_cfg1.brightness_idx] / 100;
        led_cfg1.colour[keymap[i][j].led_idx].b = hue2b[idx] * brt_lut[led_cfg1.brightness_idx] / 100;
      }
    }
  }
}

__weak void kb_led_mode_2() {
}

__weak void kb_led_mode_3() {
  for (uint8_t i = 0; i < ARRAY_SIZE(kb_rows); i++) {
    for (uint8_t j = 0; j < ARRAY_SIZE(kb_cols); j++) {
      if (keymap[i][j].led_idx > -1 && keymap[i][j].single_brt) {
        uint16_t idx = led_cfg1.hue_angle_seed % LUT_SIZE, 
                percent = keymap[i][j].single_brt * brt_lut[led_cfg1.brightness_idx];
        led_cfg1.colour[keymap[i][j].led_idx].r = hue2r[idx] * percent / 10000;
        led_cfg1.colour[keymap[i][j].led_idx].g = hue2g[idx] * percent / 10000;
        led_cfg1.colour[keymap[i][j].led_idx].b = hue2b[idx] * percent / 10000;
        keymap[i][j].single_brt -= 5;
      }
    }
  }
}

__weak void kb_led_mode_4() {

}
