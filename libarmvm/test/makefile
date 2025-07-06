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

export TARGET_EXE = test

include git/libbse/makefiles/common_setup.mk


# build/recipies


all: $(TARGET_EXE)


$(OBJ_TARGET_EXE): git/libarmvm/libarmvm.a

$(OBJ_TARGET_EXE): git/libarmcc/libarmcc.a

$(OBJ_TARGET_EXE): git/libarm/libarm.a

$(OBJ_TARGET_EXE): git/libbse/libbse.a


include git/libbse/makefiles/build_exe.mk
