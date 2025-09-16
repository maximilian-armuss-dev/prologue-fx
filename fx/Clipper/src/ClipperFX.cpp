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

void ClipperFX::process_signal(float* main_frames, uint32_t frame_count) {
    for (uint32_t i = 0; i < frame_count; ++i) {
        float in_signal = main_frames[i] * gain_;
        float shaped_signal = shape_fn_(in_signal) / gain_factor_;
        main_frames[i] = clip1m1f(shaped_signal);
    }
}

void ClipperFX::process_main_L(float* main_frames_L, uint32_t frame_count) {
    process_signal(main_frames_L, frame_count);
}

void ClipperFX::process_main_R(float* main_frames_R, uint32_t frame_count) {
    process_signal(main_frames_R, frame_count);
}
