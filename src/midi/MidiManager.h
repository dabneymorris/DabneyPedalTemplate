#pragma once

#include <cstdint>
#include "daisy_seed.h"
#include "midi/MidiState.h"
#include "config/MidiConfig.h"

namespace midi
{
    // MidiManager wraps MIDI message handling:
    // - Filters by channel
    // - Updates MidiState for Note, CC, Program Change
    class MidiManager
    {
      public:
        MidiManager() {}
        ~MidiManager() {}

        void Init(daisy::DaisySeed& hw, MidiState* state)
        {
            (void)hw; // unused for now (will be used when we wire Daisy MIDI)
            state_       = state;
            listen_chan_ = config::midi::kDefaultChannel; // 0 = omni
            timestamp_   = 0;
        }

        // Called regularly from App::Loop()
        void Process()
        {
            // In the future: pull MIDI events from Daisy here and
            // dispatch them via HandleNoteOn/Off/ControlChange/ProgramChange.
        }

        // --- Channel handling ---

        // 0 = omni, 1-16 = specific channel
        void SetChannel(uint8_t channel)
        {
            if(channel <= 16)
                listen_chan_ = channel;
        }

        uint8_t GetChannel() const { return listen_chan_; }

        // --- Event handlers (to be called from Daisy MIDI callbacks later) ---

        void HandleNoteOn(uint8_t channel, uint8_t note, uint8_t velocity)
        {
            if(!state_ || !ChannelMatches(channel))
                return;

            ++timestamp_;
            state_->NoteOn(note, velocity, timestamp_);
        }

        void HandleNoteOff(uint8_t channel, uint8_t note, uint8_t velocity)
        {
            (void)velocity; // unused for now
            if(!state_ || !ChannelMatches(channel))
                return;

            ++timestamp_;
            state_->NoteOff(note);
        }

        void HandleControlChange(uint8_t channel,
                                 uint8_t controller,
                                 uint8_t value)
        {
            if(!state_ || !ChannelMatches(channel))
                return;

            ++timestamp_;

            // Sustain pedal (CC 64)
            if(controller == 64)
            {
                if(value >= 64)
                    state_->SustainOn();
                else
                    state_->SustainOff();
            }

            state_->ControlChange(controller, value);
        }

        void HandleProgramChange(uint8_t channel, uint8_t program)
        {
            if(!state_ || !ChannelMatches(channel))
                return;

            ++timestamp_;
            state_->ProgramChange(program);
        }

        MidiState* GetState() const { return state_; }

      private:
        bool ChannelMatches(uint8_t msg_chan) const
        {
            if(listen_chan_ == 0)
                return true; // omni
            return msg_chan == listen_chan_;
        }

        MidiState* state_       = nullptr;
        uint8_t    listen_chan_ = 0; // 0=omni, 1-16 valid
        uint32_t   timestamp_   = 0; // increments per processed event
    };

} // namespace midi