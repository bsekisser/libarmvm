# setup

CFLAGS += -Os
CFLAGS += -march=native
export CFLAGS

LDLIBS += -Lgit/libarmvm -larmvm
LDLIBS += -Lgit/libarmcc -larmcc
LDLIBS += -Lgit/libarm -larm
LDLIBS += -Lgit/libbse -lbse
LDLIBS += -lcapstone
LDLIBS += -lstdc++
export LDLIBS

export SRC_DIR = source

export TARGETs = test.exe

include git/makefiles/common.mk
