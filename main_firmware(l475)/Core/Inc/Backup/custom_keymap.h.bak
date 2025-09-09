#pragma once
#include "custom_keymap_definition.h"

#define NUM_LED 82
#define LED_BUFFER_SIZE NUM_LED * 24 + 80
uint32_t led_buffer[LED_BUFFER_SIZE];

#define row0_set   (ROW0_GPIO_Port->BSRR = ROW0_Pin)
#define row0_reset (ROW0_GPIO_Port->BRR  = ROW0_Pin)

#define row1_set   (ROW1_GPIO_Port->BSRR = ROW1_Pin)
#define row1_reset (ROW1_GPIO_Port->BRR  = ROW1_Pin)

#define row2_set   (ROW2_GPIO_Port->BSRR = ROW2_Pin)
#define row2_reset (ROW2_GPIO_Port->BRR  = ROW2_Pin)

#define row3_set   (ROW3_GPIO_Port->BSRR = ROW3_Pin)
#define row3_reset (ROW3_GPIO_Port->BRR  = ROW3_Pin)

#define row4_set   (ROW4_GPIO_Port->BSRR = ROW4_Pin)
#define row4_reset (ROW4_GPIO_Port->BRR  = ROW4_Pin)

#define row5_set   (ROW5_GPIO_Port->BSRR = ROW5_Pin)
#define row5_reset (ROW5_GPIO_Port->BRR  = ROW5_Pin)

#define row_scan_interval 10

#define col0_offset  (COL0_GPIO_Port->IDR  &  COL0_Pin)

#define col1_offset  (COL1_GPIO_Port->IDR  &  COL1_Pin)

#define col2_offset  (COL2_GPIO_Port->IDR  &  COL2_Pin)

#define col3_offset  (COL3_GPIO_Port->IDR  &  COL3_Pin)

#define col4_offset  (COL4_GPIO_Port->IDR  &  COL4_Pin)

#define col5_offset  (COL5_GPIO_Port->IDR  &  COL5_Pin)

#define col6_offset  (COL6_GPIO_Port->IDR  &  COL6_Pin)

#define col7_offset  (COL7_GPIO_Port->IDR  &  COL7_Pin)

#define col8_offset  (COL8_GPIO_Port->IDR  &  COL8_Pin)

#define col9_offset  (COL9_GPIO_Port->IDR  &  COL9_Pin)

#define col10_offset (COL10_GPIO_Port->IDR & COL10_Pin)

#define col11_offset (COL11_GPIO_Port->IDR & COL11_Pin)

#define col12_offset (COL12_GPIO_Port->IDR & COL12_Pin)

#define col13_offset (COL13_GPIO_Port->IDR & COL13_Pin)

#define col14_offset (COL14_GPIO_Port->IDR & COL14_Pin)

#define col15_offset (COL15_GPIO_Port->IDR & COL15_Pin)

void fn_press_handle(uint8_t * buffer, uint8_t * media);

void fn_not_press_handle(uint8_t * buffer, uint8_t * media);

void fill_led_buffer(uint32_t color_rgb);

void set_led_buffer_at_location(uint8_t pos, uint32_t color_rgb);

#define LED_LOGIC_1 5
#define LED_LOGIC_0 3
#define MAX_brightness 0x7f
uint32_t calc_led_brightness(uint8_t brightness, uint32_t color_rgb);

typedef struct{
	uint8_t _is_pressed;
	uint8_t _led_single_breath_brightness;
	uint32_t _led_rgb;
}key;

typedef struct{
	key _key[83];
}map;

//static map _map[2];

static const uint8_t led_keymap[] = {
		12,     11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0, 82,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 81,
		40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 80,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,   53,   79,
		66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55,     54, 78,
		67, 68, 69, 70,         71,         72, 73, 74, 75, 76, 77
};

//static const uint8_t led_keymap[] = {
//		11,     10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,  0, 81,
//		12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 80,
//		39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 79,
//		40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,   52,   78,
//		65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54,     53, 77,
//		66, 67, 68, 69,         70,         71, 72, 73, 74, 75, 76
//};

//extern uint16_t led_buffer_length = 2;
extern uint8_t caps_lock_status;
//extern uint32_t led_buffer[led_buffer_length];

static const uint8_t keymap_1[] = {
		      Key_Esc, Key_MEDIA_MUTE,      Key_F1, Key_F2, Key_F3, Key_F4, Key_F5, Key_F6,     Key_F7,  Key_F8,        Key_F9,        Key_F10,        Key_F11,       Key_F12,   Key_Delete,
		    Key_Grave,          Key_1,       Key_2,  Key_3,  Key_4,  Key_5,  Key_6,  Key_7,      Key_8,   Key_9,         Key_0,      Key_Minus,      Key_Equal, Key_Backspace,   Key_Pageup,
		      Key_Tab,          Key_Q,       Key_W,  Key_E,  Key_R,  Key_T,  Key_Y,  Key_U,      Key_I,   Key_O,         Key_P,  Key_Leftbrace, Key_Rightbrace, Key_Backslash, Key_Pagedown,
		  Key_Caplock,          Key_A,       Key_S,  Key_D,  Key_F,  Key_G,  Key_H,  Key_J,      Key_K,   Key_L, Key_Semicolon, Key_Apostrophe,      Key_Enter,                    Key_Home,
		Modify_LShift,          Key_Z,       Key_X,  Key_C,  Key_V,  Key_B,  Key_N,  Key_M,  Key_Comma, Key_Dot,     Key_Slash,  Modify_RShift,         Key_Up,                     Key_End,
		 Modify_LCtrl,   Modify_LMeta, Modify_LAlt,              Key_Space,                         Modify_RAlt,             0,   Modify_RCtrl,       Key_Left,      Key_Down,    Key_Right
};

static const uint8_t keymap_2[] = {
		0, 0, 0, 0, 0, 0, 0, Key_MEDIA_PREVIOUSSONG, Key_MEDIA_PLAYPAUSE, Key_MEDIA_NEXTSONG, 0, 0, 0, 0,      Key_Insert,
		0, 0, 0, 0, 0, 0, 0,                      0,                   0,                  0, 0, 0, 0, 0, Key_Printscreen,
		0, 0, 0, 0, 0, 0, 0,                      0,                   0,                  0, 0, 0, 0, 0,     Key_Numlock,
		0, 0, 0, 0, 0, 0, 0,                      0,                   0,                  0, 0, 0, 0,     Key_Scrolllock,
		0, 0, 0, 0, 0, 0, 0,                      0,                   0,                  0, 0, 0, 0,                  0,
		0, 0, 0, 0,    0,                                                                  0, 0, 0, 0,                  0
};
