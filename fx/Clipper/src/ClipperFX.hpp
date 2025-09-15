#pragma once

#include "../../math_util.hpp"
#include "usermodfx.h"
#include "modes.hpp"
#include "FXBase.hpp"


class ClipperFX : public FXBase {
public:
    void init() override;
    void reset() override;
    void set_speed(float speed) override;
    void set_depth(float depth) override;
    float process_main_L(float x) override;
    float process_main_R(float x) override;

private:
    ShapeFn shape_fn_;
    float gain_;

    const float gain_factor_ = 40.f;
    
    float process_signal(float x);
};
