CFLAGS = -O1
CFLAGS += -fvisibility=hidden
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

export SRC_DIR = source

TARGETs = libarmvm.a libarmvm.so

include git/makefiles/common.mk
