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

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>

#include <zephyr/settings/settings.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include "../inc/editable.h"

extern const struct bt_data ad[];
extern const struct bt_data sd[];
extern uint8_t is_using_ble;

void connected(struct bt_conn *conn, uint8_t err);
void disconnected(struct bt_conn *conn, uint8_t reason);
void security_changed(struct bt_conn *conn, bt_security_t level, enum bt_security_err err);

void bt_ready(int err);
void auth_passkey_display(struct bt_conn *conn, unsigned int passkey);
void auth_cancel(struct bt_conn *conn);
extern struct bt_conn_auth_cb auth_cb_display;

extern struct hids_info info;
ssize_t read_info(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
ssize_t read_report_map(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
ssize_t read_report(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
void input_ccc_changed(const struct bt_gatt_attr *attr, uint16_t value);
ssize_t read_input_report(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
ssize_t write_ctrl_point(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags);

// void hog_button_loop(void);
int8_t kb_bt_send_report_id(uint8_t *report, uint8_t size, k_ticks_t delay);

// #define SAMPLE_BT_PERM_READ BT_GATT_PERM_READ_AUTHEN
// #define SAMPLE_BT_PERM_WRITE BT_GATT_PERM_WRITE_AUTHEN

#define SAMPLE_BT_PERM_READ BT_GATT_PERM_READ_ENCRYPT
#define SAMPLE_BT_PERM_WRITE BT_GATT_PERM_WRITE_ENCRYPT

// #define SAMPLE_BT_PERM_READ BT_GATT_PERM_READ
// #define SAMPLE_BT_PERM_WRITE BT_GATT_PERM_WRITE

enum {
	HIDS_REMOTE_WAKE = BIT(0),
	HIDS_NORMALLY_CONNECTABLE = BIT(1),
};

struct hids_info {
	uint16_t version; /* version number of base USB HID Specification */
	uint8_t code; /* country HID Device hardware is localized for. */
	uint8_t flags;
} __packed;

struct hids_report {
	uint8_t id; /* report id */
	uint8_t type; /* report type */
} __packed;

enum {
	HIDS_INPUT = 0x01,
	HIDS_OUTPUT = 0x02,
	HIDS_FEATURE = 0x03,
};

extern struct hids_report input[NUMBER_OF_REPORTS];
extern uint8_t ctrl_point;
extern uint8_t is_ble_ready;
