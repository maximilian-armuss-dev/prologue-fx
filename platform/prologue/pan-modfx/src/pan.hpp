#pragma once

#include "usermodfx.h"


void MODFX_INIT(uint32_t platform, uint32_t api);
void MODFX_PROCESS(const float *main_buffer_in, float *main_buffer_out,
                   const float *sub_buffer_in,  float *sub_buffer_out,
                   uint32_t frame_count);
void MODFX_PARAM(uint8_t fx_index, int32_t fx_value);

float depth_gain;
float speed_pan;
