CFLAGS += -O1
CFLAGS += -fvisibility=hidden
CFLAGS += -march=native
export CFLAGS

#LDLIBS += -L/home/$(USER)/.local/lib
#LDLIBS += -Lgit/libbse
	LDLIBS += -lbse
LDLIBS += -lcapstone
export LDLIBS

export SRC_DIR = source

TARGETs = libarm.a libarm.so

include git/makefiles/common.mk
