# Building and Flashing the Daisy Pedal Template

This document describes how to build, flash, and debug this Daisy Seed firmware
template using the official Daisy toolchain and the provided Makefile.

This file should live at:
docs/BUILDING.md

---

## Requirements

### Hardware
- Electro-Smith Daisy Seed
- USB cable capable of data (for DFU flashing)

### Software
- DaisyToolchain (ARM GCC + dfu-util)
  https://github.com/electro-smith/DaisyToolchain

The toolchain must provide:
- arm-none-eabi-gcc
- arm-none-eabi-g++
- arm-none-eabi-objcopy
- dfu-util
- (optional) openocd and arm-none-eabi-gdb

All tools must be available on your system PATH.

---

## Repository setup

This repository expects libDaisy and DaisySP to be present locally.

### Using git submodules (recommended)

After cloning the repo:

git submodule update --init --recursive

This will populate:
- libDaisy/
- DaisySP/

### Manual copy (not recommended)

Alternatively, you may clone or copy the repositories manually:

https://github.com/electro-smith/libDaisy
https://github.com/electro-smith/DaisySP

They must live at the repo root:

repo/
- libDaisy/
- DaisySP/
- src/
- Makefile

---

## Build system overview

The top-level Makefile is a thin wrapper that:
- Defines the project name (TARGET)
- Collects all source files under src/
- Includes libDaisy’s core Makefile

Key variables (from the Makefile):

- TARGET = DabneyPitchPedal
- LIBDAISY_DIR ?= libDaisy
- DAISYSP_DIR  ?= DaisySP

You may override library paths at build time if needed:

LIBDAISY_DIR=/path/to/libDaisy DAISYSP_DIR=/path/to/DaisySP make

---

## Building the firmware

From the repository root:

make

On success, build artifacts are placed in:

build/

Typical outputs include:
- build/DabneyPitchPedal.elf
- build/DabneyPitchPedal.bin
- build/DabneyPitchPedal.hex

---

## Flashing via USB DFU (recommended)

1. Connect the Daisy Seed via USB.
2. Put the Daisy into DFU mode:
   - Hold BOOT
   - Press RESET
   - Release RESET
   - Release BOOT

3. Flash using:

make program-dfu

If dfu-util reports an error:
- Press RESET on the Daisy Seed
- Retry the command

---

## Flashing via OpenOCD (advanced)

If you are using an external debugger (ST-Link, J-Link):

make program

Notes:
- APP_TYPE must be BOOT_NONE for OpenOCD flashing
- OpenOCD configuration is inherited from libDaisy

---

## Debugging

### VS Code (recommended)

This repository includes VS Code task definitions in:
.vscode/tasks.json

Available tasks include:
- build
- build_and_program
- build_and_program_dfu
- build_all
- build_all_debug
- program
- program-dfu

To debug with Cortex-Debug:
- Ensure OpenOCD is installed
- Update .vscode/launch.json if necessary

Important:
The default launch.json may reference a placeholder ELF name.
Ensure it matches:
build/DabneyPitchPedal.elf

---

## Common issues

### Build fails with missing arm-none-eabi tools
- Verify DaisyToolchain is installed
- Verify PATH is correctly set

### dfu-util cannot find device
- Confirm Daisy is in DFU mode
- Try pressing RESET and retrying
- Try a different USB cable or port

### Debugger cannot find ELF
- Ensure a successful build occurred
- Verify TARGET name matches launch.json

---

## Notes

- Audio sample rate and block size are configured in:
  src/config/DspConfig.h
- Hardware wiring (GPIO, ADC, MIDI, OLED) is not implemented in this template
- This template is intended as a clean starting point, not a finished pedal

---

## Summary

This build system follows standard Daisy Seed conventions and is intentionally minimal.
Once the toolchain is installed and submodules are initialized, building and flashing
should be a single-command operation.
