# Project Name
TARGET = DabneyPedalTemplate

# Sources
CPP_SOURCES = \
  src/main.cpp \
  src/app/App.cpp \
  src/dsp/DspEngine.cpp \
  src/system/HardwareConfig.cpp \
  src/util/Log.cpp \
  src/midi/MidiState.cpp \
  src/midi/MidiManager.cpp \
  src/ui/Encoder.cpp \
  src/ui/Footswitch.cpp \
  src/ui/OledView.cpp \
  src/ui/UiController.cpp \
  src/ui/Button.cpp \
  src/ui/ToggleSwitch.cpp \
  src/ui/Potentiometer.cpp \
  src/ui/Led.cpp

# Library Locations (local to this repo)
LIBDAISY_DIR ?= libDaisy
DAISYSP_DIR  ?= DaisySP

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

# Add our src/ folder to the include search path
C_INCLUDES += -Isrc