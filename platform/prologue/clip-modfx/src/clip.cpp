#include "clip.hpp"

using ShaperFunction = float(*)(float);
constexpr ShaperFunction shaper_functions[] = {
    fastertanhf,
    clip_atan,
};


uint8_t param_val_to_mode(int32_t val) {
    float normalized = (val + 1.0f) * 0.5f * num_modes;
    uint8_t mode = static_cast<int>(normalized);
    mode = clampfsel(0, mode, num_modes);
    return mode;
}


float clip_signal(const float x){
    float in_signal = x * depth_gain;
    float shaped_signal = shaper_functions[speed_mode](in_signal);
    float clipped_signal = clip1m1f(shaped_signal);
    return clipped_signal;
}


void MODFX_INIT(uint32_t platform, uint32_t api) {
    depth_gain = 1.f;
    speed_mode = 0;
    num_modes = static_cast<int>(Mode::count);
}


void MODFX_PROCESS(const float *main_xn, float *main_yn,
                   const float *sub_xn,  float *sub_yn,
                   uint32_t frames) {

    const float * main_buffer_in = main_xn;
    float * __restrict main_buffer_out = main_yn;
    const float * sub_buffer_in = sub_xn;
    float * __restrict sub_buffer_out = sub_yn;

    // interleaved stereo: 1 frame holds both L & R samples subsequently in memory ( L0 R0 L1 R1 ... )
    const float * endptr = main_buffer_out + 2 * frames;

    for (; main_buffer_out < endptr;) {
        *(main_buffer_out++) = clip_signal(*(main_buffer_in++));
        *(main_buffer_out++) = clip_signal(*(main_buffer_in++));
        *(sub_buffer_out++) = clip_signal(*(sub_buffer_in++));
        *(sub_buffer_out++) = clip_signal(*(sub_buffer_in++));
    }
}


void MODFX_PARAM(uint8_t index, int32_t value) {
    const float value_f32 = q31_to_f32(value);
    switch (index) {
        case k_user_modfx_param_time:
            speed_mode = param_val_to_mode(value_f32);
            break;
        case k_user_modfx_param_depth:
            depth_gain = value_f32;
            break;
        default:
            break;
    }
}
