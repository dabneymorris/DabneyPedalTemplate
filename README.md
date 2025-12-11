Dabney Daisy Pedal Template

A clean, modular, professional firmware template for building Daisy Seed–based guitar pedals, synths, and audio processors.

This repository provides a reusable firmware skeleton with a clear architectural separation between DSP, UI, MIDI, hardware configuration, and system utilities. It is designed to scale from simple one-knob pedals to complex multi-voice digital processors.

Features

Modular architecture

The codebase is fully separated into functional layers:
	•	app/ – High-level app orchestration
	•	dsp/ – DSP engine and audio processing
	•	midi/ – MIDI parsing, note-state logic, CC & Program Change
	•	ui/ – Encoder, buttons, pots, toggles, LEDs, OLED
	•	system/ – Hardware configuration (sample rate, block size, pins)
	•	config/ – Editable build-time configuration constants
	•	util/ – Shared types and logging system

Real-world pedal ready
	•	Complete MIDI logic (notes, CC, sustain, program change)
	•	Expandable DSP engine
	•	Template UI controller
	•	Clean Makefile build system
	•	gitignore tuned for Daisy projects
	•	Fully compatible with DaisyToolchain

⸻

Directory Structure

src/
app/ – App-level orchestration
dsp/ – DSP engine (extend with voices, filters, shifters, etc.)
midi/ – MIDI state + MIDI manager
ui/ – Encoder, buttons, pots, switches, LEDs, OLED
system/ – Hardware configuration + clock/block size
config/ – Build, DSP, UI, and MIDI configuration headers
util/ – Shared types + logging stub
main.cpp – Entry point

libDaisy/ – Daisy core library (submodule or copied)
DaisySP/ – DaisySP library (submodule or copied)
Makefile – Build rules
README.md – This file

⸻

Requirements

Before building, install:

DaisyToolchain (ARM GCC, dfu-util):
https://github.com/electro-smith/DaisyToolchain

libDaisy and DaisySP:
Clone or copy these into the root of the project:

DabneyPedalTemplate/
libDaisy/
DaisySP/

⸻

Building the Firmware

From the project root:

make

To flash the Daisy Seed via USB DFU mode:

make program-dfu

If dfu-util reports an error, press RESET on the Daisy Seed and retry.

⸻

Customizing for Your Own Pedal

1. Modify DSP

Add your custom voices, filters, shifters, or routing inside:

src/dsp/DspEngine.cpp

2. Wire UI hardware

In HardwareConfig, define the pins for:
	•	Encoder
	•	Footswitches
	•	Buttons
	•	Potentiometers (ADC)
	•	Toggle switches
	•	LEDs
	•	OLED (I2C or SPI)

Then fill in the UI component initializers.

3. Enable MIDI

MidiManager already handles:
	•	Note On / Note Off
	•	Sustain pedal (CC 64)
	•	CC values
	•	Program Change
	•	MIDI channel filtering

Wire Daisy’s USB or UART MIDI events into:

HandleNoteOn(…)
HandleControlChange(…)
HandleProgramChange(…)

4. Add presets, voice allocation, and advanced UI

A preset system or voice manager fits naturally inside src/app/.

⸻

Creating a New Pedal Project

Once this repository is stable, mark it as a GitHub Template Repository.
Then create new pedal projects easily:

Use this template → Create new repository

Each new project starts with a clean, proven firmware base.

⸻

Logging System

A lightweight logging API is included:

LOGF(“Value: %d”, x);

Logging can be enabled or disabled in:

src/config/BuildConfig.h

⸻

Testing Audio

The template boots with a pass-through audio engine so you can immediately confirm Daisy hardware is functioning.

⸻

License

MIT — free for commercial and personal use.

⸻

About

This project serves as a professional Daisy firmware foundation for high-performance guitar pedals, synthesizers, and advanced DSP processors. Designed for extendability, clarity, and long-term maintainability.