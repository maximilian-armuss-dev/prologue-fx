#pragma once

#include "usermodfx.h"
#include "fixed_math.h"


template<typename T>
class FXWrapper {
    public:
        void init() {
            dsp.set_speed(0.5f);
            dsp.set_depth(0.5f);
        }

        float q31_to_normalized_f32(const int32_t param_value) {
            // Normalizes floats from a distribution [-1, 1] to [0, 1]
            const float value_f32 = q31_to_f32(param_value);
            return (value_f32 + 1.f) * 0.5f;
        }

        void set_param(uint8_t index, int32_t value) {
            float f32_val = q31_to_f32(value);
            switch (index) {
                case k_user_modfx_param_time: dsp.set_speed(f32_val); break;
                case k_user_modfx_param_depth: dsp.set_depth(f32_val); break;
                default: break;
            }
        }

        void process(const float *main_xn, float *main_yn,
                     const float *sub_xn,  float *sub_yn,
                     uint32_t frames) {

            const float * main_buffer_in = main_xn;
            float * __restrict main_buffer_out = main_yn;
            const float * sub_buffer_in = sub_xn;
            float * __restrict sub_buffer_out = sub_yn;

            // interleaved stereo: 1 frame holds both L & R samples subsequently in memory ( L0 R0 L1 R1 ... )
            const float * endptr = main_buffer_out + 2 * frames;

            for (; main_buffer_out < endptr;) {
                *(main_buffer_out++) = dsp.process_main_L(*(main_buffer_in++));
                *(main_buffer_out++) = dsp.process_main_R(*(main_buffer_in++));
                *(sub_buffer_out++) = dsp.process_sub_L(*(sub_buffer_in++));
                *(sub_buffer_out++) = dsp.process_sub_R(*(sub_buffer_in++));
            }
        }

    private:
        T dsp;
};
