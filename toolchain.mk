# For cross-archtiecture compilation, set these environment variables to something like
# "aarch64" and "aarch64-linux-gnu-" for arm64, if you're building FOR x86, then "x86_64" and "x86_64-linux-gnu-"
ARCH ?= $(shell uname -m)

ifeq ($(CROSS_COMPILE),)
ifeq ($(ARCH),x86_64)
CROSS_COMPILE := x86_64-linux-gnu-
endif
ifeq ($(ARCH),aarch64)
CROSS_COMPILE := aarch64-linux-gnu-
endif
endif # ifeq CROSS_COMPILE,

### Standard build utilities ###
MAKE ?= make
SHELL ?= /bin/sh
CC := $(CROSS_COMPILE)gcc
CXX := $(CROSS_COMPILE)g++
LD := $(CROSS_COMPILE)gcc
AR := $(CROSS_COMPILE)ar
RUSTC := rustc
PROTOC ?= protoc
PKG_CONFIG ?= pkg-config
CARGO ?= cargo
INSTALL ?= install

### Standard GNU/Linux Utilities ###
CP ?= cp
RM ?= rm
MKDIR ?= mkdir
TOUCH ?= touch
UNZIP ?= unzip
ZIP ?= zip