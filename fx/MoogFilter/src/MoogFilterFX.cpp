#include "MoogFilterFX.hpp"


MoogFilterFX::MoogFilterFX() {
    for (int i = 0; i < 4; ++i) {
        filter_stages_L_[i] = 0.0f;
        filter_stages_R_[i] = 0.0f;
    }
    last_sample_L_ = 0.0f;
    last_sample_R_ = 0.0f;
    set_speed(0.5f);
    set_depth(0.0f);
}
    
void MoogFilterFX::set_speed(const float speed) {
    speed_ = speed;
    const float cutoff_freq = min_freq_ * fastpowf(max_freq_ / min_freq_, speed_);
    filter_coefficient_ = 1.0f - fastexpf(-2.0f * M_PI * (cutoff_freq / samplerate_));
}

void MoogFilterFX::set_depth(const float depth) { depth_ = depth; }

float MoogFilterFX::process_filter(float input, float filter_stages[4], float &last_sample) const {
    float current_signal = input;
    for (int i = 0; i < 4; ++i) {
        const float velocity = (current_signal - filter_stages[i]) * filter_coefficient_;
        last_sample = velocity + filter_stages[i]; // Keep last sample for the resonance loop
        filter_stages[i] = last_sample + velocity; // Update the internal state for the next sample
    }
    return last_sample;
}

float MoogFilterFX::process_main_L(const float x) {
    const float drive_gain = 1.0f + depth_ * 29.0f;
    const float signal = fasttanfullf(x * drive_gain);
    const float filter_input = signal - resonance_ * fasttanfullf(last_sample_L_);
    const float processed_signal = process_filter(filter_input, filter_stages_L_, last_sample_L_);
    return clip1m1f(processed_signal);
}

float MoogFilterFX::process_main_R(const float x) {
    const float drive_gain = 1.0f + depth_ * 29.0f;
    const float signal = fasttanfullf(x * drive_gain);
    const float filter_input = signal - resonance_ * fasttanfullf(last_sample_R_);
    const float processed_signal = process_filter(filter_input, filter_stages_R_, last_sample_R_);
    return clip1m1f(processed_signal);
}
