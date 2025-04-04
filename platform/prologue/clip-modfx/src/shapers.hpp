#pragma once

#include "float_math.h"


static inline float identity(const float x) { return x; }
static inline float clip_tanh(const float x) { return fastertanhf(x); }
static inline float clip_atan(const float x) { return x / (1.0f + 0.28f * x * x); }
static inline float clip_sig(const float x) { return (2.0f / 1 + fasterexpf(-x)) - 1; }
static inline float clip_sin_abs(const float x) { return si_fabsf(x) * fastersinf(M_PI_2 * x); }
