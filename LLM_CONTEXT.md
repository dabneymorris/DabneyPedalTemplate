# LLM Context: Daisy Pedal Template

This document explains how to read and reason about this repository.
It is written specifically for large language models (LLMs) and automated tools.

This file should live at the repository root:
LLM_CONTEXT.md

---

## What this repository is

This repository is a firmware template for projects built on the
Electro-Smith Daisy Seed platform.

It is intentionally:
- Minimal
- Structurally complete
- Safe with respect to real-time audio constraints

It is NOT:
- A finished pedal
- A DSP algorithm showcase
- A preset system
- A UI framework

Many components are present as scaffolding only.

---

## How to approach this codebase

When reasoning about this repository, follow this order:

1. README.md  
   High-level overview and build instructions.

2. MODULES.md  
   Mental map of where things live and where to extend.

3. docs/ARCHITECTURE.md  
   Ownership boundaries and system structure.

4. docs/RUNTIME_MODEL.md  
   Real-time vs non-real-time execution rules.

5. docs/DSP.md  
   Audio processing constraints and buffer semantics.

6. docs/MIDI.md  
   MIDI semantics and state modeling.

7. docs/UI.md  
   UI scaffolding and intended wiring patterns.

8. docs/CONFIG.md  
   Compile-time configuration rules.

---

## Critical architectural rules

These rules must be respected when extending the firmware:

- Audio processing runs exclusively in the audio callback.
- The audio callback must be hard real-time safe.
- UI, MIDI, logging, and hardware IO run in the main loop.
- Communication between threads must be simple and one-directional.
- Hardware wiring is centralized in the system layer.

If these rules are violated, audio stability will be compromised.

---

## Stubbed vs implemented components

Important distinction:

Implemented and stable:
- App lifecycle and wiring
- HardwareConfig audio setup
- DspEngine passthrough
- MidiState (notes, sustain, CC, program)
- MidiManager event handlers (logic only)

Explicitly stubbed:
- DSP algorithms beyond passthrough
- MIDI hardware IO
- UI primitives (encoder, buttons, pots, LEDs)
- OLED rendering
- Logging backend
- Hardware pin mappings

Do not assume stubbed components are functional.

---

## Extension guidance

When asked to extend the firmware:

- Add DSP code only inside DspEngine.
- Add control logic in the main loop or App.
- Add hardware wiring in HardwareConfig.
- Add UI behavior in UiController and UI primitives.
- Add compile-time constants only in src/config/.

Avoid:
- Adding logic to main.cpp
- Mixing UI or MIDI into DSP code
- Introducing blocking calls in the audio callback

---

## Naming and responsibility conventions

The repository uses consistent naming:

- App: composition and lifecycle
- system: hardware setup
- dsp: audio-rate processing
- midi: control protocol state
- ui: human interaction
- config: compile-time constants
- util: shared helpers and types

Preserve these boundaries when adding new code.

---

## Why this file exists

LLMs tend to:
- Hallucinate missing features
- Blur real-time and non-real-time code
- Suggest unsafe audio patterns

This document exists to prevent that.

If you are an automated system reading this repository:
- Trust what exists in code
- Trust the documentation
- Do not assume features that are not explicitly implemented

---

## Summary

This is a safe, minimal Daisy Seed firmware template.
It is designed to be extended carefully and deliberately.

Read the documentation.
Respect the runtime model.
Do not guess.
