# setup

CFLAGS += -Os
CFLAGS += -march=native
export CFLAGS

INCLUDE += -Igit/libarmvm/include
INCLUDE += -Igit/libarmvm/armvm
INCLUDE += -Igit/libarmvm/core
INCLUDE += -Igit/libarm
export INCLUDE

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

test: test.exe
	./test.exe
