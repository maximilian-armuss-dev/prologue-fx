#pragma once

#include <type_traits>
#include "fixed_math.h"
#include "FXBase.hpp"


template<typename T, int TimeID, int DepthID>
class FXWrapper {
    static_assert(std::is_base_of<FXBase, T>::value,
                  "FXClass must inherit from FXBase");
public:
    void init() {
        dsp.set_speed(0.5f);
        dsp.set_depth(0.5f);
    }

    float q31_to_normalized_f32(const int32_t param_value) {
        // Normalizes floats from a distribution [-1, 1] to [0, 1]
        const float value_f32 = q31_to_f32(param_value);
        return (value_f32 + 1.f) * 0.5f;
    }

    void set_param(uint8_t index, int32_t value) {
        float f32_val = q31_to_f32(value);
        switch (index) {
            case TimeID: dsp.set_speed(f32_val); break;
            case DepthID: dsp.set_depth(f32_val); break;
            default: break;
        }
    }

protected:
    T dsp;
};
