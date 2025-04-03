#pragma once

#include "shapers.hpp"

using ShapeFn = float(*)(float);

enum class Mode {
    TANH,
    ATAN,
    FOLD,
    FUZZ,
    count
};

constexpr int num_modes = static_cast<int>(Mode::count);

ShapeFn get_shaper(Mode mode) {
    switch (mode) {
        case Mode::TANH: return clip_tanh;
        case Mode::ATAN: return clip_atan;
        default: return identity;
    }
}