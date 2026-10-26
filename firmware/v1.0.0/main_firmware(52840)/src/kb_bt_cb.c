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

#include "../inc/kb_bt_cb.h"

#include <zephyr/kernel/thread.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/reboot.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(kb_bt_cb, LOG_LEVEL_INF);

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
	.security_changed = security_changed,
};

BT_GATT_SERVICE_DEFINE(hog_svc,
	BT_GATT_PRIMARY_SERVICE(BT_UUID_HIDS),
	BT_GATT_CHARACTERISTIC(BT_UUID_HIDS_INFO, BT_GATT_CHRC_READ, BT_GATT_PERM_READ, read_info, NULL, &info),
	BT_GATT_CHARACTERISTIC(BT_UUID_HIDS_REPORT_MAP, BT_GATT_CHRC_READ, BT_GATT_PERM_READ, read_report_map, NULL, NULL),
	BT_GATT_CHARACTERISTIC(BT_UUID_HIDS_REPORT, BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, SAMPLE_BT_PERM_READ, read_input_report, NULL, NULL),
	BT_GATT_CCC(input_ccc_changed, SAMPLE_BT_PERM_READ | SAMPLE_BT_PERM_WRITE),
  BT_GATT_REPORTS_SERVICE,
	BT_GATT_CHARACTERISTIC(BT_UUID_HIDS_CTRL_POINT, BT_GATT_CHRC_WRITE_WITHOUT_RESP, BT_GATT_PERM_WRITE, NULL, write_ctrl_point, &ctrl_point),
);

const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL, BT_UUID_16_ENCODE(BT_UUID_HIDS_VAL), BT_UUID_16_ENCODE(BT_UUID_BAS_VAL)),
};

const struct bt_data sd[] = {
	BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME) - 1),
};

void connected(struct bt_conn *conn, uint8_t err){
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	if (err) {
		LOG_ERR("Failed to connect to %s, err 0x%02x %s", addr, err, bt_hci_err_to_str(err));
		return;
	}

	LOG_INF("Connected %s\n", addr);

	if (bt_conn_set_security(conn, BT_SECURITY_L2)) {
		LOG_ERR("Failed to set security");
	}
}

void disconnected(struct bt_conn *conn, uint8_t reason){
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	LOG_INF("Disconnected from %s, reason 0x%02x %s", addr, reason, bt_hci_err_to_str(reason));

  //sys_reboot(SYS_REBOOT_COLD);
}

void security_changed(struct bt_conn *conn, bt_security_t level, enum bt_security_err err){
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	if (!err) {
		LOG_INF("Security changed: %s level %u", addr, level);
	} else {
		LOG_ERR("Security failed: %s level %u err %s(%d)", addr, level, bt_security_err_to_str(err), err);
	}
}

void bt_ready(int err)
{
	if (err) {
		LOG_ERR("Bluetooth init failed (err %d)", err);
		return;
	}

  err = settings_subsys_init();
  if (err) {
    LOG_ERR("failed to init settings_subsys");
  }

	LOG_INF("Bluetooth initialized");

	//hog_init();

  err = settings_load();
  if (err) {
    LOG_ERR("failed to load settings");
    return;
  }
  
	err = bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (err) {
		LOG_ERR("Advertising failed to start (err %d)", err);
		return;
	}

	LOG_INF("Advertising successfully started");
}

void auth_passkey_display(struct bt_conn *conn, unsigned int passkey)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	LOG_WRN("Passkey for %s: %06u", addr, passkey);
}

void auth_cancel(struct bt_conn *conn)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	LOG_WRN("Pairing cancelled: %s", addr);
}

struct hids_info info = {
	.version = 0x0001,
	.code = 0x00,
	.flags = HIDS_NORMALLY_CONNECTABLE,
};

struct bt_conn_auth_cb auth_cb_display = {
	.passkey_display = auth_passkey_display,
	.passkey_entry = NULL,
	.cancel = auth_cancel,
};

struct hids_report input[NUMBER_OF_REPORTS] = {
  { .id = 0x01, .type = HIDS_INPUT, },
  { .id = 0x02, .type = HIDS_INPUT, },
};

uint8_t is_ble_ready = 0;
uint8_t ctrl_point = 0;

ssize_t read_info(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset){
  LOG_INF("read_info called");
	return bt_gatt_attr_read(conn, attr, buf, len, offset, attr->user_data, sizeof(struct hids_info));
}

ssize_t read_report_map(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset){
  LOG_INF("read_report_map called");
	return bt_gatt_attr_read(conn, attr, buf, len, offset, hid_report_desc, sizeof(hid_report_desc));
}

ssize_t read_report(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset){
  LOG_INF("read_report called");
	return bt_gatt_attr_read(conn, attr, buf, len, offset, attr->user_data, sizeof(struct hids_report));
}

void input_ccc_changed(const struct bt_gatt_attr *attr, uint16_t value){
  LOG_INF("input_ccc_changed called, %d", value);
	is_ble_ready = (value == BT_GATT_CCC_NOTIFY) ? 1 : 0;
}

ssize_t read_input_report(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset){
  LOG_INF("read_input_report called");
	return bt_gatt_attr_read(conn, attr, buf, len, offset, NULL, 0);
}

ssize_t write_ctrl_point(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags){
  LOG_INF("write_ctrl_point called");
	uint8_t *value = attr->user_data;

	if (offset + len > sizeof(ctrl_point)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}

	memcpy(value + offset, buf, len);

	return len;
}

// void hog_button_loop(void){
//   int8_t ret = 0;
// 	uint8_t i = 0;
// 	uint8_t report[8] = {1, 0, 0, 0, 0, 0, 0, 0};
// 	k_sleep(K_MSEC(3000));
// 	for (;;) {
// 		k_sleep(K_MSEC(900));
// 		report[2] = (i++ % 26) + 4;
// 		ret = bt_gatt_notify(NULL, &hog_svc.attrs[5], report, KB1_REPORT_COUNT);
//     LOG_INF("bt report notify: %d", ret);
// 		k_sleep(K_MSEC(100));
// 		report[2] = 0;
// 		bt_gatt_notify(NULL, &hog_svc.attrs[5], report, KB1_REPORT_COUNT);
// 	}
// }

int8_t kb_bt_send_report_id(uint8_t *report, uint8_t size, k_ticks_t delay){
  if (delay < k_ms_to_ticks_floor64(10)) {
    k_sleep(K_TICKS(k_ms_to_ticks_floor64(10) - delay));
  }
  
  if(BT_PUSH_REPORTS_PRECHECK) return -128;

  if (is_ble_ready) {
    return bt_gatt_notify(NULL, &hog_svc.attrs[report[0] + 4], &report[1], size - 1);
  }

  return -128;
}
