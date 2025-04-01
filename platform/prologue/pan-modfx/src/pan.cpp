#include "usermodfx.h"
#include "pan.hpp"


void MODFX_INIT(uint32_t platform, uint32_t api) {
    /*
        Called on instantiation of the effect. Use this callback to perform any required initializations. 
        See inc/userprg.h for possible values of platform and api.
    */
    return;
}
    

void MODFX_PROCESS(const float *main_xn, float *main_yn, const float *sub_xn,  float *sub_yn, uint32_t frames) {
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
   return;
}


void MODFX_PARAM(uint8_t index, uint32_t value) {
    /* 
        Called whenever the user changes a parameter value.
    */
   return;
}
