# setup

CFLAGS += -O2
CFLAGS += -march=native
export CFLAGS

LDLIBS += -Lgit/libarmvm -larmvm
LDLIBS += -Lgit/libarm -larm
LDLIBS += -Lgit/libbse -lbse
LDLIBS += -lcapstone
export LDLIBS

export SRC_DIR = source

export TARGET = libarmcc

include git/libbse/makefiles/common_setup.mk

# build/recipies

.PHONY: all
all:
	$(MAKE) -f git/libbse/makefiles/build_liba.mk all
	$(MAKE) -f git/libbse/makefiles/build_libso.mk all

include git/libbse/makefiles/common.mk
