#include "RevDelFXWrapper.hpp"
#include "module_config.hpp"
#include "userdelfx.h"


static RevDelFXWrapper<FXClass, k_user_delfx_param_time, k_user_delfx_param_depth> delfx_wrapper;

void DELFX_INIT(uint32_t, uint32_t) { delfx_wrapper.init(); }

void DELFX_PARAM(uint8_t index, int32_t value) { delfx_wrapper.set_param(index, value); }

void DELFX_PROCESS(float* main_xn, uint32_t frames) { delfx_wrapper.process_rdfx(main_xn, frames); }
