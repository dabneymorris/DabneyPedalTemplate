#include "midi/MidiState.h"

namespace midi
{
    void MidiState::Reset()
    {
        for(auto &n : notes_)
        {
            n.down      = false;
            n.sustained = false;
            n.velocity  = 0;
            n.age       = 0;
        }
        sustain_      = false;
        last_program_ = 0;

        for(auto &v : cc_values_)
        {
            v = 0;
        }
    }

    void MidiState::NoteOn(uint8_t note, uint8_t velocity, uint32_t timestamp)
    {
        if(note >= notes_.size())
            return;

        auto &n  = notes_[note];
        n.down   = true;
        n.velocity = velocity;
        n.age    = timestamp; // caller provides a monotonically increasing time index
        // Sustained flag is handled in NoteOff/SustainOn/SustainOff.
    }

    void MidiState::NoteOff(uint8_t note)
    {
        if(note >= notes_.size())
            return;

        auto &n = notes_[note];
        n.down  = false;

        // If sustain is active, keep the note "alive" as sustained
        if(sustain_)
        {
            n.sustained = true;
        }
        else
        {
            // No sustain: fully release the note
            n.sustained = false;
            n.velocity  = 0;
            n.age       = 0;
        }
    }

    void MidiState::SustainOn()
    {
        sustain_ = true;
        // Existing notes remain as-is; NoteOff will start marking sustained notes.
    }

    void MidiState::SustainOff()
    {
        sustain_ = false;

        // Any notes that are only sustained (key already released) should die now
        for(auto &n : notes_)
        {
            if(!n.down && n.sustained)
            {
                n.sustained = false;
                n.velocity  = 0;
                n.age       = 0;
            }
        }
    }

    bool MidiState::IsNoteActive(uint8_t note) const
    {
        if(note >= notes_.size())
            return false;

        const auto &n = notes_[note];
        return n.down || n.sustained;
    }

    int MidiState::GetMostRecentActiveNote() const
    {
        uint32_t best_age  = 0;
        int      best_note = -1;

        for(uint8_t i = 0; i < notes_.size(); ++i)
        {
            const auto &n = notes_[i];
            if((n.down || n.sustained) && n.age >= best_age)
            {
                best_age  = n.age;
                best_note = static_cast<int>(i);
            }
        }

        return best_note;
    }

    void MidiState::ControlChange(uint8_t controller, uint8_t value)
    {
        if(controller >= cc_values_.size())
            return;

        cc_values_[controller] = value;
    }

    uint8_t MidiState::GetCcValue(uint8_t controller) const
    {
        if(controller >= cc_values_.size())
            return 0;

        return cc_values_[controller];
    }

    void MidiState::ProgramChange(uint8_t program)
    {
        last_program_ = program;
    }

} // namespace midi