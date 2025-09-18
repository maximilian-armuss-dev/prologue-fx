#include "MoogFilterFX.hpp"

// ------------------------------------- FXBase Overrides -------------------------------------

void MoogFilterFX::init() {
    for (int i = 0; i < 4; ++i) {
        filter_stages_L_[i] = 0.0f;
        filter_stages_R_[i] = 0.0f;
    }
    for (int i = 0; i < 2; ++i) {
        downsample_filter_stages_L_[i] = 0.0f;
        downsample_filter_stages_R_[i] = 0.0f;
    }
    upsample_delay_line_L_ = 0.0f;
    upsample_delay_line_R_ = 0.0f;

    set_speed(0.1f); // Low initial drive value
    set_depth(1.0f); // Filter is fully open initially

    resonance_ = 3.5f; // Moog-characteristic
    const float downsample_cutoff_freq = k_samplerate / 2.0f / k_oversampling_factor;
    downsample_filter_coefficient_ = 1.0f - fasterexpf(-2.0f * M_PI * (downsample_cutoff_freq / oversampled_samplerate_));
}

void MoogFilterFX::reset() { init(); }

void MoogFilterFX::set_speed(const float speed) { drive_ = 1.0f + speed * 9.0f; }

void MoogFilterFX::set_depth(const float depth) {
    cutoff_ = depth;
    const float cutoff_freq = min_freq_ * fasterpowf(max_freq_ / min_freq_, cutoff_);
    g_ = 1.0f - fasterexpf(-2.0f * M_PI * cutoff_freq / oversampled_samplerate_);
}

void MoogFilterFX::process_main_L(float* main_frames_L, uint32_t frame_count) {
    process_main_channel(main_frames_L, main_frames_L, frame_count, filter_stages_L_, downsample_filter_stages_L_, upsample_delay_line_L_);
}

void MoogFilterFX::process_main_R(float* main_frames_R, uint32_t frame_count) {
    process_main_channel(main_frames_R, main_frames_R, frame_count, filter_stages_R_, downsample_filter_stages_R_, upsample_delay_line_R_);
}

// ------------------------------------- Helper Functions -------------------------------------

float MoogFilterFX::process_filter_stage(float input, float stages[4]) {
    input = fastertanfullf(input * drive_);
    float feedback = stages[3] * resonance_;
    float filter_input = input - feedback;

    stages[0] = stages[0] + g_ * (fastertanfullf(filter_input) - fastertanfullf(stages[0]));
    stages[1] = stages[1] + g_ * (fastertanfullf(stages[0]) - fastertanfullf(stages[1]));
    stages[2] = stages[2] + g_ * (fastertanfullf(stages[1]) - fastertanfullf(stages[2]));
    stages[3] = stages[3] + g_ * (fastertanfullf(stages[2]) - fastertanfullf(stages[3]));

    return stages[3];
}

void MoogFilterFX::process_main_channel(float* input_frames, float* output_frames, uint32_t frame_count, float filter_stages[4], float downsample_stages[2], float& upsample_delay_line) {
    for (uint32_t i = 0; i < frame_count; ++i) {
        const float current_input = input_frames[i];
        const float prev_input = upsample_delay_line;
        const float step = (current_input - prev_input) / k_oversampling_factor;
        for (uint32_t j = 0; j < k_oversampling_factor; ++j) {
            const float oversampled_input = prev_input + j * step;
            const float oversampled_output = process_filter_stage(oversampled_input, filter_stages);
            downsample_stages[0] += (oversampled_output - downsample_stages[0]) * downsample_filter_coefficient_;
            downsample_stages[1] += (downsample_stages[0] - downsample_stages[1]) * downsample_filter_coefficient_;
        }
        upsample_delay_line = current_input;
        output_frames[i] = fx_softclipf(softclip_fact_, downsample_stages[1]);
    }
}