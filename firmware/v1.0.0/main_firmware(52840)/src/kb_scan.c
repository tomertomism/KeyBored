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
#include <zephyr/usb/usbd.h>
#include <zephyr/usb/class/usbd_hid.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/watchdog.h>

#include "../inc/editable.h"
#include "../inc/kb_cb.h"
#include "../inc/kb_bt_cb.h"
#include "../inc/keymap_lut.h"
#include "../inc/kb_scan.h"
#include "../inc/kb_led.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(kb_scan, LOG_LEVEL_INF);

K_THREAD_DEFINE(kb_run_scan_id, 1024, kb_run_scan, NULL, NULL, NULL, 5, 0, 1500);

uint8_t report[KB1_REPORT_COUNT + KB2_REPORT_COUNT] = {0x01, 0, 0x04, 0, 0, 0, 0, 0, 0x02, 0};

static const struct device *hid_dev = DEVICE_DT_GET_ONE(zephyr_hid_device);

static const struct device *knob_qdec = DEVICE_DT_GET(DT_NODELABEL(just_a_qdec));

static const struct device *wdt_dev = DEVICE_DT_GET(DT_NODELABEL(wdt0));

struct wdt_timeout_cfg wdt_config = {
  .window.min = 0,
  .window.max = 5000,
  .flags = WDT_FLAG_RESET_SOC,
  .callback = NULL,
};

kb_scan_cfg_t scan_cfg1 = {
  .value = 0,
  .polling_ms = 1,
};

void kb_set_row(uint8_t idx){
  for(uint8_t i = 0; i < ARRAY_SIZE(kb_rows); i++){
    gpio_pin_set_dt(&kb_rows[i], i == idx ? 1 : 0);
  }
}

uint8_t kb_get_col(uint8_t *report, uint8_t *id1_offset, uint8_t row_idx, kb_scan_cfg_t *cfg){
  int8_t ret = 0;

  for(uint8_t i = 0; i < ARRAY_SIZE(kb_cols); i++) {
    ret = gpio_pin_get_dt(&kb_cols[i]);

    // if key(s) is/are pressed
    if (ret > 0) {

      keymap[row_idx][i].single_brt = 100;

      // if FN key is pressed
      if (keymap[row_idx][i].on_set) keymap[row_idx][i].on_set();

      uint8_t pr_id = (cfg->value & KB_SCAN_FN_PRESSED) ? 1 : 0;
      uint8_t rep_id = keymap[row_idx][i].report_idx[pr_id];
      
      if (rep_id == 1) {
        uint8_t code = keymap[row_idx][i].keycode[pr_id];
        if (code >= 0xe0 && code <= 0xe7)
          report[KB1_MOD_KEY] = translateMODkey(code);
        else if (*id1_offset < 6)
          report[2 + (*id1_offset)++] = code;
        report[KB1_REPORT_ID] = 0x01;
      }

      //LOG_INF("%d %#x", rep_id, keymap[row_idx][i].keycode[pr_id]);
      if (rep_id == 2) {
        report[KB1_REPORT_COUNT + KB2_KEY_CODE1] = keymap[row_idx][i].keycode[pr_id];
        report[KB1_REPORT_COUNT + KB2_REPORT_ID] = 0x02;
      }

    // if no key is pressed
    }else if (ret == 0){

      if (keymap[row_idx][i].on_reset) keymap[row_idx][i].on_reset();

    }
  }
  return ret;
}

uint8_t translateMODkey(uint8_t code){
  return ((code - 0xe0) <= 7) ? (1 << (code - 0xe0)) : 0;
}

int8_t kb_report_push_usb(uint8_t *report, uint8_t size, k_ticks_t delay) {
  ARG_UNUSED(delay);
  int8_t ret = 0;
  if (is_usb_ready) {
    ret = hid_device_submit_report(hid_dev, size, report);
    if (ret) {
      LOG_ERR("report%d sent failed, %d", report[0], ret);
    } else {
      LOG_INF("report%d pushed by usb", report[0]);
      LOG_HEXDUMP_INF(report, size, "");
      return ret;
    }
  }
  
  LOG_INF("USB HID device is not ready");
  return ret;
}

int8_t kb_report_push_ble(uint8_t *report, uint8_t size, k_ticks_t delay) {
  int8_t ret = 0;
  if (is_ble_ready) {
    ret = kb_bt_send_report_id(report, delay, size);
    if (ret) 
      LOG_ERR("report%d notify failed, %d", report[0], ret);
    else {
      LOG_INF("report%d pushed by ble", report[0]);
      LOG_HEXDUMP_INF(report, size, "");
      return ret;
    }
  }

  LOG_INF("BT device is not ready");
  return ret;
}

void kb_run_scan() {
  int8_t ret = 0;
  int wdt_channel_id;
  if (!scan_cfg1.polling_ms) {
    LOG_ERR("kb_run_scan cannot run with 0 polling ms");
    return;
  }

  if (ARRAY_SIZE(kb_rows) > 0) {
    for (size_t i = 0; i < ARRAY_SIZE(kb_rows); i++) {
      if (!gpio_is_ready_dt(&kb_rows[i])) {
        LOG_ERR("kb row_%d failed", i);
        return;
      }
      gpio_pin_configure_dt(&kb_rows[i], GPIO_OUTPUT_INACTIVE);
    }
    LOG_INF("GPIO %d rows is ready", ARRAY_SIZE(kb_rows));
  } else {
    LOG_ERR("no kb row(s) defined");
    return;
  }

  if (ARRAY_SIZE(kb_cols) > 0) {
    for (size_t i = 0; i < ARRAY_SIZE(kb_cols); i++) {
      if (!gpio_is_ready_dt(&kb_cols[i])) {
        LOG_ERR("kb col_%d failed", i);
        return;
      }
      gpio_pin_configure_dt(&kb_cols[i], GPIO_INPUT);
    }
    LOG_INF("GPIO %d cols is ready", ARRAY_SIZE(kb_cols));
  } else {
    LOG_ERR("no kb column(s) defined");
    return;
  }

  ret = device_is_ready(knob_qdec);
  if (!ret) {
    LOG_ERR("QDEC failed, %d", ret);
    scan_cfg1.is_qdec_ready = 0;
  } else {
    scan_cfg1.is_qdec_ready = 1;
    LOG_INF("QDEC ready");
  }

  ret = device_is_ready(wdt_dev);
  if (!ret) {
    LOG_ERR("WDT not ready, %d", ret);
    return;
  }

  wdt_channel_id = wdt_install_timeout(wdt_dev, &wdt_config);
  if (wdt_channel_id < 0) {
    LOG_ERR("WDT install failed");
    return;
  }

  if (wdt_setup(wdt_dev, 0) < 0) {
    LOG_ERR("WDT setup failed");
    return;
  }
  LOG_INF("wdt ready");

  KB_SCAN_KEY_CB

  while (1){
    if (wdt_dev) wdt_feed(wdt_dev, wdt_channel_id);
    k_ticks_t next_wake = k_uptime_ticks();
    uint8_t *kb_buf = k_malloc(KB1_REPORT_COUNT + KB2_REPORT_COUNT);
    memset(kb_buf, 0, KB1_REPORT_COUNT + KB2_REPORT_COUNT);

    uint8_t current_row = ARRAY_SIZE(kb_rows), id1_offset = 0;
    do{
      kb_set_row(current_row - 1);
      if ((current_row - 1) < ARRAY_SIZE(kb_rows)) {
        k_busy_wait(10);
        kb_get_col(kb_buf, &id1_offset, current_row - 1, &scan_cfg1);
      }
    }while(current_row--);
    //LOG_INF("%d, %d", (uint16_t) (k_uptime_ticks() - next_wake), id1_offset);

    if (kb_buf[KB1_REPORT_COUNT] != 0x02 && scan_cfg1.is_qdec_ready) {
      struct sensor_value val;
      ret = sensor_sample_fetch(knob_qdec);
      if (ret) {
        LOG_ERR("qdec error: %d", ret);
      }
      ret = sensor_channel_get(knob_qdec, SENSOR_CHAN_ROTATION, &val);
      if (ret) {
        LOG_ERR("qdec reading error: %d", ret);
      }
      //todo val...
    }

    if (memcmp(&report[KB1_KEY_CODE1], &kb_buf[KB1_KEY_CODE1], KB1_REPORT_COUNT - KB1_KEY_CODE1)) {
      memcpy(&report[KB1_KEY_CODE1], &kb_buf[KB1_KEY_CODE1], KB1_REPORT_COUNT - KB1_KEY_CODE1);
      if (scan_cfg1.push_report) 
        scan_cfg1.push_report(report, KB1_REPORT_COUNT, k_ms_to_ticks_floor64(scan_cfg1.polling_ms));
    }

    if (memcmp(&report[KB1_REPORT_COUNT + KB2_KEY_CODE1], &kb_buf[KB1_REPORT_COUNT + KB2_KEY_CODE1], KB2_KEY_CODE1)) {
      memcpy(&report[KB1_REPORT_COUNT + KB2_KEY_CODE1], &kb_buf[KB1_REPORT_COUNT + KB2_KEY_CODE1], KB2_KEY_CODE1);
      if (scan_cfg1.push_report) 
        scan_cfg1.push_report(&report[KB1_REPORT_COUNT], KB2_REPORT_COUNT, k_ms_to_ticks_floor64(scan_cfg1.polling_ms));
    }
    k_free(kb_buf);

    if(kb_ep_00_report[0] & KB_EP_00_REPORT_SET) {
      kb_ep_00_report[0] &= ~KB_EP_00_REPORT_SET;
      LOG_HEXDUMP_INF(kb_ep_00_report, KB_EP_00_REPORT_SIZE, "");
      //todo ep00
    }
    //LOG_INF("%d", (uint16_t) (k_uptime_ticks() - next_wake));
    next_wake += k_ms_to_ticks_floor64(scan_cfg1.polling_ms);
    k_ticks_t ticks_to_wait = next_wake - k_uptime_ticks();
    if (ticks_to_wait > 0) {
      k_sleep(K_TICKS(ticks_to_wait));
    }
  }
}

__weak void kb_scan_fn_gui_set(void) {
  if (scan_cfg1.value & KB_SCAN_FN_PRESSED) {
    keymap[5][1].on_set = NULL;
    keymap[5][1].on_reset = kb_scan_fn_gui_reset;
    keymap[5][1].keycode[0] = 0;
    keymap[5][1].report_idx[0] = 0;
    LOG_INF("GUI key disabled");
  }
}

__weak void kb_scan_fn_gui_reset(void) {
  if (scan_cfg1.value & KB_SCAN_FN_PRESSED) {
    keymap[5][1].on_set = kb_scan_fn_gui_set;
    keymap[5][1].on_reset = NULL;
    keymap[5][1].keycode[0] = 0xe3;
    keymap[5][1].report_idx[0] = 1;
    LOG_INF("GUI key enabled");
  }
}

void kb_scan_fn_set() {
  keymap[5][10].on_set = NULL;
  keymap[5][10].on_reset = kb_scan_fn_reset;
  scan_cfg1.value |= KB_SCAN_FN_PRESSED;
  LOG_INF("FN pressed");
}

void kb_scan_fn_reset() {
  keymap[5][10].on_set = kb_scan_fn_set;
  keymap[5][10].on_reset = NULL;
  scan_cfg1.value &= ~(KB_SCAN_FN_PRESSED);
  LOG_INF("FN released");
}

__weak void kb_scan_fn_arrow_up_set() {
  if (scan_cfg1.value & KB_SCAN_FN_PRESSED && !(scan_cfg1.value & KB_SCAN_LED_LOCKED)) {
    scan_cfg1.value |= KB_SCAN_LED_LOCKED;
    scan_cfg1.media_row = 4;
    scan_cfg1.media_col = 14;
    keymap[scan_cfg1.media_row][scan_cfg1.media_col].on_reset = kb_scan_fn_arrow_all_reset;
    kb_led_brt_inc();
    LOG_INF("led brt inc");
  }
}

__weak void kb_scan_fn_arrow_dn_set() {
  if (scan_cfg1.value & KB_SCAN_FN_PRESSED && !(scan_cfg1.value & KB_SCAN_LED_LOCKED)) {
    scan_cfg1.value |= KB_SCAN_LED_LOCKED;
    scan_cfg1.media_row = 5;
    scan_cfg1.media_col = 14;
    keymap[scan_cfg1.media_row][scan_cfg1.media_col].on_reset = kb_scan_fn_arrow_all_reset;
    kb_led_brt_dec();
    LOG_INF("led brt dec");
  }
}

__weak void kb_scan_fn_arrow_lt_set() {
  if (scan_cfg1.value & KB_SCAN_FN_PRESSED && !(scan_cfg1.value & KB_SCAN_LED_LOCKED)) {
    scan_cfg1.value |= KB_SCAN_LED_LOCKED;
    scan_cfg1.media_row = 5;
    scan_cfg1.media_col = 13;
    keymap[scan_cfg1.media_row][scan_cfg1.media_col].on_reset = kb_scan_fn_arrow_all_reset;
    kb_led_mod_dec();
    LOG_INF("led mode dec");
  }
}

__weak void kb_scan_fn_arrow_rt_set() {
  if (scan_cfg1.value & KB_SCAN_FN_PRESSED && !(scan_cfg1.value & KB_SCAN_LED_LOCKED)) {
    scan_cfg1.value |= KB_SCAN_LED_LOCKED;
    scan_cfg1.media_row = 5;
    scan_cfg1.media_col = 15;
    keymap[scan_cfg1.media_row][scan_cfg1.media_col].on_reset = kb_scan_fn_arrow_all_reset;
    kb_led_mod_inc();
    LOG_INF("led mode inc");
  }
}

__weak void kb_scan_fn_arrow_all_reset() {
  keymap[scan_cfg1.media_row][scan_cfg1.media_col].on_reset = NULL;
  scan_cfg1.value &= ~KB_SCAN_LED_LOCKED;
  scan_cfg1.media_row = 0;
  scan_cfg1.media_col = 0;
}

__weak void kb_scan_fn_f9_set(void) {
  if (scan_cfg1.value & KB_SCAN_FN_PRESSED && !(scan_cfg1.value & KB_SCAN_LED_LOCKED)) {
    scan_cfg1.value |= KB_SCAN_LED_LOCKED;
    scan_cfg1.media_row = 0;
    scan_cfg1.media_col = 11;
    keymap[scan_cfg1.media_row][scan_cfg1.media_col].on_reset = kb_scan_fn_f_all_reset;
    kb_led_hue_dec();
    LOG_INF("led hue dec");
  }
}

__weak void kb_scan_fn_f10_set(void) {
  if (scan_cfg1.value & KB_SCAN_FN_PRESSED && !(scan_cfg1.value & KB_SCAN_LED_LOCKED)) {
    scan_cfg1.value |= KB_SCAN_LED_LOCKED;
    scan_cfg1.media_row = 0;
    scan_cfg1.media_col = 12;
    keymap[scan_cfg1.media_row][scan_cfg1.media_col].on_reset = kb_scan_fn_f_all_reset;
    kb_led_hue_inc();
    LOG_INF("led hue inc");
  }
}

__weak void kb_scan_fn_f11_set() {
  if (scan_cfg1.value & KB_SCAN_FN_PRESSED && !(scan_cfg1.value & KB_SCAN_LED_LOCKED)) {
    scan_cfg1.value |= KB_SCAN_LED_LOCKED;
    scan_cfg1.media_row = 0;
    scan_cfg1.media_col = 13;
    keymap[scan_cfg1.media_row][scan_cfg1.media_col].on_reset = kb_scan_fn_f_all_reset;
    kb_led_spd_dec();
    LOG_INF("led spd dec");
  }
}

__weak void kb_scan_fn_f12_set() {
  if (scan_cfg1.value & KB_SCAN_FN_PRESSED && !(scan_cfg1.value & KB_SCAN_LED_LOCKED)) {
    scan_cfg1.value |= KB_SCAN_LED_LOCKED;
    scan_cfg1.media_row = 0;
    scan_cfg1.media_col = 14;
    keymap[scan_cfg1.media_row][scan_cfg1.media_col].on_reset = kb_scan_fn_f_all_reset;
    kb_led_spd_inc();
    LOG_INF("led spd inc");
  }
}

__weak void kb_scan_fn_f_all_reset() {
  kb_scan_fn_arrow_all_reset();
}
