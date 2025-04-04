#pragma once

#include "usermodfx.h"
#include "math_util.hpp"


class PanFX{
public:
    void set_speed(const float speed);
    void set_depth(const float depth);
    float process_main_L(const float x);
    float process_main_R(const float x);
    float process_sub_R(const float x);
    float process_sub_L(const float x);

private:
    float process_L(const float x);
    float process_R(const float x);
    float pan_;
    float gain_;
};
