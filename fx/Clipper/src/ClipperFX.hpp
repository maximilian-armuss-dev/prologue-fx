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
    void process_main_L(float* main_frames_L, uint32_t frame_count) override;
    void process_main_R(float* main_frames_R, uint32_t frame_count) override;

private:
    ShapeFn shape_fn_;
    float gain_;

    const float gain_factor_ = 40.f;
    
    void process_signal(float* main_frames, uint32_t frame_count);
};
