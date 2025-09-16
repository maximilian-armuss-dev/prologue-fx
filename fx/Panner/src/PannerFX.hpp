#pragma once

#include "float_math.h"
#include "FXBase.hpp"


class PannerFX : public FXBase {
public:
    void init() override;
    void reset() override;
    void set_speed(float speed) override;
    void set_depth(float depth) override;
    void process_main_L(float* main_frames_L, uint32_t frame_count) override;
    void process_main_R(float* main_frames_R, uint32_t frame_count) override;

private:
    float pan_ = 0.5f;
    float gain_ = 0.5f;
};
