#pragma once

#include "FXWrapper.hpp"


template<typename T, int TimeID, int DepthID>
class RevDelFXWrapper : public FXWrapper<T, TimeID, DepthID> {
public:

    void process_rdfx(float *xn, uint32_t frames) {
        const float * main_buffer_in = xn;
        float * __restrict main_buffer_out = xn;

        // interleaved stereo: 1 frame holds both L & R samples subsequently in memory ( L0 R0 L1 R1 ... )
        const float* endptr = main_buffer_in + frames * 2;

        for (; main_buffer_out < endptr;) {
            *(main_buffer_out++) = this->dsp.process_main_L(*(main_buffer_in++));
            *(main_buffer_out++) = this->dsp.process_main_R(*(main_buffer_in++));
        }
    }
    
};
