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
#include <stdlib.h>
#include <stdbool.h>
#include <zephyr/usb/usbd.h>
#include <zephyr/usb/class/usbd_hid.h>

#include "../inc/editable.h"

extern const struct hid_device_ops kb_ops;

extern uint32_t kb_duration;
extern bool is_usb_ready;

#define KB_EP_00_REPORT_SET 0x80
extern uint8_t kb_ep_00_report[KB_EP_00_REPORT_SIZE];

void msg_cb(struct usbd_context *const usbd_ctx, const struct usbd_msg *const msg);
void kb_iface_ready(const struct device *dev, const bool ready);
int kb_get_report(const struct device *dev, const uint8_t type, const uint8_t id, const uint16_t len, uint8_t *const buf);
int kb_set_report(const struct device *dev, const uint8_t type, const uint8_t id, const uint16_t len, const uint8_t *const buf);
void kb_set_idle(const struct device *dev, const uint8_t id, const uint32_t duration);
uint32_t kb_get_idle(const struct device *dev, const uint8_t id);
void kb_set_protocol(const struct device *dev, const uint8_t proto);
void kb_output_report(const struct device *dev, const uint16_t len, const uint8_t *const buf);
