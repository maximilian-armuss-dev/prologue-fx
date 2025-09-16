#pragma once

// Missing virtual destructor as it leads to compile errors
class FXBase {
public:
    virtual void init() = 0;
    virtual void reset() = 0;
    virtual void set_speed(float speed) = 0;
    virtual void set_depth(float depth) = 0;
    virtual bool process_main_L(float* main_frames_L, uint32_t frames) = 0;
    virtual bool process_main_R(float* main_frames_R, uint32_t frames) = 0;
    virtual bool process_sub_L(float* sub_frames_L, uint32_t frames) { return process_main_L(sub_frames_L, frames); }
    virtual bool process_sub_R(float* sub_frames_R, uint32_t frames) { return process_main_R(sub_frames_R, frames); }
};