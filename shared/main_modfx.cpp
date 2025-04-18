#include "ModFXWrapper.hpp"
#include "module_config.hpp"
#include "usermodfx.h"


static ModFXWrapper<FXClass, k_user_modfx_param_time, k_user_modfx_param_depth> modfx_wrapper;

void MODFX_INIT(uint32_t, uint32_t) { modfx_wrapper.init(); }

void MODFX_PARAM(uint8_t index, int32_t value) { modfx_wrapper.set_param(index, value); }

void MODFX_PROCESS(const float* main_xn, float* main_yn,
                   const float* sub_xn, float* sub_yn,
                   uint32_t frames) { modfx_wrapper.process_mfx(main_xn, main_yn, sub_xn, sub_yn, frames); }
