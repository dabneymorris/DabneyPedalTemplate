#pragma once

#include "daisy_seed.h"
#include "system/HardwareConfig.h"
#include "dsp/DspEngine.h"
#include "midi/MidiState.h"
#include "midi/MidiManager.h"
#include "ui/UiController.h"

// The App class owns high-level pedal behavior.
class App
{
  public:
    App() {}
    ~App() {}

    void Init(daisy::DaisySeed& hw)
    {
        // Hardware (sample rate, block size, etc.)
        hw_config_.Init(hw);

        // MIDI
        midi_state_ = midi::MidiState{};
        midi_mgr_.Init(hw, &midi_state_);

        // DSP
        float sr = hw_config_.GetSampleRate();
        dsp_.Init(sr);

        // UI
        ui_.Init();
    }

    void AudioCallback(daisy::AudioHandle::InterleavingInputBuffer  in,
                       daisy::AudioHandle::InterleavingOutputBuffer out,
                       std::size_t                                  size)
    {
        dsp_.ProcessInterleaved(in, out, size);
    }

    void Loop()
    {
        // Order here can be tuned later if we want:
        midi_mgr_.Process();
        ui_.Update();
    }

  private:
    HardwareConfig    hw_config_;
    DspEngine         dsp_;
    midi::MidiState   midi_state_;
    midi::MidiManager midi_mgr_;
    UiController      ui_;
};