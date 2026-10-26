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

#include "../inc/keymap_lut.h"

struct key_mapping_t keymap[ARRAY_SIZE(kb_rows)][ARRAY_SIZE(kb_cols) + 2] = {
  /* Row 0 */
  {
    { { HID_KEY_ESC       , 0                   }, { 1, 0 }, 12,  0 , 0 , 0, NULL, NULL },
    { { 0xe2              , 0                   }, { 2, 0 }, -1, -1 , -1, 0, NULL, NULL },
    { { HID_KEY_F1        , 0                   }, { 1, 0 }, 11, 20 , 0 , 0, NULL, NULL },
    { { HID_KEY_F2        , 0                   }, { 1, 0 }, 10, 28 , 0 , 0, NULL, NULL },
    { { HID_KEY_F3        , 0                   }, { 1, 0 }, 9 , 36 , 0 , 0, NULL, NULL },
    { { HID_KEY_F4        , 0                   }, { 1, 0 }, 8 , 44 , 0 , 0, NULL, NULL },
    { { HID_KEY_F5        , 0                   }, { 1, 0 }, 7 , 54 , 0 , 0, NULL, NULL },
    { { HID_KEY_F6        , 0xb6                }, { 1, 2 }, 6 , 62 , 0 , 0, NULL, NULL },
    { { HID_KEY_F7        , 0xcd                }, { 1, 2 }, 5 , 70 , 0 , 0, NULL, NULL },
    { { HID_KEY_F8        , 0xb5                }, { 1, 2 }, 4 , 78 , 0 , 0, NULL, NULL },
    { { 0                 , 0                   }, { 0, 0 }, -1, -1 , -1, 0, NULL, NULL },
    { { HID_KEY_F9        , 0                   }, { 1, 0 }, 3 , 88 , 0 , 0, NULL, NULL },
    { { HID_KEY_F10       , 0                   }, { 1, 0 }, 2 , 96 , 0 , 0, NULL, NULL },
    { { HID_KEY_F11       , 0                   }, { 1, 0 }, 1 , 104, 0 , 0, NULL, NULL },
    { { HID_KEY_F12       , 0                   }, { 1, 0 }, 0 , 112, 0 , 0, NULL, NULL },
    { { HID_KEY_DELETE    , HID_KEY_INSERT      }, { 1, 1 }, 81, 124, 0 , 0, NULL, NULL }
  },
  /* Row 1 */
  {
    { { HID_KEY_GRAVE     , 0                   }, { 1, 0 }, 13, 0  , 8 , 0, NULL, NULL },
    { { HID_KEY_1         , 0                   }, { 1, 0 }, 14, 8  , 8 , 0, NULL, NULL },
    { { HID_KEY_2         , 0                   }, { 1, 0 }, 15, 16 , 8 , 0, NULL, NULL },
    { { HID_KEY_3         , 0                   }, { 1, 0 }, 16, 24 , 8 , 0, NULL, NULL },
    { { HID_KEY_4         , 0                   }, { 1, 0 }, 17, 32 , 8 , 0, NULL, NULL },
    { { HID_KEY_5         , 0                   }, { 1, 0 }, 18, 40 , 8 , 0, NULL, NULL },
    { { HID_KEY_6         , 0                   }, { 1, 0 }, 19, 48 , 8 , 0, NULL, NULL },
    { { HID_KEY_7         , 0                   }, { 1, 0 }, 20, 56 , 8 , 0, NULL, NULL },
    { { HID_KEY_8         , 0                   }, { 1, 0 }, 21, 64 , 8 , 0, NULL, NULL },
    { { HID_KEY_9         , 0                   }, { 1, 0 }, 22, 72 , 8 , 0, NULL, NULL },
    { { HID_KEY_0         , 0                   }, { 1, 0 }, 23, 80 , 8 , 0, NULL, NULL },
    { { HID_KEY_MINUS     , 0                   }, { 1, 0 }, 24, 88 , 8 , 0, NULL, NULL },
    { { HID_KEY_EQUAL     , 0                   }, { 1, 0 }, 25, 96 , 8 , 0, NULL, NULL },
    { { HID_KEY_BACKSPACE , 0                   }, { 1, 0 }, 26, 108, 8 , 0, NULL, NULL },
    { { 0                 , 0                   }, { 0, 0 }, -1, -1 , -1, 0, NULL, NULL },
    { { HID_KEY_PAGEUP    , HID_KEY_SYSRQ       }, { 1, 1 }, 80, 124, 8 , 0, NULL, NULL }
  },
  /* Row 2 */
  {
    { { HID_KEY_TAB       , 0                   }, { 1, 0 }, 40, 2  , 16, 0, NULL, NULL },
    { { HID_KEY_Q         , 0                   }, { 1, 0 }, 39, 12 , 16, 0, NULL, NULL },
    { { HID_KEY_W         , 0                   }, { 1, 0 }, 38, 20 , 16, 0, NULL, NULL },
    { { HID_KEY_E         , 0                   }, { 1, 0 }, 37, 28 , 16, 0, NULL, NULL },
    { { HID_KEY_R         , 0                   }, { 1, 0 }, 36, 36 , 16, 0, NULL, NULL },
    { { HID_KEY_T         , 0                   }, { 1, 0 }, 35, 44 , 16, 0, NULL, NULL },
    { { HID_KEY_Y         , 0                   }, { 1, 0 }, 34, 52 , 16, 0, NULL, NULL },
    { { HID_KEY_U         , 0                   }, { 1, 0 }, 33, 60 , 16, 0, NULL, NULL },
    { { HID_KEY_I         , 0                   }, { 1, 0 }, 32, 68 , 16, 0, NULL, NULL },
    { { HID_KEY_O         , 0                   }, { 1, 0 }, 31, 76 , 16, 0, NULL, NULL },
    { { HID_KEY_P         , 0                   }, { 1, 0 }, 30, 84 , 16, 0, NULL, NULL },
    { { HID_KEY_LEFTBRACE , 0                   }, { 1, 0 }, 29, 92 , 16, 0, NULL, NULL },
    { { HID_KEY_RIGHTBRACE, 0                   }, { 1, 0 }, 28, 100, 16, 0, NULL, NULL },
    { { HID_KEY_BACKSLASH , 0                   }, { 1, 0 }, 27, 110, 16, 0, NULL, NULL },
    { { 0                 , 0                   }, { 0, 0 }, -1, -1 , -1, 0, NULL, NULL },
    { { HID_KEY_PAGEDOWN  , HID_KEY_NUMLOCK     }, { 1, 1 }, 79, 124, 16, 0, NULL, NULL }
  },
  /* Row 3 */
  {
    { { HID_KEY_CAPSLOCK  , 0                   }, { 1, 0 }, 41, 3  , 24, 0, NULL, NULL },
    { { HID_KEY_A         , 0                   }, { 1, 0 }, 42, 14 , 24, 0, NULL, NULL },
    { { HID_KEY_S         , 0                   }, { 1, 0 }, 43, 22 , 24, 0, NULL, NULL },
    { { HID_KEY_D         , 0                   }, { 1, 0 }, 44, 30 , 24, 0, NULL, NULL },
    { { HID_KEY_F         , 0                   }, { 1, 0 }, 45, 38 , 24, 0, NULL, NULL },
    { { HID_KEY_G         , 0                   }, { 1, 0 }, 46, 46 , 24, 0, NULL, NULL },
    { { HID_KEY_H         , 0                   }, { 1, 0 }, 47, 54 , 24, 0, NULL, NULL },
    { { HID_KEY_J         , 0                   }, { 1, 0 }, 48, 62 , 24, 0, NULL, NULL },
    { { HID_KEY_K         , 0                   }, { 1, 0 }, 49, 70 , 24, 0, NULL, NULL },
    { { HID_KEY_L         , 0                   }, { 1, 0 }, 50, 78 , 24, 0, NULL, NULL },
    { { HID_KEY_SEMICOLON , 0                   }, { 1, 0 }, 51, 86 , 24, 0, NULL, NULL },
    { { HID_KEY_APOSTROPHE, 0                   }, { 1, 0 }, 52, 94 , 24, 0, NULL, NULL },
    { { 0                 , 0                   }, { 0, 0 }, -1, -1 , -1, 0, NULL, NULL },
    { { HID_KEY_ENTER     , 0                   }, { 1, 0 }, 53, 107, 24, 0, NULL, NULL },
    { { 0                 , 0                   }, { 0, 0 }, -1, -1 , -1, 0, NULL, NULL },
    { { HID_KEY_HOME      , HID_KEY_SCROLLLOCK  }, { 1, 1 }, 78, 124, 24, 0, NULL, NULL }
  },
  /* Row 4 */
  {
    { { 0xe1              , 0                   }, { 1, 0 }, 66, 5  , 32, 0, NULL, NULL },
    { { HID_KEY_Z         , 0                   }, { 1, 0 }, 65, 18 , 32, 0, NULL, NULL },
    { { HID_KEY_X         , 0                   }, { 1, 0 }, 64, 26 , 32, 0, NULL, NULL },
    { { HID_KEY_C         , 0                   }, { 1, 0 }, 63, 34 , 32, 0, NULL, NULL },
    { { HID_KEY_V         , 0                   }, { 1, 0 }, 62, 42 , 32, 0, NULL, NULL },
    { { HID_KEY_B         , 0                   }, { 1, 0 }, 61, 50 , 32, 0, NULL, NULL },
    { { HID_KEY_N         , 0                   }, { 1, 0 }, 60, 58 , 32, 0, NULL, NULL },
    { { HID_KEY_M         , 0                   }, { 1, 0 }, 59, 66 , 32, 0, NULL, NULL },
    { { HID_KEY_COMMA     , 0                   }, { 1, 0 }, 58, 74 , 32, 0, NULL, NULL },
    { { HID_KEY_DOT       , 0                   }, { 1, 0 }, 57, 82 , 32, 0, NULL, NULL },
    { { HID_KEY_SLASH     , 0                   }, { 1, 0 }, 56, 90 , 32, 0, NULL, NULL },
    { { 0                 , 0                   }, { 0, 0 }, -1, -1 , -1, 0, NULL, NULL },
    { { 0xe5              , 0                   }, { 1, 0 }, 55, 101, 32, 0, NULL, NULL },
    { { 0                 , 0                   }, { 0, 0 }, -1, -1 , -1, 0, NULL, NULL },
    { { HID_KEY_UP        , 0                   }, { 1, 0 }, 54, 114, 32, 0, NULL, NULL },
    { { HID_KEY_END       , 0                   }, { 1, 0 }, 77, 124, 32, 0, NULL, NULL }
  },
  /* Row 5 */
  {
    { { 0xe0              , 0                   }, { 1, 0 }, 67, 1  , 40, 0, NULL, NULL },
    { { 0xe3              , 0                   }, { 1, 0 }, 68, 11 , 40, 0, NULL, NULL },
    { { 0xe2              , 0                   }, { 1, 0 }, 69, 21 , 40, 0, NULL, NULL },
    { { 0                 , 0                   }, { 0, 0 }, -1, -1 , -1, 0, NULL, NULL },
    { { 0                 , 0                   }, { 0, 0 }, -1, -1 , -1, 0, NULL, NULL },
    { { HID_KEY_SPACE     , 0                   }, { 1, 0 }, 70, 51 , 40, 0, NULL, NULL },
    { { 0                 , 0                   }, { 0, 0 }, -1, -1 , -1, 0, NULL, NULL },
    { { 0                 , 0                   }, { 0, 0 }, -1, -1 , -1, 0, NULL, NULL },
    { { 0                 , 0                   }, { 0, 0 }, -1, -1 , -1, 0, NULL, NULL },
    { { 0xe6              , 0                   }, { 1, 0 }, 71, 80 , 40, 0, NULL, NULL },
    { { 0                 , 0                   }, { 0, 0 }, 72, 88 , 40, 0, NULL, NULL },
    { { 0xe4              , 0                   }, { 1, 0 }, 73, 96 , 40, 0, NULL, NULL },
    { { 0                 , 0                   }, { 0, 0 }, -1, -1 , -1, 0, NULL, NULL },
    { { HID_KEY_LEFT      , 0                   }, { 1, 0 }, 74, 106, 40, 0, NULL, NULL },
    { { HID_KEY_DOWN      , 0                   }, { 1, 0 }, 75, 114, 40, 0, NULL, NULL },
    { { HID_KEY_RIGHT     , 0                   }, { 1, 0 }, 76, 122, 40, 0, NULL, NULL }
  }
};