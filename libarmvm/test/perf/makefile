#setup

CFLAGS += -O2
CFLAGS += -march=native
export CFLAGS

LDLIBS += -Lgit/libarmvm -larmvm
LDLIBS += -Lgit/libarmcc -larmcc
LDLIBS += -Lgit/libarm -larm
LDLIBS += -Lgit/libbse -lbse
LDLIBS += -lcapstone
export LDLIBS

export SRC_DIR = source

export TARGET_EXE = test_armvm_perf

# build/recipes

all: $(TARGET_EXE)


include git/libbse/makefiles/common_setup.mk
include git/libbse/makefiles/build_exe.mk
