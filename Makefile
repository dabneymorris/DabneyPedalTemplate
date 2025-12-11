# Project Name
TARGET = DabneyPedalTemplate

# Sources
CPP_SOURCES = \
  src/main.cpp \
  src/app/App.cpp \
  src/dsp/DspEngine.cpp \
  src/system/HardwareConfig.cpp

# Library Locations (local to this repo)
LIBDAISY_DIR ?= libDaisy
DAISYSP_DIR  ?= DaisySP

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

# Add our src/ folder to the include search path
C_INCLUDES += -Isrc