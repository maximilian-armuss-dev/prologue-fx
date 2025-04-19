#pragma once

// Missing virtual destructor as it leads to compile errors
class FXBase {
public:
    virtual void set_speed(float speed) = 0;
    virtual void set_depth(float depth) = 0;
    virtual float process_main_L(float x) = 0;
    virtual float process_main_R(float x) = 0;
    virtual float process_sub_R(const float x) { return process_main_R(x); }
    virtual float process_sub_L(const float x) { return process_main_L(x); }
};