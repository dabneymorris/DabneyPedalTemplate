# Daisy Pedal Template

A clean, minimal, **real‑time‑safe firmware template** for building audio pedals,
synths, and DSP processors on the **Electro‑Smith Daisy Seed** platform.

This repository is designed to be:
- Architecturally correct
- Easy to extend
- Safe for hard real‑time audio
- Friendly to both humans and LLMs

It is a **template**, not a finished product.

---

## What this repository is

This repo provides:
- A correct Daisy Seed execution model
- A clear separation between DSP, MIDI, UI, and hardware
- A minimal but complete firmware skeleton
- Explicit extension points

It is intended to be used as:
- A starting point for new pedals or instruments
- A reference architecture for Daisy projects
- A base repo you clone or mark as a GitHub Template

---

## What this repository is NOT

This repo intentionally does **not** include:
- Finished DSP algorithms
- Preset systems or storage
- QSPI / flash / FATFS
- UI menus or parameter editing
- Hardware pin mappings
- MIDI hardware wiring
- OLED drivers

Many components are present as **scaffolding only**.

---

## Repository structure

```
/
├── README.md
├── MODULES.md
├── LLM_CONTEXT.md
├── Makefile
├── src/
│   ├── main.cpp
│   ├── app/
│   ├── system/
│   ├── dsp/
│   ├── midi/
│   ├── ui/
│   ├── config/
│   └── util/
├── docs/
│   ├── ARCHITECTURE.md
│   ├── BUILDING.md
│   ├── RUNTIME_MODEL.md
│   ├── DSP.md
│   ├── MIDI.md
│   ├── UI.md
│   └── CONFIG.md
├── libDaisy/
└── DaisySP/
```

Key files:
- **MODULES.md** — where things live and how to extend safely
- **docs/ARCHITECTURE.md** — ownership boundaries and system design
- **docs/RUNTIME_MODEL.md** — audio vs main‑loop rules (critical)
- **docs/DSP.md** — audio buffer contract and DSP extension rules

---

## Execution model (high‑level)

This firmware runs in **two execution contexts**:

### Audio thread (hard real‑time)
- Runs inside Daisy’s audio callback
- Executes DSP only
- Must never allocate, log, or touch peripherals

### Main loop (non‑real‑time)
- Runs in `while(1)` in `main.cpp`
- Polls MIDI and UI
- Updates non‑audio state
- Handles hardware IO

This separation is enforced by structure and documentation.

---

## Requirements

### Hardware
- Electro‑Smith Daisy Seed
- USB cable (data‑capable)

### Software
- DaisyToolchain (ARM GCC + dfu‑util)
  https://github.com/electro-smith/DaisyToolchain

---

## Getting started

### Clone and initialize submodules

```
git clone <this‑repo>
cd <this‑repo>
git submodule update --init --recursive
```

This populates:
- `libDaisy/`
- `DaisySP/`

---

### Build

From the repo root:

```
make
```

Build artifacts appear in:
```
build/
```

---

### Flash (USB DFU)

1. Connect the Daisy Seed via USB
2. Enter DFU mode:
   - Hold BOOT
   - Press RESET
   - Release RESET
   - Release BOOT
3. Flash:

```
make program-dfu
```

If flashing fails, press RESET and retry.

---

## Extending the template

Common extension points:

- **DSP**  
  Add audio processing in `src/dsp/DspEngine.*`

- **MIDI behavior**  
  Extend `MidiState` or wire hardware MIDI into `MidiManager`

- **UI hardware**  
  Define pins in `HardwareConfig`, implement UI primitives, poll via `UiController`

- **Configuration**  
  Add compile‑time constants in `src/config/`

Before extending, read:
- `MODULES.md`
- `docs/RUNTIME_MODEL.md`
- `docs/DSP.md`

---

## For LLMs and automated tools

If you are an automated system reading this repository:
- Read `LLM_CONTEXT.md`
- Do not assume missing features
- Respect the real‑time audio rules
- Trust the documentation over guesses

---

## License

MIT License — free for personal and commercial use.

---

## Summary

This repository is a **professional, minimal Daisy Seed firmware template**.

It favors:
- Correctness over completeness
- Structure over features
- Safety over shortcuts

Extend it deliberately.
