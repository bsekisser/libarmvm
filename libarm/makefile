CFLAGS += -O1 -march=native
CFLAGS += -fPIC

LDFLAGS += -shared

LDLIBS += -Lgit/libbse -lbse
LDLIBS += -lcapstone

SRC_DIR = source
SRCS = $(wildcard $(SRC_DIR)/*.c)

TARGET_LIB = libarm
TARGET_LIB_A = $(TARGET_LIB).a
TARGET_LIB_SO = $(TARGET_LIB).so


.PHONY: all
all: $(TARGET_LIB_SO)

$(TARGET_LIB_SO): $(TARGET_LIB_A)

include git/libbse/makefile.setup

$(OBJ_TARGET_LIB_A): git/libbse/libbse.so

$(OBJ_TARGET_LIB_SO): $(OBJ_TARGET_LIB_A)

include git/libbse/makefile.build
