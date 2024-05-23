CFLAGS += -O2 -march=native

LDLIBS += -Lgit/libarmvm -larmvm
LDLIBS += -Lgit/libarm -larm
LDLIBS += -Lgit/libbse -lbse
LDLIBS += -lcapstone

SRC_DIR = source
SRCS_CPP = $(wildcard $(SRC_DIR)/*.cpp)

TARGET_LIB = libarmcc.a


.PHONY: all
all: $(TARGET_LIB)


include ../makefile.setup
include ../makefile.build
