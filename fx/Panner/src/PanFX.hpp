#pragma once

#include "float_math.h"
#include "FXBase.hpp"


class PanFX : public FXBase {
public:
    void set_speed(const float speed) override;
    void set_depth(const float depth) override;
    float process_main_L(const float x) override;
    float process_main_R(const float x) override;

private:
    float pan_;
    float gain_;
};
