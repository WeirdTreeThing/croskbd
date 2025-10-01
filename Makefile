
include toolchain.mk

# Static compilation handler
ifneq ($(STATIC),)
CFLAGS += -static
endif

BDIR ?= build/$(ARCH)

CPPFLAGS += -Iinclude -Isrc/include -I$(BDIR)

CFLAGS += -Wall -std=c2x -g -Wno-deprecated-declarations -Wno-parentheses \
	-Wno-stringop-overflow -Wno-pointer-sign -Wno-incompatible-pointer-types \
	-Wno-unused-const-variable -Wno-unused-value -Wno-missing-braces -Wno-unused-result \
	-Wno-zero-length-array

CXXFLAGS += -Wall -g -Wno-deprecated-declarations -Wno-parentheses \
	-Wno-stringop-overflow -Wno-unused-value

LDFLAGS += 

PROGS:= $(BDIR)/croskbd
SRCS  = $(BDIR)/config.o
SRCS += $(BDIR)/cros_ec.o
SRCS += $(BDIR)/croskbd.o
SRCS += $(BDIR)/evdev.o
SRCS += $(BDIR)/remap.o
SRCS += $(BDIR)/uinput.o
SRCS += $(BDIR)/utils.o
SRCS += $(BDIR)/vivaldi.o


# Use VERBOSE=1 for debug output
ifeq ($(VERBOSE),)
Q := @
else
Q :=
endif

all: $(BDIR) $(PROGS)

.PHONY: clean
clean:
	$(Q)$(RM) -rf build

.PHONY: install
install:
	$(Q)$(INSTALL) -d /usr/local/bin/
	$(Q)$(INSTALL) -m +rx $(PROGS) /usr/local/bin/

$(BDIR):
	$(Q)$(MKDIR) -p $@

$(BDIR)/%.o: src/%.c
	@echo "  CC      $(notdir $<)"
	$(Q)$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(BDIR)/%.o: src/%.cc
	@echo "  CXX     $(notdir $<)"
	$(Q)$(CXX) -c $< -o $@ $(CXXFLAGS) $(CPPFLAGS)

$(BDIR)/croskbd: $(SRCS)
	@echo "  LD      $(notdir $@)"
	$(Q)$(LD) $^ -o $@ $(LDFLAGS)