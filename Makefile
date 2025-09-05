# Project Name
TARGET = TestTFT

# Sources
CPP_SOURCES = TestTFT.cpp TFT_SPI.cpp Frame.cpp GFX.cpp Pap.cpp Buttons.cpp Menu.cpp Spirits.cpp Sound.cpp ChordLibrary.cpp

# Library Locations
LIBDAISY_DIR = ./libDaisy
DAISYSP_DIR = ./DaisySP

# Define application type for bootloader


# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
