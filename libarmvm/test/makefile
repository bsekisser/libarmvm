CFLAGS += -Os -march=native

LDLIBS += -Lgit/libarmvm -larmvm
LDLIBS += -Lgit/libarmcc -larmcc
LDLIBS += -Lgit/libarm -larm
LDLIBS += -Lgit/libbse -lbse
LDLIBS += -lcapstone

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


include ../../makefile.setup
include ../../makefile.build
