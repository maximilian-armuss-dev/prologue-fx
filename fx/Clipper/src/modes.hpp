#pragma once

#include "shapers.hpp"

using ShapeFn = float(*)(float);

// Workaround, maps and other heap-based structures are too slow for embedded hardware

enum class Mode {
    TANH,
    ATAN,
    SIG,
    SIN,
    count
};

constexpr int NUM_MODES = static_cast<int>(Mode::count);

static inline ShapeFn get_shaper(Mode mode) {
    switch (mode) {
        case Mode::TANH: return clip_tanh;
        case Mode::ATAN: return clip_atan;
        case Mode::SIG: return clip_sig;
        case Mode::SIN: return clip_sin_abs;
        default: return identity;
    }
}
