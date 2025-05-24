CFLAGS += -O2 -march=native

CPPFLAGS = $(CFLAGS)

#INCLUDE = -Isource

LDLIBS += -Lgit/libarmvm -larmvm
LDLIBS += -Lgit/libarmcc -larmcc
LDLIBS += -Lgit/libarm -larm
LDLIBS += -Lgit/libbse -lbse
LDLIBS += -lcapstone

SRC_DIR = source
SRCS_CPP = $(wildcard $(SRC_DIR)/*.cpp)

TARGET_EXE = test_armvm_perf


all: $(TARGET_EXE)


include git/libbse/makefile.setup
include git/libbse/makefile.build
