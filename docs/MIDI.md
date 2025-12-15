# MIDI System

This document describes the MIDI subsystem of the Daisy Pedal Template, including
how MIDI state is modeled, how events are handled, and how to safely extend MIDI
behavior.

This file should live at:
docs/MIDI.md

---

## Overview

The MIDI subsystem is split into two layers:

1. MidiState — the authoritative, persistent MIDI state model
2. MidiManager — MIDI message ingestion, filtering, and dispatch

This separation keeps MIDI semantics independent of hardware wiring and DSP.

---

## MidiState

Source files:
- src/midi/MidiState.h
- src/midi/MidiState.cpp

### Purpose

MidiState represents the current musical intent expressed by MIDI input.
It is designed to be read by higher-level logic (DSP, app state) without
requiring access to raw MIDI messages.

### Note state model

MidiState tracks 128 MIDI notes, each with:

- down: whether the key is currently held
- sustained: whether the note is held by the sustain pedal
- velocity: last note-on velocity
- age: monotonically increasing timestamp for recency

A note is considered active if:
- down is true, OR
- sustained is true

### Sustain pedal behavior (CC64)

- Sustain ON (value >= 64):
  - sustain flag is set
  - note-off events mark notes as sustained instead of releasing them

- Sustain OFF (value < 64):
  - sustain flag is cleared
  - any notes that are sustained but not held are fully released

This matches standard MIDI sustain pedal behavior.

### Note priority

MidiState provides:
- GetMostRecentActiveNote()

This returns the active note with the highest age value, or -1 if none are active.
This is useful for monophonic voice selection.

---

### Control Change (CC) handling

MidiState stores the most recent value (0–127) for each CC number.

- ControlChange(controller, value) updates internal state
- GetCcValue(controller) returns the last seen value

This allows control-rate access to CC values without replaying events.

---

### Program Change handling

MidiState stores:
- last_program_ (0–127)

ProgramChange(program) updates this value.
No additional behavior is implemented in the template.

---

## MidiManager

Source files:
- src/midi/MidiManager.h
- src/midi/MidiManager.cpp

### Purpose

MidiManager is responsible for:
- Receiving raw MIDI events
- Filtering them by channel
- Translating them into updates on MidiState

It intentionally contains minimal logic beyond filtering and dispatch.

---

### Channel filtering

- listen_chan_ = 0 → omni mode (accept all channels)
- listen_chan_ = 1–16 → accept only matching channel

The default channel is defined in:
src/config/MidiConfig.h

---

### Event handlers

MidiManager exposes handlers intended to be called from a MIDI hardware layer:

- HandleNoteOn(channel, note, velocity)
- HandleNoteOff(channel, note, velocity)
- HandleControlChange(channel, controller, value)
- HandleProgramChange(channel, program)

Each handler:
- Verifies channel match
- Updates a monotonically increasing timestamp
- Mutates MidiState accordingly

---

### Current limitations

- MidiManager::Process() is currently a stub
- No Daisy MIDI input (USB or UART) is wired yet

This template defines MIDI semantics but does not bind them to hardware.

---

## Threading and safety

Recommended usage:
- MidiManager::Process() runs in the main loop
- MidiState is mutated only in the main loop
- DSP should read a stable snapshot of MIDI-derived state

Avoid:
- Updating MidiState in the audio callback
- Calling MIDI handlers from audio-rate code

---

## Extending the MIDI system

Common extensions include:
- Wiring Daisy USB MIDI or UART MIDI into MidiManager::Process()
- Adding support for pitch bend or aftertouch
- Mapping CC values to parameters
- Using Program Change to trigger preset selection (outside this template)

Recommended pattern:
- Keep MidiState focused on musical intent
- Implement higher-level behaviors (presets, voice allocation) in separate modules

---

## Common mistakes

- Mixing MIDI parsing with DSP code
- Accessing MidiState directly from UI or hardware ISRs
- Treating raw MIDI events as real-time audio data

---

## Summary

- MidiState is the authoritative MIDI model
- MidiManager handles event filtering and dispatch
- MIDI logic belongs in the main loop, not the audio thread
- Hardware wiring is intentionally left to downstream projects

This separation keeps MIDI behavior predictable and extensible.
