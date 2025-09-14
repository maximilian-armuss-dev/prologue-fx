#include "PanFX.hpp"


void PanFX::set_speed(const float speed) { pan_ = speed; }

void PanFX::set_depth(const float depth) { gain_ = 2 * depth; }

float PanFX::process_main_L(const float x) {
    float amped_signal = x * gain_;
    float panned_signal = amped_signal * pan_;
    return clip1m1f(panned_signal);
}

float PanFX::process_main_R(const float x) {
    float amped_signal = x * gain_;
    float panned_signal = amped_signal * (1 - pan_);
    return clip1m1f(panned_signal);
}
