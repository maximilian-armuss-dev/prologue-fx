#include "pan.hpp"

void MODFX_INIT(uint32_t platform, uint32_t api) {
    /*
        Called on instantiation of the effect. Use this callback to perform any required initializations.
        See inc/userprg.h for possible values of platform and api.
    */
    depth_gain = 1.f;
    speed_pan = 0.5f;
}


void MODFX_PROCESS(const float *main_xn, float *main_yn,
                   const float *sub_xn,  float *sub_yn,
                   uint32_t frames) {
    /*
        This is where you should process the input samples.
        This function is called every time a buffer of frames samples is required (1 sample per frame).
        *_xn buffers denote inputs and *_yn denote output buffers, where you should write your results.

        Note: There are main_ and sub_ versions of the inputs and outputs in order to support the dual timbre feature of the prologue.
        On the prologue, both main and sub should be processed the same way in parallel.
        (On other non-multitimbral platforms you can ignore sub_xn and sub_yn.)

        Note: Buffer lengths up to 64 frames should be supported.
        However you can perform multiple passes on smaller buffers if you prefer.
        (Optimize for powers of two: 16, 32, 64)
    */
    const float * main_buffer_in = main_xn;
    float * __restrict main_buffer_out = main_yn;
    const float * sub_buffer_in = sub_xn;
    float * __restrict sub_buffer_out = sub_yn;

    // interleaved stereo: 1 frame holds both L & R samples subsequently in memory ( L0 R0 L1 R1 ... )
    const float * endptr = main_buffer_out + 2 * frames;

    for (; main_buffer_out < endptr;) {
        *(main_buffer_out++) = *(main_buffer_in++) * depth_gain * (1.f - speed_pan);
        *(main_buffer_out++) = *(main_buffer_in++) * depth_gain * speed_pan;
        *(sub_buffer_out++) = *(sub_buffer_in++) * depth_gain * (1.f - speed_pan);
        *(sub_buffer_out++) = *(sub_buffer_in++) * depth_gain * speed_pan;
    }
}


void MODFX_PARAM(uint8_t index, int32_t value) {
    const float value_f32 = q31_to_f32(value);
    switch (index) {
        case k_user_modfx_param_time:
            speed_pan = value_f32;
            break;
        case k_user_modfx_param_depth:
            depth_gain = value_f32;
            break;
        default:
            break;
    }
}
