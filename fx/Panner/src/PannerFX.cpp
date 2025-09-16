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

bool PannerFX::process_main_L(float* main_frames_L, uint32_t frames) {
    for (uint32_t i = 0; i < frames; ++i) {
        float amped_signal = main_frames_L[i] * gain_;
        float panned_signal = amped_signal * pan_;
        main_frames_L[i] = clip1m1f(panned_signal);
    }
    return true;
}

bool PannerFX::process_main_R(float* main_frames_R, uint32_t frames) {
    for (uint32_t i = 0; i < frames; ++i) {
        float amped_signal = main_frames_R[i] * gain_;
        float panned_signal = amped_signal * (1 - pan_);
        main_frames_R[i] = clip1m1f(panned_signal);
    }
    return true;
}
