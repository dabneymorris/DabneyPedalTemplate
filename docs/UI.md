# User Interface System

This document describes the UI subsystem of the Daisy Pedal Template, including
the role of the UI controller, the purpose of UI primitives, and the intended
hardware wiring pattern.

This file should live at:
docs/UI.md

---

## Overview

The UI subsystem is intentionally split into two layers:

1. UiController — orchestration and high-level UI state
2. UI primitives — hardware abstractions (encoder, buttons, pots, LEDs, OLED)

All UI code runs in the main loop and must never execute in the audio thread.

---

## UiController

Source files:
- src/ui/UiController.h
- src/ui/UiController.cpp

### Purpose

UiController is responsible for:
- Initializing UI primitives
- Polling UI primitives each loop
- Tracking high-level UI state (pages, modes)
- Coordinating OLED rendering (once implemented)

UiController does NOT:
- Perform DSP
- Own hardware pin definitions
- Handle audio-rate logic

---

### Lifecycle

UiController::Init():
- Called once during App::Init()
- Initializes all UI primitives
- Sets the initial UI page

UiController::Update():
- Called repeatedly from App::Loop()
- Polls all UI primitives
- Will eventually update display output

---

## UI Primitives

UI primitives represent individual hardware controls or outputs.
Each primitive is responsible for its own hardware interaction and debouncing.

All primitives are currently implemented as explicit stubs.

---

### Encoder

Source files:
- src/ui/Encoder.h
- src/ui/Encoder.cpp

Intended responsibilities:
- Track rotary encoder delta
- Track encoder push-button state
- Provide edge and level queries

Current state:
- Stub implementation
- No GPIO wiring
- Always returns neutral values

---

### Button

Source files:
- src/ui/Button.h
- src/ui/Button.cpp

Intended responsibilities:
- Represent a momentary button
- Provide debounced edge (pressed/released) and level (held) states

Current state:
- Stub implementation
- No GPIO wiring or debouncing

---

### Footswitch

Source files:
- src/ui/Footswitch.h
- src/ui/Footswitch.cpp

Intended responsibilities:
- Represent a footswitch input
- Provide edge and level state
- Support latching or momentary behavior (future)

Current state:
- Stub implementation
- No GPIO wiring or debouncing

---

### ToggleSwitch

Source files:
- src/ui/ToggleSwitch.h
- src/ui/ToggleSwitch.cpp

Intended responsibilities:
- Represent a two-position toggle switch
- Expose a stable position enum

Current state:
- Stub implementation
- No GPIO wiring

---

### Potentiometer

Source files:
- src/ui/Potentiometer.h
- src/ui/Potentiometer.cpp

Intended responsibilities:
- Represent an analog control
- Read from an ADC channel
- Apply smoothing or filtering
- Expose a normalized value (0.0–1.0)

Current state:
- Stub implementation
- No ADC configuration or sampling

---

### Led

Source files:
- src/ui/Led.h
- src/ui/Led.cpp

Intended responsibilities:
- Represent an LED output
- Support on/off or PWM brightness

Current state:
- Stub implementation
- No GPIO or PWM wiring

---

### OledView

Source files:
- src/ui/OledView.h
- src/ui/OledView.cpp

Intended responsibilities:
- Wrap an OLED display driver
- Draw text and graphics
- Present rendered frames to the display

Current state:
- Stub implementation
- No display driver or bus configured

---

## Hardware Wiring Pattern (Intended)

Recommended approach when wiring real hardware:

1. Define all pins and peripheral handles in HardwareConfig
2. Pass configured handles or pin identifiers to UI primitives
3. Implement Init() and Update() inside each primitive
4. Keep UiController focused on behavior, not wiring

This keeps hardware concerns centralized and UI logic clean.

---

## Threading Rules

UI code:
- Must run in the main loop
- Must never execute in the audio callback

UI primitives:
- May access GPIO, ADC, I2C, SPI
- May allocate memory if needed (sparingly)

---

## Common Mistakes

- Reading UI controls in the audio callback
- Performing OLED rendering from DSP code
- Mixing pin definitions into UiController
- Allowing UI primitives to own application logic

---

## Summary

- UiController orchestrates UI behavior
- UI primitives abstract individual hardware elements
- All UI code runs outside the audio thread
- Hardware wiring is intentionally deferred in this template

This structure keeps UI extensible without compromising audio stability.
