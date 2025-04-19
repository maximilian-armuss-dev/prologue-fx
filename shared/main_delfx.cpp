#include "RevDelFXWrapper.hpp"
#include "module_config.hpp"
#include "userdelfx.h"


static RevDelFXWrapper<FXClass, k_user_delfx_param_time, k_user_delfx_param_depth> delfx_wrapper;

void DELFX_INIT(uint32_t platform, uint32_t api) { delfx_wrapper.init(); }

void DELFX_PARAM(uint8_t index, int32_t value) { delfx_wrapper.set_param(index, value); }

void DELFX_PROCESS(float* xn, uint32_t frames) { delfx_wrapper.process_rdfx(xn, frames); }
