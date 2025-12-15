# Project Name
TARGET = DabneyPitchPedal

# ----------------------------------------------------------------
# C++ source files
# ----------------------------------------------------------------
CPP_SOURCES = \
    $(wildcard src/*.cpp) \
    $(wildcard src/app/*.cpp) \
    $(wildcard src/config/*.cpp) \
    $(wildcard src/dsp/*.cpp) \
    $(wildcard src/midi/*.cpp) \
    $(wildcard src/music/*.cpp) \
    $(wildcard src/presets/*.cpp) \
    $(wildcard src/ui/*.cpp) \
    $(wildcard src/settings/*.cpp) \
    $(wildcard src/system/*.cpp) \
    $(wildcard src/util/*.cpp)

# Library Locations (local to this repo)
LIBDAISY_DIR ?= libDaisy
DAISYSP_DIR  ?= DaisySP

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

# Add our src/ folder to the include search path
C_INCLUDES += -Isrc