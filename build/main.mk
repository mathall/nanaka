OS ?= linux
TARGET_ARCH ?= x86
DEBUG ?= 1
VERBOSE ?= 0
USE_CLANG ?= $(shell command -v clang > /dev/null && echo 1)
USE_NINJA ?= $(shell command -v ninja > /dev/null && echo 1)
ifeq ($(OS), osx)
PARALLELISM ?= $(shell sysctl -n hw.ncpu)
else
PARALLELISM ?= $(shell grep -ic 'processor.*:' /proc/cpuinfo)
endif

ifeq ($(VERBOSE), 1)
ifeq ($(USE_NINJA), 1)
VERBOSITY += -v
else
VERBOSITY += V=1
endif
endif

GYP_FLAGS := --depth=$(SOURCE_ROOT)
GYP_FLAGS += -DOS=$(OS)
GYP_FLAGS += -Duse_clang=$(USE_CLANG)
GYP_FLAGS += -I../../../nanaka/build/common.gypi

ifeq ($(DEBUG), 1)
BUILDTYPE := Debug
else
BUILDTYPE := Release
endif

TARGET_ARCH ?= x86
ARM_VERSION ?= 7

GYP_FLAGS += -Dtarget_arch=$(TARGET_ARCH)
ifeq ($(TARGET_ARCH), arm)
GYP_FLAGS += -Darm_version=$(ARM_VERSION)
endif

OUT_DIR := out_$(OS)_$(TARGET_ARCH)
ifeq ($(TARGET_ARCH), arm)
OUT_DIR := $(OUT_DIR)v$(ARM_VERSION)
endif
GYP_FLAGS += -Goutput_dir=$(OUT_DIR)

ifeq ($(USE_NINJA), 1)
GYP_FLAGS += -fninja
BUILD_TOOL := ninja
BUILD_DIR := $(SOURCE_ROOT)/$(OUT_DIR)/$(BUILDTYPE)
else
BUILD_TOOL := $(MAKE)
BUILD_DIR := $(SOURCE_ROOT)
BUILDTYPE_FLAG := BUILDTYPE=$(BUILDTYPE)
endif

nanaka:
	$(BUILD_TOOL) -C $(BUILD_DIR) -j$(PARALLELISM) $(VERBOSITY) $(BUILDTYPE_FLAG)
