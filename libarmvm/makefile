CFLAGS += -O1 -march=native
CFLAGS += -fPIC

INCLUDE += -Iarmvm
INCLUDE += -Icore

LDFLAGS += -shared

LDLIBS += -Lgit/libarm -larm
LDLIBS += -Lgit/libbse -lbse
LDLIBS += -lcapstone

SRC_DIR=.
SRCS_ARMVM = $(wildcard armvm/*.c)
SRCS_CORE += $(wildcard core/*.c)

TARGET_LIB = libarmvm
TARGET_LIB_A = $(TARGET_LIB).a
TARGET_LIB_SO = $(TARGET_LIB).so

VPATH = armvm
VPATH += core


.PHONY: all
all: $(TARGET_LIB_SO)


include git/libbse/makefile.setup

OBJS = $(patsubst armvm/%.c, $(OBJ_DIR)/%.o, $(SRCS_ARMVM))
OBJS += $(patsubst core/%.c, $(OBJ_DIR)/%.o, $(SRCS_CORE))

$(OBJ_TARGET_LIB_A): git/libarm/libarm.so

$(OBJ_TARGET_LIB_A): git/libbse/libbse.so

$(OBJ_TARGET_LIB_SO): $(OBJ_TARGET_LIB_A)

$(TARGET_LIB_SO): $(TARGET_LIB_A)

include git/libbse/makefile.build
