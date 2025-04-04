#include "FXWrapper.hpp"
#include "module_config.hpp"


static FXWrapper<FXClass> fx_wrapper;

void MODFX_INIT(uint32_t, uint32_t) { fx_wrapper.init(); }

void MODFX_PARAM(uint8_t index, int32_t value) { fx_wrapper.set_param(index, value); }

void MODFX_PROCESS(const float* main_xn, float* main_yn,
                   const float* sub_xn, float* sub_yn,
                   uint32_t frames) { fx_wrapper.process(main_xn, main_yn, sub_xn, sub_yn, frames); }
