#pragma once

#include "usermodfx.h"
#include "funcs.hpp"

enum class Mode {
    CLIP,
    TANH,
    FOLD,
    FUZZ,
    count
};


void MODFX_INIT(uint32_t platform, uint32_t api);
void MODFX_PROCESS(const float *main_xn, float *main_yn,
                   const float *sub_xn,  float *sub_yn,
                   uint32_t frames);
void MODFX_PARAM(uint8_t fx_index, int32_t fx_value);

float depth_gain;
uint8_t speed_mode;
uint8_t num_modes;

