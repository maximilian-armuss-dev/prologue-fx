#include "RevDelFXWrapper.hpp"
#include "module_config.hpp"
#include "userrevfx.h"


static RevDelFXWrapper<FXClass, k_user_revfx_param_time, k_user_revfx_param_depth> revfx_wrapper;

void REVFX_INIT(uint32_t platform, uint32_t api) { revfx_wrapper.init(); }

void REVFX_PARAM(uint8_t index, int32_t value) { revfx_wrapper.set_param(index, value); }

void REVFX_PROCESS(float* xn, uint32_t frames) { revfx_wrapper.process_rdfx(xn, frames); }
