#pragma once

#include "daisy_seed.h"
#include "system/HardwareConfig.h"
#include "dsp/DspEngine.h"

// The App class owns high-level pedal behavior:
// - Initializes hardware config
// - Initializes DSP
// - Routes audio callback to DSP
// - Later: handles MIDI, UI, preset management, etc.
class App
{
  public:
    App() {}
    ~App() {}

    // Initialize application with hardware reference
    void Init(daisy::DaisySeed& hw)
    {
        // Configure hardware (sample rate, block size, etc.)
        hw_config_.Init(hw);

        // Initialize DSP using the configured sample rate
        float sr = hw_config_.GetSampleRate();
        dsp_.Init(sr);
    }

    // Called from audio callback
    void AudioCallback(daisy::AudioHandle::InterleavingInputBuffer  in,
                       daisy::AudioHandle::InterleavingOutputBuffer out,
                       std::size_t                                size)
    {
        dsp_.ProcessInterleaved(in, out, size);
    }

    // Called from main loop (future UI, MIDI, preset management)
    void Loop()
    {
        // Nothing yet — will expand
    }

  private:
    HardwareConfig hw_config_;
    DspEngine      dsp_;
};