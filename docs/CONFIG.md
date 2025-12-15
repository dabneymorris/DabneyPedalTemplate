# Configuration System

This document describes the configuration headers used by the Daisy Pedal Template.
These headers define compile-time constants that control build behavior, DSP
settings, MIDI defaults, and UI timing.

This file should live at:
docs/CONFIG.md

---

## Overview

All configuration values in this template are:
- Compile-time constants
- Centralized in src/config/
- Intended to be stable and globally visible

Runtime state does not belong in config headers.

---

## Configuration Directory

Location:
src/config/

Files:
- BuildConfig.h
- DspConfig.h
- MidiConfig.h
- UiConfig.h

Each header owns a distinct configuration domain.

---

## BuildConfig.h

Purpose:
- Control build- and debug-related options.

Current contents:
- BUILD_DEBUG
- ENABLE_LOGGING

Notes:
- ENABLE_LOGGING only controls whether LOGF expands to a call or a no-op.
- A logging backend is not implemented in this template.

Guidelines:
- Keep this file small.
- Avoid adding feature flags that change runtime behavior dramatically.
- Prefer using build system flags for major build variants.

---

## DspConfig.h

Purpose:
- Define global audio parameters.

Current configuration:
- config::dsp::kSampleRate
- config::dsp::kBlockSize

These values are applied during HardwareConfig::Init() before audio starts.

Guidelines:
- Treat these as system-wide constants.
- Changing block size affects latency and CPU load.
- Changing sample rate affects all DSP code and DaisySP modules.

---

## MidiConfig.h

Purpose:
- Define default MIDI behavior.

Current configuration:
- config::midi::kDefaultChannel

Notes:
- Channel value meanings:
  - 0 = omni
  - 1–16 = specific MIDI channel

Guidelines:
- Keep MIDI defaults here.
- Do not store dynamic MIDI state in config headers.

---

## UiConfig.h

Purpose:
- Define UI timing and behavior constants.

Current configuration:
- config::ui::kEncoderDoubleClickMs

Intended future use:
- Debounce timing
- Hold thresholds
- UI animation timing
- Display layout constants

Guidelines:
- Store timing and tuning constants here.
- Avoid storing pin numbers or hardware handles.

---

## Configuration vs Runtime State

Configuration headers:
- Are read-only
- Do not change at runtime
- Are safe to include anywhere

Runtime state:
- Lives in App, DSP, MIDI, or UI modules
- Must not be placed in src/config/

Keeping this separation avoids hidden dependencies and side effects.

---

## Common Mistakes

- Putting runtime variables in config headers
- Using config headers as global state
- Adding hardware pin mappings to config files
- Mixing configuration and behavior

---

## Summary

- All compile-time constants live in src/config/
- Each config header has a single responsibility
- Config values are applied during initialization
- Runtime state belongs elsewhere

This structure keeps configuration predictable and maintainable.
