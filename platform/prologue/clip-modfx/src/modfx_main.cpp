#include "modfx_main.hpp"


void MODFX_INIT(uint32_t platform, uint32_t api) {
    gain = 1.f;
    shape_fn = identity;
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
        *(main_buffer_out++) = process_signal_mono(*(main_buffer_in++));
        *(main_buffer_out++) = process_signal_mono(*(main_buffer_in++));
        *(sub_buffer_out++) = process_signal_mono(*(sub_buffer_in++));
        *(sub_buffer_out++) = process_signal_mono(*(sub_buffer_in++));
    }
}


void MODFX_PARAM(uint8_t index, int32_t value) {
    switch (index) {
        case k_user_modfx_param_time:
            shape_fn = q31_to_shape_fn(value);
            break;
        case k_user_modfx_param_depth:
            gain = q31_to_normalized_f32(value);
            break;
        default:
            break;
    }
}
