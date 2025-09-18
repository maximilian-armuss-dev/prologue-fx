#include "MoogFilterSimpleFX.hpp"

// ------------------------------------- FXBase Overrides -------------------------------------

void MoogFilterSimpleFX::init() {
    for (int i = 0; i < 4; ++i) {
        filter_stages_L_[i] = 0.0f;
        filter_stages_R_[i] = 0.0f;
    }

    set_speed(0.1f); // Low initial drive value
    set_depth(1.0f); // Filter is fully open initially

    resonance_ = 3.5f; // Moog-characteristic
}

void MoogFilterSimpleFX::reset() { init(); }

void MoogFilterSimpleFX::set_speed(const float speed) { drive_ = 1.0f + speed * 9.0f; }

void MoogFilterSimpleFX::set_depth(const float depth) {
    cutoff_ = depth;
    const float cutoff_freq = min_freq_ * fasterpowf(max_freq_ / min_freq_, cutoff_);
    g_ = 1.0f - fasterexpf(-2.0f * M_PI * cutoff_freq / k_samplerate);
}

void MoogFilterSimpleFX::process_main_L(float* main_frames_L, uint32_t frame_count) {
    process_main_channel(main_frames_L, main_frames_L, frame_count, filter_stages_L_);
}

void MoogFilterSimpleFX::process_main_R(float* main_frames_R, uint32_t frame_count) {
    process_main_channel(main_frames_R, main_frames_R, frame_count, filter_stages_R_);
}

// ------------------------------------- Helper Functions -------------------------------------

float MoogFilterSimpleFX::process_filter_stage(float input, float stages[4]) {
    float drive_input = clampfsel(-M_PI_2, input * drive_, M_PI_2);
    input = fastertanhf(drive_input);
    float feedback = stages[3] * resonance_;
    float filter_input = input - feedback;

    float clamped_filter_input = clampfsel(-M_PI_2, filter_input, M_PI_2);
    float clamped_stages_0_input = clampfsel(-M_PI_2, stages[0], M_PI_2);
    stages[0] = stages[0] + g_ * (fastertanhf(clamped_filter_input) - fastertanhf(clamped_stages_0_input));
    stages[0] = clampfsel(-4.0f, stages[0], 4.0f);

    float clamped_stages_1_input = clampfsel(-M_PI_2, stages[0], M_PI_2);
    stages[1] = stages[1] + g_ * (fastertanhf(clamped_stages_1_input) - fastertanhf(clampfsel(-M_PI_2, stages[1], M_PI_2)));
    stages[1] = clampfsel(-4.0f, stages[1], 4.0f);

    float clamped_stages_2_input = clampfsel(-M_PI_2, stages[1], M_PI_2);
    stages[2] = stages[2] + g_ * (fastertanhf(clamped_stages_2_input) - fastertanhf(clampfsel(-M_PI_2, stages[2], M_PI_2)));
    stages[2] = clampfsel(-4.0f, stages[2], 4.0f);

    float clamped_stages_3_input = clampfsel(-M_PI_2, stages[2], M_PI_2);
    stages[3] = stages[3] + g_ * (fastertanhf(clamped_stages_3_input) - fastertanhf(clampfsel(-M_PI_2, stages[3], M_PI_2)));
    stages[3] = clampfsel(-4.0f, stages[3], 4.0f);

    return stages[3];
}

void MoogFilterSimpleFX::process_main_channel(float* input_frames, float* output_frames, uint32_t frame_count, float filter_stages[4]) {
    for (uint32_t i = 0; i < frame_count; ++i) {
        const float current_input = input_frames[i];
        const float output = process_filter_stage(current_input, filter_stages);
        output_frames[i] = fx_softclipf(softclip_fact_, output); 
    }
}