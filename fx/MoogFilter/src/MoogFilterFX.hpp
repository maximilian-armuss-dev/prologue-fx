#pragma once

#include "float_math.h"
#include "usermodfx.h"
#include "FXBase.hpp"


class MoogFilterFX : public FXBase {
public:
    /**
     * @brief Initializes the Moog Filter FX. This method is called once when the effect is loaded.
     */
    void init() override;
    /**
     * @brief Resets the Moog Filter FX to its initial state. This method is called when the effect is reset.
     */
    void reset() override;
    /**
     * @brief Sets the drive parameter of the filter.
     * @param speed A normalized value (0.0 to 1.0) controlling the drive amount.
     */
    void set_speed(float speed) override;
    /**
     * @brief Sets the cutoff frequency parameter of the filter.
     * @param depth A normalized value (0.0 to 1.0) controlling the cutoff frequency.
     */
    void set_depth(float depth) override;
    /**
     * @brief Processes the left channel audio input.
     * @param x The input audio sample for the left channel.
     * @return The processed audio sample for the left channel.
     */
    void process_main_L(float* main_frames_L, uint32_t frame_count) override;
    /**
     * @brief Processes the right channel audio input.
     * @param x The input audio sample for the right channel.
     * @return The processed audio sample for the right channel.
     */
    void process_main_R(float* main_frames_R, uint32_t frame_count) override;

private:
    float drive_;
    float cutoff_;
    float filter_coefficient_;
    float filter_stages_L_[4];
    float filter_stages_R_[4];
    float last_sample_L_;
    float last_sample_R_;

    static const uint32_t k_oversampling_factor = 4;
    // Couldn't find 'k_samplerate' in usermodfx.h, but at least in osc_api.h, where it's set to 48kHz
    static constexpr float k_samplerate = 48000.0f; 
    const float oversampled_samplerate_ = k_samplerate * k_oversampling_factor;

    float downsample_filter_stages_L_[2];
    float downsample_filter_stages_R_[2];
    float downsample_filter_coefficient_;
    
    const float min_freq_ = 20.0f;
    const float max_freq_ = 20000.0f;
    const float resonance_ = 3.0f;

    // Delay Line for Upsampling-Interpolation (stores previous samples)
    float upsample_delay_line_L_[2]; // Stores the last 2 samples for linear interpolation
    float upsample_delay_line_R_[2]; // Stores the last 2 samples for linear interpolation

    /**
     * @brief Processes a single stage of the 4-pole Moog-style low-pass filter.
     * @param input The input signal to the filter stage.
     * @param stages An array representing the 4 filter stages.
     * @param delay A reference to the last sample processed, used for the resonance feedback loop.
     * @return The output of the current filter stage.
     */
    float process_filter(float input, float stages[4], float &delay);
    /**
     * @brief Performs linear interpolation upsampling of an audio sample.
     * @param prev_input The previous input sample.
     * @param current_input The current input sample.
     * @param buffer A pointer to the buffer where the upsampled samples will be stored.
     * @param buffer_size The size of the buffer (i.e., the oversampling factor).
     */
    void upsample_input(float prev_input, float current_input, float *buffer, uint32_t buffer_size) const; 
    /**
     * @brief Processes a single oversampled filter stage, including drive and resonance.
     * @param input The oversampled input sample.
     * @param last_sample A reference to the last sample processed by the main filter for resonance feedback.
     * @param filter_stages An array representing the 4 filter stages.
     * @return The processed oversampled output sample.
     */
    float process_oversampled_filter_stage(float input, float& last_sample, float filter_stages[4]);
    /**
     * @brief Performs 2-pole low-pass downsampling of an oversampled signal.
     * @param oversampled_output The oversampled output sample to be downsampled.
     * @param downsample_stages An array representing the 2 downsample filter stages.
     * @param downsample_coeff A reference to the downsampling filter coefficient.
     * @return The downsampled output sample.
     */
    float downsample_output(float oversampled_output, float downsample_stages[2], float& downsample_coeff) const;
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
    void process_main_channel(float* input_frames, float* output_frames, uint32_t frame_count, float filter_stages[4], float& last_sample, float downsample_stages[2], float& downsample_coeff, bool is_left_channel);

};
