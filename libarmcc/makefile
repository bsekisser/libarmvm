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

export TARGETs = libarmcc.a libarmcc.so

include git/libbse/makefiles/common.mk
