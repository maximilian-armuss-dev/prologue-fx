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
    virtual bool process_main_L(float* main_frames_L, uint32_t frames) override;
    virtual bool process_main_R(float* main_frames_R, uint32_t frames) override;

private:
    ShapeFn shape_fn_;
    float gain_;

    const float gain_factor_ = 40.f;
    
    float process_signal(float x);
};
