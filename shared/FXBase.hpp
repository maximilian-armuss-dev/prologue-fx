#pragma once

class FXBase {
public:
    virtual ~FXBase() = default;
    virtual void set_speed(const float speed) = 0;
    virtual void set_depth(const float depth) = 0;
    virtual float process_main_L(const float x) = 0;
    virtual float process_main_R(const float x) = 0;
    virtual float process_sub_R(const float x) { return process_main_R(x); }
    virtual float process_sub_L(const float x) { return process_main_L(x); }
};