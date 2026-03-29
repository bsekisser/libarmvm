CFLAGS = -O1
CFLAGS += -march=native
export CFLAGS

#INCLUDE = -Iarmvm
#INCLUDE += -Icore
#export INCLUDE

#LDLIBS += -L/home/$(USER)/.local/lib
#LDLIBS += -Lgit/libarm
	LDLIBS += -larm
#LDLIBS += -Lgit/libbse
	LDLIBS += -lbse
LDLIBS += -lcapstone
export LDLIBS

SRC_DIR = armvm
SRC_DIR += core
export SRC_DIR

TARGETs = libarmvm.a libarmvm.so

include git/makefiles/common.mk
