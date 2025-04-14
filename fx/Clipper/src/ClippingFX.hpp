#pragma once

#include "../../math_util.hpp"
#include "usermodfx.h"
#include "modes.hpp"


class ClippingFX {
    public:
        void set_speed(const float speed);
        void set_depth(const float depth);
        float process_main_L(const float x);
        float process_main_R(const float x);
        float process_sub_R(const float x);
        float process_sub_L(const float x);

    private:
        float process_signal(const float x);
        ShapeFn shape_fn_;
        float gain_;
        float gain_factor_ = 40.f;
};
