#include "PannerFX.hpp"

void PannerFX::init(){
    pan_ = 0.5f;
    gain_ = 0.5f;
}

void PannerFX::reset(){
    init();
}

void PannerFX::set_speed(const float speed) { pan_ = speed; }

void PannerFX::set_depth(const float depth) { gain_ = 2 * depth; }

float PannerFX::process_main_L(const float x) {
    float amped_signal = x * gain_;
    float panned_signal = amped_signal * pan_;
    return clip1m1f(panned_signal);
}

float PannerFX::process_main_R(const float x) {
    float amped_signal = x * gain_;
    float panned_signal = amped_signal * (1 - pan_);
    return clip1m1f(panned_signal);
}
