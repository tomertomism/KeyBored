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

#include <zephyr/usb/usbd.h>
#include <zephyr/usb/class/usbd_hid.h>
#include <zephyr/usb/bos.h>
#include <zephyr/device.h>

#include "../inc/kb_cb.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(kb_cb, LOG_LEVEL_INF);

uint32_t kb_duration = 0;
bool is_usb_ready = 0;

uint8_t kb_ep_00_report[KB_EP_00_REPORT_SIZE] = {0, 0};

const struct hid_device_ops kb_ops = {
	.iface_ready = kb_iface_ready,
	.get_report = kb_get_report,
	.set_report = kb_set_report,
	.set_idle = kb_set_idle,
	.get_idle = kb_get_idle,
	.set_protocol = kb_set_protocol,
	.output_report = kb_output_report,
};

void msg_cb(struct usbd_context *const usbd_ctx, const struct usbd_msg *const msg){
	LOG_INF("USBD message: %s", usbd_msg_type_string(msg->type));

	if (msg->type == USBD_MSG_CONFIGURATION) {
		LOG_INF("\tConfiguration value %d", msg->status);
	}

	if (usbd_can_detect_vbus(usbd_ctx)) {
		if (msg->type == USBD_MSG_VBUS_READY) {
			if (usbd_enable(usbd_ctx)) {
				LOG_ERR("Failed to enable device support");
			}
		}

		if (msg->type == USBD_MSG_VBUS_REMOVED) {
			if (usbd_disable(usbd_ctx)) {
				LOG_ERR("Failed to disable device support");
			}
		}
	}
}

void kb_iface_ready(const struct device *dev, const bool ready){
	LOG_INF("HID device %s interface is %s", dev->name, ready ? "ready" : "not ready");
	is_usb_ready = ready;
}

int kb_get_report(const struct device *dev, const uint8_t type, const uint8_t id, const uint16_t len, uint8_t *const buf){
	LOG_ERR("Get Report not implemented, Type %u ID %u", type, id);
	return 0;
}

int kb_set_report(const struct device *dev, const uint8_t type, const uint8_t id, const uint16_t len, const uint8_t *const buf){
  //LOG_INF("HID report sent from host, %d", len);
  if (type != HID_REPORT_TYPE_OUTPUT) {
    LOG_WRN("Unsupported report type");
    return -ENOTSUP;
  }
  memcpy(kb_ep_00_report, buf, KB_EP_00_REPORT_SIZE);
  kb_ep_00_report[0] |= KB_EP_00_REPORT_SET;
  return 0;
}

/* Idle duration is stored but not used to calculate idle reports. */
void kb_set_idle(const struct device *dev, const uint8_t id, const uint32_t duration){
	LOG_INF("Set Idle %u to %u", id, duration);
	kb_duration = duration;
}

uint32_t kb_get_idle(const struct device *dev, const uint8_t id){
	LOG_INF("Get Idle %u to %u", id, kb_duration);
	return kb_duration;
}

void kb_set_protocol(const struct device *dev, const uint8_t proto){
	LOG_INF("Protocol changed to %s", proto == 0U ? "Boot Protocol" : "Report Protocol");
}

void kb_output_report(const struct device *dev, const uint16_t len, const uint8_t *const buf){
	//LOG_HEXDUMP_INF(buf, len, "o.r.");
	kb_set_report(dev, HID_REPORT_TYPE_OUTPUT, 0U, len, buf);
}
