#pragma once

#include "float_math.h"


static inline float identity(float x) { return x; }
static inline float clip_tanh(float x) { return fastertanhf(x); }
static inline float clip_atan(float x) { return x / (1.0f + 0.28f * x * x); }
