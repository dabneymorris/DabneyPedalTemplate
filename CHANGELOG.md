[0.1.0] – 2025-01-01

Added
	•	Initial project structure and directory layout.
	•	Complete modular firmware skeleton for Daisy Seed projects:
	•	app/ – High-level application control
	•	dsp/ – DSP engine framework
	•	midi/ – MIDI parsing, note-state logic, and channel/CC/program filtering
	•	ui/ – Encoder, footswitch, button, potentiometer, toggle, LED, and OLED abstractions
	•	system/ – Hardware configuration layer
	•	config/ – Build, DSP, UI, and MIDI configuration headers
	•	util/ – Shared types and logging layer
	•	Hardware pass-through audio engine (baseline audio test).
	•	README.md describing setup, structure, and customization.
	•	LICENSE (MIT License).

Notes
	•	No hardware wiring implemented yet (UI, MIDI I/O, GPIO, ADC, OLED).
	•	DSP engine contains pass-through placeholder; real DSP to be added in future versions.
	•	Project intended as a reusable template for multiple Daisy pedals.

⸻

[Unreleased]

Added
	•	(To be documented as the project evolves.)