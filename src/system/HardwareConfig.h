#pragma once

#include "daisy_seed.h"
#include "config/DspConfig.h"

// HardwareConfig centralizes hardware-related setup:
// - Audio sample rate and block size
// - (Later) pin mappings, peripherals, etc.
class HardwareConfig
{
  public:
    HardwareConfig() {}
    ~HardwareConfig() {}

    // Call once after hw.Configure() and hw.Init() have been called in main().
    void Init(daisy::DaisySeed& hw)
    {
        hw_ = &hw;

        // Set block size and sample rate from config::dsp
        hw.SetAudioBlockSize(config::dsp::kBlockSize);
        hw.SetAudioSampleRate(config::dsp::kSampleRate);

        sample_rate_ = hw.AudioSampleRate();
        block_size_  = hw.AudioBlockSize();
    }

    float GetSampleRate() const { return sample_rate_; }
    std::size_t GetBlockSize() const { return block_size_; }

    daisy::DaisySeed* GetHw() const { return hw_; }

  private:
    daisy::DaisySeed* hw_{nullptr};
    float             sample_rate_{48000.0f};
    std::size_t       block_size_{16};
};