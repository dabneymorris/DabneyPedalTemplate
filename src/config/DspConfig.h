#pragma once

#include "daisy_seed.h"

// DSP-related configuration for all pedals based on this template.
namespace config
{
namespace dsp
{
    // Default audio sample rate
    static constexpr daisy::SaiHandle::Config::SampleRate kSampleRate
        = daisy::SaiHandle::Config::SampleRate::SAI_48KHZ;

    // Default audio block size (number of samples per channel per callback)
    static constexpr std::size_t kBlockSize = 16;
} // namespace dsp
} // namespace config