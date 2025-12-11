#pragma once

#include <cstdint>
#include <array>

namespace midi
{
    struct NoteState
    {
        bool     down      = false;  // true if key is currently held
        bool     sustained = false;  // true if held by sustain pedal
        uint8_t  velocity  = 0;      // last velocity
        uint32_t age       = 0;      // monotonically increasing timestamp
    };

    class MidiState
    {
      public:
        MidiState() { Reset(); }

        void Reset();

        // --- Note handling ---

        // Called on Note On events (after channel filtering)
        void NoteOn(uint8_t note, uint8_t velocity, uint32_t timestamp);

        // Called on Note Off events (after channel filtering)
        void NoteOff(uint8_t note);

        // Sustain pedal pressed (CC64 >= 64)
        void SustainOn();

        // Sustain pedal released (CC64 < 64)
        void SustainOff();

        bool IsNoteActive(uint8_t note) const;

        // Returns the most recent active note, or -1 if none
        int GetMostRecentActiveNote() const;

        bool GetSustain() const { return sustain_; }

        // --- Control Changes ---

        // Generic CC update (0-127)
        void ControlChange(uint8_t controller, uint8_t value);

        // Get last seen value for a CC (0-127)
        uint8_t GetCcValue(uint8_t controller) const;

        // --- Program Change ---

        void ProgramChange(uint8_t program); // 0-127

        uint8_t GetLastProgram() const { return last_program_; }

      private:
        std::array<NoteState, 128>  notes_;
        bool                        sustain_ = false;

        std::array<uint8_t, 128>   cc_values_;    // last value per CC
        uint8_t                     last_program_ = 0;
    };

} // namespace midi