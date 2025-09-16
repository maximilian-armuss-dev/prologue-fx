#include "MoogFilterFX.hpp"

// ------------------------------------- FXBase Overrides -------------------------------------

/**
 * @brief Initializes the Moog Filter FX.
 * This method is called once when the effect is loaded.
 */
void MoogFilterFX::init(){
    filter_coefficient_ = 0;
    for (int i = 0; i < 4; ++i) {
        filter_stages_L_[i] = 0.0f;
        filter_stages_R_[i] = 0.0f;
    }
    last_sample_L_ = 0.0f;
    last_sample_R_ = 0.0f;
    for (int i = 0; i < 2; ++i) {
        downsample_filter_stages_L_[i] = 0.0f;
        downsample_filter_stages_R_[i] = 0.0f;
    }
    downsample_filter_coefficient_ = 0.0f; 

    set_speed(0.5f);
    set_depth(1.0f);

    // Initialisiere Upsampling-Delay-Lines
    for (int i = 0; i < 2; ++i) {
        upsample_delay_line_L_[i] = 0.0f;
        upsample_delay_line_R_[i] = 0.0f;
    }

    const float downsample_cutoff_freq = k_samplerate / 2.0f / k_oversampling_factor;
    downsample_filter_coefficient_ = 1.0f - fastexpf(-2.0f * M_PI * (downsample_cutoff_freq / oversampled_samplerate_));
}

/**
 * @brief Resets the Moog Filter FX to its initial state.
 * This method is called when the effect is reset.
 */
void MoogFilterFX::reset(){
    init();
}
    
/**
 * @brief Sets the drive parameter of the filter.
 * @param speed A normalized value (0.0 to 1.0) controlling the drive amount.
 *        The internal drive_ parameter is scaled from 1.0 to 30.0.
 */
void MoogFilterFX::set_speed(const float speed) {
    drive_ = 1.0f + speed * 29.0f;
}

/**
 * @brief Sets the cutoff frequency parameter of the filter.
 * @param depth A normalized value (0.0 to 1.0) controlling the cutoff frequency.
 *        The internal filter_coefficient_ is calculated based on a logarithmic frequency range (20Hz to 20kHz).
 */
void MoogFilterFX::set_depth(const float depth) {
    cutoff_ = depth;
    const float cutoff_freq = min_freq_ * fastpowf(max_freq_ / min_freq_, cutoff_);
    filter_coefficient_ = 1.0f - fastexpf(-2.0f * M_PI * (cutoff_freq / oversampled_samplerate_));
}

/**
 * @brief Processes the left channel audio input.
 * @param x The input audio sample for the left channel.
 * @return The processed audio sample for the left channel.
 */
void MoogFilterFX::process_main_L(float* main_frames_L, uint32_t frame_count) {
    process_main_channel(main_frames_L, main_frames_L, frame_count, filter_stages_L_, last_sample_L_, downsample_filter_stages_L_, downsample_filter_coefficient_, true);
}

/**
 * @brief Processes the right channel audio input.
 * @param x The input audio sample for the right channel.
 * @return The processed audio sample for the right channel.
 */
void MoogFilterFX::process_main_R(float* main_frames_R, uint32_t frame_count) {
    process_main_channel(main_frames_R, main_frames_R, frame_count, filter_stages_R_, last_sample_R_, downsample_filter_stages_R_, downsample_filter_coefficient_, false);
}

// ------------------------------------- Helper Functions -------------------------------------

/**
 * @brief Processes a single stage of the 4-pole Moog-style low-pass filter.
 * @param input The input signal to the filter stage.
 * @param stages An array representing the 4 filter stages.
 * @param last_sample A reference to the last sample processed, used for the resonance feedback loop.
 * @return The output of the current filter stage.
 */
float MoogFilterFX::process_filter(float input, float filter_stages[4], float &last_sample) {
    float current_signal = input;
    for (int i = 0; i < 4; ++i) {
        const float velocity = (current_signal - filter_stages[i]) * filter_coefficient_;
        last_sample = velocity + filter_stages[i]; 
        filter_stages[i] = last_sample + velocity;
    }
    return last_sample;
}

/**
 * @brief Performs linear interpolation upsampling of an audio sample.
 * @param prev_input The previous input sample.
 * @param current_input The current input sample.
 * @param buffer A pointer to the buffer where the upsampled samples will be stored.
 * @param buffer_size The size of the buffer (i.e., the oversampling factor).
 */
void MoogFilterFX::upsample_input(float prev_input, float current_input, float *buffer, uint32_t buffer_size) const {
    for (uint32_t i = 0; i < buffer_size; ++i) {
        float alpha = (float)i / (float)buffer_size;
        buffer[i] = prev_input * (1.0f - alpha) + current_input * alpha;
    }
}

/**
 * @brief Processes a single oversampled filter stage, including drive and resonance.
 * @param input The oversampled input sample.
 * @param last_sample A reference to the last sample processed by the main filter for resonance feedback.
 * @param filter_stages An array representing the 4 filter stages.
 * @return The processed oversampled output sample.
 */
float MoogFilterFX::process_oversampled_filter_stage(float input, float& last_sample, float filter_stages[4]) {
    const float signal = fasttanfullf(input * drive_);
    const float filter_input = signal - resonance_ * fasttanfullf(last_sample);
    return process_filter(filter_input, filter_stages, last_sample);
}

/**
 * @brief Performs 2-pole low-pass downsampling of an oversampled signal.
 * @param oversampled_output The oversampled output sample to be downsampled.
 * @param downsample_stages An array representing the 2 downsample filter stages.
 * @param downsample_coeff A reference to the downsampling filter coefficient.
 * @return The downsampled output sample.
 */
float MoogFilterFX::downsample_output(float oversampled_output, float downsample_stages[2], float& downsample_coeff) const {
    float current_downsample_signal = oversampled_output;
    for (int i = 0; i < 2; ++i) {
        const float velocity = (current_downsample_signal - downsample_stages[i]) * downsample_coeff;
        downsample_stages[i] += velocity;
        current_downsample_signal = downsample_stages[i];
    }
    return current_downsample_signal;
}

/**
 * @brief Processes a single channel (left or right) through the Moog filter, including oversampling and downsampling.
 * @param input The input audio sample for the current channel.
 * @param filter_stages An array representing the 4 main filter stages for the current channel.
 * @param last_sample A reference to the last sample processed by the main filter for resonance feedback (current channel).
 * @param downsample_stages An array representing the 2 downsample filter stages for the current channel.
 * @param downsample_coeff A reference to the downsampling filter coefficient for the current channel.
 * @param is_left_channel A boolean indicating if the current channel is the left channel (true) or right channel (false).
 * @return The processed output sample for the current channel.
 */
void MoogFilterFX::process_main_channel(float* input_frames, float* output_frames, uint32_t frame_count, float filter_stages[4], float& last_sample, float downsample_stages[2], float& downsample_coeff, bool is_left_channel) {
    float *current_upsample_delay_line = is_left_channel ? upsample_delay_line_L_ : upsample_delay_line_R_;
    for (uint32_t j = 0; j < frame_count; ++j) {
        float prev_input = current_upsample_delay_line[0];
        current_upsample_delay_line[0] = input_frames[j];
        float temp_oversampled_input_buffer[k_oversampling_factor];
        upsample_input(prev_input, input_frames[j], temp_oversampled_input_buffer, k_oversampling_factor);
        float oversampled_output = 0.0f;
        for (uint32_t i = 0; i < k_oversampling_factor; ++i) {
            oversampled_output = process_oversampled_filter_stage(temp_oversampled_input_buffer[i], last_sample, filter_stages); 
        }
        float final_output = downsample_output(oversampled_output, downsample_stages, downsample_coeff);
        output_frames[j] = clip1m1f(final_output);
    }
}
