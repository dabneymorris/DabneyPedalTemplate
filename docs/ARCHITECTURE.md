# Daisy Pedal Template – Firmware Architecture

This repository is a clean, minimal firmware template for projects built on the
Electro-Smith Daisy Seed platform.

It intentionally provides:
- A correct real-time execution model
- A clear separation of concerns
- Well-defined extension points

It does NOT include finished DSP algorithms, hardware wiring, presets, storage,
or UI rendering. Those are explicitly left to downstream projects.

---

## Design Goals

1. Hard real-time safety  
   Audio processing must never block, allocate, log, or touch peripherals.

2. Clear ownership  
   Each subsystem owns exactly one responsibility.

3. Scaffolded growth  
   DSP, UI, MIDI, and hardware wiring are structured but minimal.

4. LLM-first clarity  
   Intent is explicit; stubs are clearly labeled as such.

---

## Execution Model

The firmware runs in two distinct execution contexts.

### Audio Thread (Hard Real-Time)

Driven by Daisy’s audio interrupt.

Call chain:
Daisy audio ISR  
→ AudioCallback(...)  
→ App::AudioCallback(...)  
→ DspEngine::ProcessInterleaved(...)

Rules:
- Runs at the configured audio block rate
- Must never:
  - Allocate memory
  - Perform logging
  - Access UI, GPIO, ADC, I2C, SPI, or MIDI
  - Block or sleep

The current DSP implementation is a stereo passthrough.

---

### Main Loop (Non-Real-Time)

Runs continuously inside while(1) in main.cpp.

Call chain:
while (1)
- MidiManager::Process()
- UiController::Update()

Responsibilities:
- Poll MIDI (currently stubbed)
- Poll UI controls (currently stubbed)
- Update non-audio state

This loop may touch peripherals, perform I/O, and evolve freely.

---

## High-Level Module Responsibilities

### main.cpp
- Configures and initializes the Daisy Seed
- Starts the audio engine
- Owns the infinite main loop
- Contains no application logic

---

### App
Role: Composition root and lifecycle owner.

Responsibilities:
- Initialize all subsystems
- Bridge audio callback to DSP
- Define update order for non-audio systems

Non-responsibilities:
- DSP algorithms
- MIDI semantics
- UI rendering
- Hardware pin definitions

---

### HardwareConfig
Role: Central hardware setup authority.

Current responsibilities:
- Configure audio sample rate
- Configure audio block size
- Store references to the Daisy hardware object

Planned growth (not implemented here):
- GPIO pin mappings
- ADC channel setup
- Peripheral initialization

---

### DspEngine
Role: All audio-rate processing.

Contract:
- Input/output buffers are interleaved stereo floats
- Processing occurs entirely inside the audio callback
- Must remain real-time safe

Current implementation:
- Pure passthrough

Extension point:
- Add voices, effects, filters, pitch-shifters, etc.
- Must not depend on UI or hardware directly

---

### MidiState
Role: Canonical MIDI state model.

Implements:
- Per-note state (down, sustained, velocity, age)
- Sustain pedal behavior (CC64)
- Control Change storage
- Program Change storage
- Most-recent-note resolution

This class is fully implemented and stable.

---

### MidiManager
Role: MIDI event ingestion and filtering.

Responsibilities:
- Channel filtering (omni or specific channel)
- Translate raw MIDI events into MidiState mutations

Current limitations:
- No hardware MIDI source wired yet
- Process() is a stub

---

### UiController
Role: UI polling and orchestration.

Responsibilities:
- Poll all UI primitives each loop
- Track current UI page
- Eventually drive OLED rendering

Current state:
- Calls Init() and Update() on UI elements
- No rendering or behavior implemented

---

### UI Primitives (ui/)
Includes:
- Encoder
- Button
- Footswitch
- ToggleSwitch
- Potentiometer
- Led
- OledView

Current state:
- All are explicit stubs
- No GPIO, ADC, debouncing, or drivers implemented
- Intended as placeholders for hardware-specific wiring

---

### util/
- Types.h: shared enums (UI pages, bypass state)
- Log: logging interface stub (macro exists, no backend)

---

## Configuration Model

All compile-time configuration lives in src/config/.

- BuildConfig.h – debug and logging toggles
- DspConfig.h – sample rate and block size
- MidiConfig.h – default MIDI channel
- UiConfig.h – UI timing constants

Configuration values are read-only at runtime.

---

## What This Template Does NOT Include

This repository intentionally does NOT implement:
- Preset systems
- QSPI or flash storage
- Voice allocation
- DSP algorithms beyond passthrough
- UI menus or parameter editing
- Hardware pin mappings
- MIDI hardware wiring
- OLED drivers

These are left to downstream projects.

---

## Summary

This template provides:
- A correct Daisy Seed execution model
- A safe audio architecture
- A clean separation between DSP, MIDI, UI, and hardware
- A stable foundation for growth

It is designed to be extended, not filled with assumptions.
