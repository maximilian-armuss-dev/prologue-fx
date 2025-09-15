#include "ClipperFX.hpp"


void ClipperFX::init(){
    shape_fn_ = identity;
    gain_ = 0.5f;
}

void ClipperFX::reset(){
    init();
}

void ClipperFX::set_speed(const float speed)  {
    float mode_f32 = speed * (NUM_MODES-1);
    int mode_index = static_cast<int>(mode_f32);
    int safe_mode_index = clamp<int>(0, mode_index, NUM_MODES-1);
    Mode mode = static_cast<Mode>(safe_mode_index);
    shape_fn_ = get_shaper(mode);
}

void ClipperFX::set_depth(const float depth)  {
    gain_ = gain_factor_ * depth;
}

float ClipperFX::process_signal(const float x) {
    float in_signal = x * gain_;
    float shaped_signal = shape_fn_(in_signal) / gain_factor_;
    return clip1m1f(shaped_signal);
}

float ClipperFX::process_main_L(const float x) { return process_signal(x); }

float ClipperFX::process_main_R(const float x) { return process_signal(x); }
