CFLAGS += -Os -march=native

CPPFLAGS = $(CFLAGS)

LDLIBS += -Lgit/libarmvm -larmvm
LDLIBS += -Lgit/libarmcc -larmcc
LDLIBS += -Lgit/libarm -larm
LDLIBS += -Lgit/libbse -lbse
LDLIBS += -lcapstone
LDLIBS += -lstdc++


INCLUDE += -Iinclude
INCLUDE += -Igit
INCLUDE += -I..

VPATH += source
VPATH += git/libarmvm

SRC_DIR = source
SRCS = $(wildcard $(SRC_DIR)/*.c)
SRCS_CPP = $(wildcard $(SRC_DIR)/*.cpp)

TARGET_EXE = test


all: $(TARGET_EXE)


include git/libbse/makefile.setup

$(OBJ_TARGET_EXE): git/libarmvm/libarmvm.a

$(OBJ_TARGET_EXE): git/libarmcc/libarmcc.a

$(OBJ_TARGET_EXE): git/libarm/libarm.a

$(OBJ_TARGET_EXE): git/libbse/libbse.a

include git/libbse/makefile.build
