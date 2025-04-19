#pragma once

#include "../../math_util.hpp"
#include "usermodfx.h"
#include "modes.hpp"
#include "FXBase.hpp"


class ClippingFX : public FXBase {
    public:
        void set_speed(float speed) override;
        void set_depth(float depth) override;
        float process_main_L(float x) override;
        float process_main_R(float x) override;

    private:
        float process_signal(float x);
        ShapeFn shape_fn_ = identity;
        float gain_ = 0.5f;
        float gain_factor_ = 40.f;
};
