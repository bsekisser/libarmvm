CFLAGS += -O1
CFLAGS += -march=native
export CFLAGS

LDLIBS += -Lgit/libbse -lbse
LDLIBS += -lcapstone
export LDLIBS

export SRC_DIR = source

export TARGET = libarm

include git/libbse/makefiles/common_setup.mk

.PHONY: all
all:
	$(MAKE) -f git/libbse/makefiles/build_liba.mk all
	$(MAKE) -f git/libbse/makefiles/build_libso.mk all

include git/libbse/makefiles/common.mk
