#pragma once

#include "float_math.h"
#include "usermodfx.h"
#include "FXBase.hpp"


class MoogFilterFX : public FXBase {
public:
    void init() override;
    void reset() override;
    void set_speed(float speed) override;
    void set_depth(float depth) override;
    float process_main_L(float x) override;
    float process_main_R(float x) override;

private:
    float speed_;
    float depth_;
    float filter_coefficient_;
    float filter_stages_L_[4];
    float filter_stages_R_[4];
    float last_sample_L_;
    float last_sample_R_;

    // Couldn't find 'k_samplerate' in usermodfx.h, but at least in osc_api.h, where it's set to 48kHz
    const float samplerate_ = 48000.0f; 
    const float min_freq_ = 20.0f;
    const float max_freq_ = 20000.0f;
    const float resonance_ = 3.5f;

    float process_filter(float input, float stages[4], float &delay) const;
};
