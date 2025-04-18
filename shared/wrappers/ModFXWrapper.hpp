#pragma once

#include "FXWrapper.hpp"


template<typename T, int TimeID, int DepthID>
class ModFXWrapper : public FXWrapper<T, TimeID, DepthID> {
public:

    void process_mfx(const float *main_xn, float *main_yn,
                     const float *sub_xn,  float *sub_yn,
                     uint32_t frames) {

        const float * main_buffer_in = main_xn;
        float * __restrict main_buffer_out = main_yn;
        const float * sub_buffer_in = sub_xn;
        float * __restrict sub_buffer_out = sub_yn;

        // interleaved stereo: 1 frame holds both L & R samples subsequently in memory ( L0 R0 L1 R1 ... )
        const float * endptr = main_buffer_out + 2 * frames;

        for (; main_buffer_out < endptr;) {
            *(main_buffer_out++) = this->dsp.process_main_L(*(main_buffer_in++));
            *(main_buffer_out++) = this->dsp.process_main_R(*(main_buffer_in++));
            *(sub_buffer_out++) = this->dsp.process_sub_L(*(sub_buffer_in++));
            *(sub_buffer_out++) = this->dsp.process_sub_R(*(sub_buffer_in++));
        }
    }

};
