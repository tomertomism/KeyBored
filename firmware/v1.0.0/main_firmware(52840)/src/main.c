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

#include <sample_usbd.h>

#include <zephyr/kernel/thread.h>
#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/devicetree.h>
#include <zephyr/usb/usbd.h>
#include <zephyr/usb/class/usbd_hid.h>

#include "../inc/editable.h"
#include "../inc/kb_cb.h"
#include "../inc/kb_bt_cb.h"
#include "../inc/main.h"
#include "../inc/keymap_lut.h"
#include "../inc/kb_adc.h"
#include "../inc/kb_scan.h"
#include "../inc/kb_led.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static const struct device *hid_dev = DEVICE_DT_GET_ONE(zephyr_hid_device);
//static const struct device *adc = DEVICE_DT_GET_ONE(adc);

int main(void){
  int ret = 0;
  struct usbd_context *sample_usbd = 0;
  scan_cfg1.push_report = kb_report_push_usb;
  usb_init(sample_usbd);
  k_msleep(900);

  if (!is_usb_ready) {
    scan_cfg1.push_report = kb_report_push_ble;
    ret = bt_init();
    if (ret) {
      LOG_ERR("BT init failed");
      return 0;
    }
  }

  while (1) {
    k_sleep(K_TICKS(K_TICKS_FOREVER));
  }
  return 0;
}

int usb_init(struct usbd_context *sample_usbd){
  int8_t ret;

  if (!device_is_ready(hid_dev)) {
    LOG_ERR("HID Device is not ready");
    return -EIO;
  }

  k_msleep(100);
  ret = hid_device_register(hid_dev, hid_report_desc, sizeof(hid_report_desc), &kb_ops);
  if (ret != 0) {
    LOG_ERR("Failed to register HID Device, %d", ret);
    return ret;
  }

  sample_usbd = sample_usbd_init_device(msg_cb);
  if (sample_usbd == NULL) {
    LOG_ERR("Failed to initialize USB device");
    return -ENODEV;
  }else
    LOG_INF("sample_usbd: %#08x", (uint32_t) sample_usbd);
  
  if (!usbd_can_detect_vbus(sample_usbd)) {
    /* doc device enable start */
    ret = usbd_enable(sample_usbd);
    if (ret) {
      LOG_ERR("Failed to enable device support");
      return ret;
    }
    /* doc device enable end */
  }
  return ret;
}

int bt_init(){
  int err = 0;

  err = bt_enable(bt_ready);
  if (err) {
    LOG_ERR("Bluetooth init failed (err %d)", err);
    return 0;
  }

  return err;
}
