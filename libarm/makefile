CFLAGS += -O2 -march=native

LDLIBS += -Lgit/libbse -lbse
LDLIBS += -lcapstone

SRC_DIR = source
SRCS = $(wildcard $(SRC_DIR)/*.c)

TARGET_LIB = libarm.a


.PHONY: all
all: $(TARGET_LIB)


include ../makefile.setup
include ../makefile.build
