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
#include <zephyr/drivers/adc.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(kb_adc, LOG_LEVEL_INF);

#include "../inc/kb_adc.h"

// #if !DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
// #error "No sample io-channels specified"
// #endif

// #define ADC_NODE DT_PHANDLE(DT_PATH(zephyr_user), io_channels)
// static const struct adc_dt_spec adc_channel = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));

// int8_t kb_adc_init(){
//   int8_t ret = 0;
//   int16_t buf;
//   struct adc_sequence sequence = {
//       .buffer = &buf,
//       .buffer_size = sizeof(buf),
//   };

//   LOG_ERR("ADC reading example\n");

//   ret = adc_is_ready_dt(&adc_channel);
//   if (!ret) {
//       LOG_ERR("ADC controller device not ready\n");
//       return 0;
//   }

//   ret = adc_channel_setup_dt(&adc_channel);
//   if (ret < 0) {
//       LOG_ERR("Could not setup channel (err %d)\n", ret);
//       return 0;
//   }

//   ret = adc_sequence_init_dt(&adc_channel, &sequence);
//   if (ret < 0) {
//       LOG_ERR("Could not initialize sequence (err %d)\n", ret);
//       return 0;
//   }

//   return ret;
// }