#pragma once

#include "float_math.h"
#include "usermodfx.h"
#include "FXBase.hpp"


class MoogFilterSimpleFX : public FXBase {
public:
    /**
     * @brief Initializes the Moog Filter FX.
     */
    void init() override;
    /**
     * @brief Resets the Moog Filter FX to its initial state.
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
     * @brief Processes a block of audio for the left channel.
     * @param main_frames_L Pointer to the input/output buffer for the left channel.
     * @param frame_count The number of samples to process.
     */
    void process_main_L(float* main_frames_L, uint32_t frame_count) override;
    /**
     * @brief Processes a block of audio for the right channel.
     * @param main_frames_R Pointer to the input/output buffer for the right channel.
     * @param frame_count The number of samples to process.
     */
    void process_main_R(float* main_frames_R, uint32_t frame_count) override;

private:
    // --- Parameters ---
    float drive_;
    float cutoff_;
    float resonance_;

    // --- Filter Coefficients ---
    float g_;
    float downsample_filter_coefficient_;

    // --- State Variables ---
    float filter_stages_L_[4];
    float filter_stages_R_[4];

    // --- Constants ---
    static const uint32_t k_oversampling_factor = 1;
    static constexpr float k_samplerate = 48000.0f;
    const float oversampled_samplerate_ = k_samplerate * k_oversampling_factor;
    const float min_freq_ = 20.0f;
    const float max_freq_ = 20000.0f;
    const float softclip_fact_ = 0.1f;
    
    // --- Private Helper Functions ---

    /**
     * @brief Executes one stage of the 4-pole Moog Ladder Filter.
     * @param input The oversampled input signal for this stage.
     * @param stages Array with the 4 states of the filter stages.
     * @return The output signal of the 4-pole filter cascade.
     */
    float process_filter_stage(float input, float stages[4]);
    
    /**
     * @brief Processes a complete audio block for one channel (L or R).
     * Includes upsampling, filtering, and downsampling.
     * @param input_frames Pointer to the input buffer.
     * @param output_frames Pointer to the output buffer.
     * @param frame_count The number of samples to process.
     * @param filter_stages The 4 filter stages of the channel.
     */
    void process_main_channel(float* input_frames, float* output_frames, uint32_t frame_count, 
                                float filter_stages[4]);

};