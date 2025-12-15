# Runtime Model

This document explains the runtime execution model of the Daisy Pedal Template.
Understanding this model is critical to extending the firmware safely, especially
with respect to hard real-time audio constraints.

This file should live at:
docs/RUNTIME_MODEL.md

---

## Overview

The firmware operates in two strictly separated execution contexts:

1. Audio Thread (hard real-time)
2. Main Loop (non-real-time)

Each context has different rules and responsibilities. Violating these boundaries
will lead to audio glitches, instability, or lockups.

---

## Audio Thread (Hard Real-Time)

### What it is

The audio thread is driven by the Daisy audio interrupt. It executes the registered
audio callback at a fixed rate determined by the audio block size and sample rate.

### Call chain

Daisy audio interrupt  
→ AudioCallback(...)  
→ App::AudioCallback(...)  
→ DspEngine::ProcessInterleaved(...)

### Configuration

- Sample rate is set in:
  src/config/DspConfig.h
- Block size is set in:
  src/config/DspConfig.h
- Both are applied during HardwareConfig::Init()

### Responsibilities

The audio thread is responsible for:
- Reading audio input buffers
- Producing audio output buffers
- Running all DSP code

### Hard rules (non-negotiable)

The audio thread MUST NOT:
- Allocate memory (new, malloc, std::vector resize, etc.)
- Perform logging or printing
- Access GPIO, ADC, I2C, SPI, UART, USB
- Touch OLED or other displays
- Block, sleep, or wait
- Use mutexes or OS primitives

Violating any of these rules can break real-time audio.

### Current implementation

The current DspEngine implementation performs a simple stereo passthrough.
It is real-time safe and serves as a baseline.

---

## Main Loop (Non-Real-Time)

### What it is

The main loop runs continuously in main.cpp inside a while(1) loop.
It is not time-critical and may run at an irregular cadence depending
on workload.

### Call chain

while (1):
- App::Loop()
  - MidiManager::Process()
  - UiController::Update()

### Responsibilities

The main loop is responsible for:
- Polling MIDI input
- Polling UI hardware (buttons, encoders, pots, switches)
- Updating application state
- Rendering OLED/UI output (once implemented)
- Communicating with peripherals

### Allowed operations

Unlike the audio thread, the main loop may:
- Allocate memory
- Perform logging
- Access peripherals
- Use blocking calls sparingly

Even so, long blocking operations should be avoided to keep UI responsive.

---

## Communication Between Threads

### Key principle

The audio thread should only read stable, precomputed state.
The main loop should prepare that state.

### Recommended pattern

- Main loop:
  - Updates control state (UI, MIDI, parameters)
  - Writes results into simple structs or atomic values

- Audio thread:
  - Reads those values
  - Applies them during DSP processing

Avoid:
- Mutexes in the audio thread
- Complex shared data structures
- Writing to shared state from the audio callback

---

## Why this separation exists

Audio processing requires:
- Predictable timing
- Bounded execution time
- No interaction with slow peripherals

UI, MIDI, and hardware IO are inherently non-deterministic and must be isolated
from the audio callback.

This template enforces that separation structurally.

---

## Common mistakes

- Logging from inside DspEngine
- Reading GPIO or ADC values in the audio callback
- Updating UI state from audio code
- Sharing complex objects between threads
- Adding "just one quick print" in the callback

All of these will eventually cause audio failure.

---

## Summary

- There are two execution contexts: audio thread and main loop
- DSP lives exclusively in the audio thread
- UI, MIDI, and hardware IO live exclusively in the main loop
- Communication must be one-directional and lock-free

If you follow this model, the firmware will remain stable as it grows.
