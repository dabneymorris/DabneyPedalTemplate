# DSP Engine

This document describes the DSP layer of the Daisy Pedal Template, including
audio buffer semantics, real-time constraints, and recommended extension patterns.

This file should live at:
docs/DSP.md

---

## Role of the DSP Engine

The DSP engine is responsible for all audio-rate processing.
It is the only code that runs inside the Daisy audio callback.

In this template, the DSP engine is implemented by:
- src/dsp/DspEngine.h
- src/dsp/DspEngine.cpp

---

## Audio Callback Contract

### Callback signature

The Daisy audio system provides interleaved input and output buffers
to the registered audio callback.

The callback is registered in main.cpp and forwarded to App, then to DspEngine.

### Buffer format

- Buffers are float32
- Stereo, interleaved
- Layout:
  index 0 = Left sample 0
  index 1 = Right sample 0
  index 2 = Left sample 1
  index 3 = Right sample 1
  ...

The size parameter passed to ProcessInterleaved is the total number
of float samples in the interleaved buffer.

---

## Current Implementation

The current DspEngine implementation performs a pure passthrough:

- Input samples are copied directly to output
- No gain, filtering, or processing
- Fully real-time safe

This serves as:
- A hardware sanity check
- A safe baseline for extension

---

## Real-Time Safety Rules

The DSP engine runs in the hard real-time audio thread.

It MUST NOT:
- Allocate memory (new, malloc, STL containers)
- Perform logging or printing
- Access GPIO, ADC, I2C, SPI, UART, USB
- Touch OLED or UI state
- Block, sleep, or wait
- Use mutexes or locks

Violating these rules will cause audio glitches or lockups.

---

## Extending the DSP Engine

### Where to add DSP code

All audio processing should live in:
- DspEngine::ProcessInterleaved

Or in helper classes/functions called from it.

Avoid placing DSP logic in:
- App
- UI modules
- MIDI modules
- HardwareConfig

---

### Parameters and control input

Recommended pattern:
- Main loop computes control values (UI, MIDI, presets)
- These values are written into simple structs or atomic variables
- DSP engine reads those values during audio processing

Avoid:
- Reading UI or MIDI state directly in the audio callback
- Writing shared state from the audio thread

---

### Parameter smoothing

When adding parameters that change at control rate:
- Use simple one-pole smoothing or linear interpolation
- Update smoothing per audio block, not per sample if possible
- Keep math predictable and bounded

---

## Multichannel and Voice Expansion

If you add voices or multiple DSP modules:
- Keep all voice processing inside DspEngine
- Avoid dynamic allocation for voices
- Prefer fixed-size arrays or compile-time limits

If voice allocation is needed:
- Perform allocation decisions in the main loop
- Provide DSP with a stable voice configuration snapshot

---

## DaisySP Usage

This template includes DaisySP as a dependency.

Recommended usage:
- Instantiate DaisySP objects as members of DspEngine
- Initialize them in DspEngine::Init
- Process them inside ProcessInterleaved

Avoid:
- Creating or destroying DaisySP objects in the audio callback

---

## Testing and Debugging DSP

Safe debugging techniques:
- Bypass sections of DSP with compile-time flags
- Use test tones or constant signals
- Temporarily reduce block size for stress testing

Unsafe debugging techniques:
- printf or LOGF in the audio callback
- Toggling GPIO from DSP code

---

## Common Mistakes

- Reading ADC values directly in ProcessInterleaved
- Calling UI or MIDI code from DSP
- Adding heap allocations for filters or buffers
- Using STL containers inside the audio callback

---

## Summary

- DspEngine owns all audio-rate processing
- Audio buffers are stereo, interleaved float32
- Real-time safety is the highest priority
- Control logic belongs in the main loop
- DSP reads precomputed, stable state only

Following these rules ensures glitch-free audio and long-term stability.
