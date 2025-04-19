#pragma once

#include "float_math.h"
#include "FXBase.hpp"


class PanFX : public FXBase {
public:
    void set_speed(float speed) override;
    void set_depth(float depth) override;
    float process_main_L(float x) override;
    float process_main_R(float x) override;

private:
    float pan_ = 0.5f;
    float gain_ = 0.5f;
};
