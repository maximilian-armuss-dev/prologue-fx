#pragma once

#include "usermodfx.h"
#include "modes.hpp"
#include "math_util.hpp"


void MODFX_INIT(uint32_t platform, uint32_t api);
void MODFX_PROCESS(const float *main_xn, float *main_yn,
                   const float *sub_xn,  float *sub_yn,
                   uint32_t frames);
void MODFX_PARAM(uint8_t fx_index, int32_t fx_value);

float gain;
ShapeFn shape_fn;


static inline float q31_to_normalized_f32(const int32_t param_value) {
    // Noaizes floats from a distribution [-1, 1] to [0, 1]
    const float value_f32 = q31_to_f32(param_value);
    return (value_f32 + 1.f) * 0.5f;
}


static inline ShapeFn q31_to_shape_fn(const int32_t param_value) {
    float normalized = q31_to_normalized_f32(param_value);
    int mode_index = static_cast<int>(normalized);
    int safe_mode_index = clamp<int>(0, mode_index, num_modes-1);
    Mode mode = static_cast<Mode>(safe_mode_index);
    ShapeFn shape_fn = get_shaper(mode);
    return shape_fn;
}


static inline float process_signal_mono(float x) {
    float in_signal = x * gain;
    float shaped_signal = shape_fn(in_signal);
    return clip1m1f(shaped_signal);
}
