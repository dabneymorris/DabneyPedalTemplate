# MODULES

This document describes the module layout of this Daisy Seed firmware template, what each module owns, and where to extend the repo safely.

Repository goals:
- Keep audio processing hard real-time safe.
- Keep responsibilities separated (DSP vs MIDI vs UI vs hardware wiring).
- Make extension points obvious for humans and LLMs.

---

## Quick orientation

Primary entry point:
- src/main.cpp

High-level composition root:
- src/app/App.h

Core subsystems:
- src/system/HardwareConfig.h
- src/dsp/DspEngine.h
- src/midi/MidiState.h
- src/midi/MidiManager.h
- src/ui/UiController.h

Configuration headers:
- src/config/*.h

Shared utilities:
- src/util/*.h

Third-party dependencies (submodules):
- libDaisy/ (hardware + audio framework)
- DaisySP/ (DSP utilities and modules)

---

## Real-time boundaries (non-negotiable)

Audio thread (hard real-time):
- Runs inside the Daisy audio callback.
- Only DSP should run here (DspEngine and code it calls).

Must NOT happen in the audio thread:
- Memory allocation
- Logging / printf
- I2C / SPI / UART / USB operations
- OLED drawing or GPIO polling
- Any blocking/sleeping

Main loop (non-real-time):
- Runs in while(1) in main.cpp via App::Loop().
- This is where you poll MIDI/UI, update state, and (eventually) render the OLED.

---

## Directory modules

### src/main.cpp
Purpose:
- Configure and initialize the Daisy Seed hardware object.
- Start the audio engine.
- Run the infinite main loop.

Owns:
- The global DaisySeed instance (hw)
- The global App instance (app)
- The audio callback trampoline that forwards to App

Extension guidance:
- Keep main.cpp minimal.
- Do not implement business logic here; put it in App or lower modules.

---

### src/app/
Files:
- src/app/App.h
- src/app/App.cpp

Purpose:
- App is the composition root.
- Owns subsystem lifecycles and the call order of the system.

Owns:
- HardwareConfig instance
- DspEngine instance
- MidiState and MidiManager instances
- UiController instance

Key responsibilities:
- Init(daisy::DaisySeed&): initialize subsystems in a safe order.
- AudioCallback(...): forward buffers to DSP only.
- Loop(): poll MIDI and UI (and later other non-audio tasks).

Extension points:
- Add additional subsystems (e.g., a parameter model, routing state, a simple “app state” struct).
- Define the canonical order of non-audio updates in Loop().

Avoid:
- Putting DSP code into App::AudioCallback; keep it inside DspEngine.
- Calling UI/MIDI/peripheral operations from AudioCallback.

---

### src/system/
Files:
- src/system/HardwareConfig.h
- src/system/HardwareConfig.cpp

Purpose:
- Centralize hardware-related configuration and setup.

Current implementation:
- Sets audio block size and sample rate from src/config/DspConfig.h
- Stores sample rate and block size for downstream use
- Stores a pointer to the DaisySeed object

Extension points (intended future wiring location):
- Pin mappings (GPIO pins for buttons, LEDs, relays, etc.)
- ADC channel definitions and initialization
- I2C/SPI bus setup for OLED
- MIDI UART configuration

Recommended pattern:
- Keep pin constants and peripheral handles here (or here plus small dedicated system submodules).
- Provide getters to hand the configured handles to the UI and MIDI modules.

---

### src/dsp/
Files:
- src/dsp/DspEngine.h
- src/dsp/DspEngine.cpp

Purpose:
- All audio-rate DSP processing belongs here.

Current implementation:
- Stereo interleaved passthrough.

Buffer contract:
- Input and output buffers are interleaved stereo floats:
  index 0 = L0, 1 = R0, 2 = L1, 3 = R1, ...

Extension points:
- Replace passthrough with your DSP chain.
- Add internal DSP modules/voices.
- Add parameter smoothing and per-block updates.

Real-time rules:
- DspEngine::ProcessInterleaved must stay hard real-time safe.
- Do not log, allocate, or touch peripherals here.

Suggested growth pattern:
- Add a lightweight “DSP parameters snapshot” struct that is updated in the main loop and read atomically in the audio thread (without locks or allocations).
- If you later add shared state between threads, keep it simple and safe (avoid mutexes in audio).

---

### src/midi/
Files:
- src/midi/MidiState.h
- src/midi/MidiState.cpp
- src/midi/MidiManager.h
- src/midi/MidiManager.cpp

Purpose:
- MIDI module is split into:
  - MidiState: the authoritative state model (notes, CC, program, sustain).
  - MidiManager: message ingestion, filtering, and updating MidiState.

MidiState (implemented):
- Tracks:
  - per-note down/sustained/velocity/age
  - sustain pedal behavior (CC64)
  - last-seen CC values (0–127)
  - last Program Change (0–127)
- Provides:
  - IsNoteActive(note)
  - GetMostRecentActiveNote()

MidiManager (partially implemented):
- Implements handlers:
  - HandleNoteOn
  - HandleNoteOff
  - HandleControlChange (includes sustain handling)
  - HandleProgramChange
- Channel filtering:
  - listen_chan_ = 0 means omni
  - 1–16 means specific channel

Current limitation:
- MidiManager::Process() is a stub (no hardware MIDI IO is wired yet).

Extension points:
- Wire actual Daisy MIDI input in MidiManager::Process() (or via Daisy callbacks).
- Expand MidiState for additional concepts (e.g., pitch bend, aftertouch) if needed.
- Provide a stable “voice intent” layer on top of MidiState for DSP consumption (in a future module, not inside DSP).

Threading note:
- Updating MidiState should happen in the main loop.
- The audio thread should only read a stable snapshot/state.

---

### src/ui/
Files:
- src/ui/UiController.h
- src/ui/UiController.cpp
- src/ui/OledView.h
- src/ui/OledView.cpp
- src/ui/Encoder.h / .cpp
- src/ui/Button.h / .cpp
- src/ui/Footswitch.h / .cpp
- src/ui/ToggleSwitch.h / .cpp
- src/ui/Potentiometer.h / .cpp
- src/ui/Led.h / .cpp

Purpose:
- UI is currently scaffolded into:
  - UiController: orchestrates polling and (later) rendering.
  - UI primitives: device abstractions (encoder/button/pot/etc.).
  - OledView: placeholder for display rendering.

Current implementation:
- UiController calls Init() once and Update() repeatedly.
- All UI primitives are stubs; no GPIO/ADC/debounce is implemented yet.
- OledView is a stub; no driver/bus is implemented.

Extension points:
- Add actual hardware wiring by:
  - Defining pins/buses in HardwareConfig
  - Implementing Init()/Update() in each UI primitive using libDaisy GPIO/ADC helpers
  - Implementing OledView using a chosen display driver and bus (I2C/SPI)

Recommended responsibilities split:
- UI primitives:
  - Own their hardware handles and debouncing/smoothing.
  - Expose clean edge/level/value APIs.
- UiController:
  - Owns page state and routing of user interaction.
  - Calls OledView to render.
  - Does not implement DSP itself.

Threading note:
- UI polling and OLED rendering must remain in the main loop, not the audio callback.

---

### src/config/
Files:
- src/config/BuildConfig.h
- src/config/DspConfig.h
- src/config/MidiConfig.h
- src/config/UiConfig.h

Purpose:
- Central place for compile-time constants.

Current configuration knobs:
- BuildConfig.h:
  - BUILD_DEBUG
  - ENABLE_LOGGING (logging backend not implemented yet)
- DspConfig.h:
  - config::dsp::kSampleRate (48 kHz)
  - config::dsp::kBlockSize (16)
- MidiConfig.h:
  - config::midi::kDefaultChannel (1)
- UiConfig.h:
  - config::ui::kEncoderDoubleClickMs (250 ms)

Extension points:
- Add only stable, global constants here (not runtime state).
- Prefer enums/constexpr values over macros when possible (except when interacting with build flags).

---

### src/util/
Files:
- src/util/Types.h
- src/util/Log.h
- src/util/Log.cpp

Purpose:
- Shared types and lightweight cross-module helpers.

Types.h:
- types::BypassState
- types::UiPage

Log.h:
- LOGF macro and logutil::Logf declaration.

Current limitation:
- Log.cpp is not implemented yet, so LOGF does not produce output.

Extension points:
- Implement logutil::Logf using a Daisy logging mechanism (or UART) in Log.cpp.
- Keep logging out of the audio thread even after implementation.

---

## Extension checklist (quick)

If you want to add…
- A new DSP effect: edit src/dsp/DspEngine.* (and keep it RT safe).
- A new MIDI behavior: extend MidiState and/or call MidiManager handlers from real MIDI IO.
- A new hardware control: add pins/handles in HardwareConfig, implement the UI primitive, then poll it in UiController.
- A new UI page/menu: add state to UiController (and a render path in OledView once implemented).
- A new compile-time knob: add it to src/config/ (then use it in system/dsp/midi/ui).

---

## Naming and organization conventions

This template intentionally keeps:
- “composition” in App
- “hardware setup” in system
- “audio code” in dsp
- “control protocol state” in midi
- “human interaction” in ui
- “constants” in config
- “shared helpers/types” in util

Try to preserve this separation as the project grows.
