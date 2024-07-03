CFLAGS += -O1 -march=native

LDLIBS += -Lgit/libbse -lbse
LDLIBS += -lcapstone

SRC_DIR = source
SRCS = $(wildcard $(SRC_DIR)/*.c)

TARGET_LIB = libarm.a


.PHONY: all
all: $(TARGET_LIB)


include git/libbse/makefile.setup
include git/libbse/makefile.build
