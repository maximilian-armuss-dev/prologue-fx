#include "RevDelFXWrapper.hpp"
#include "module_config.hpp"
#include "userrevfx.h"


static RevDelFXWrapper<FXClass, k_user_revfx_param_time, k_user_revfx_param_depth> revfx_wrapper;

void DELFX_INIT(uint32_t, uint32_t) { revfx_wrapper.init(); }

void DELFX_PARAM(uint8_t index, int32_t value) { revfx_wrapper.set_param(index, value); }

void DELFX_PROCESS(float* main_xn, uint32_t frames) { revfx_wrapper.process_rdfx(main_xn, frames); }
