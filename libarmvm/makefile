CFLAGS = -O1
CFLAGS += -march=native
export CFLAGS

INCLUDE = -Iarmvm
INCLUDE += -Icore
export INCLUDE

LDLIBS = -Lgit/libarm -larm
LDLIBS += -Lgit/libbse -lbse
LDLIBS += -lcapstone
export LDLIBS

SRC_DIR = armvm
SRC_DIR += core
export SRC_DIR

export TARGET = libarmvm

include git/libbse/makefiles/common_setup.mk

.PHONY: all
all:
	$(MAKE) -f git/libbse/makefiles/build_liba.mk all
	$(MAKE) -f git/libbse/makefiles/build_libso.mk all

$(OBJ_TARGET_LIB_A): git/libarm/libarm.so

$(OBJ_TARGET_LIB_A): git/libbse/libbse.so

include git/libbse/makefiles/common.mk
