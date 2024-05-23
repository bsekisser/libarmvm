CFLAGS += -O2 -march=native

#INCLUDE= -Isource

LDLIBS += -Lgit/libarmvm -larmvm
LDLIBS += -Lgit/libarmcc -larmcc
LDLIBS += -Lgit/libarm -larm
LDLIBS += -Lgit/libbse -lbse
LDLIBS += -lcapstone

SRC_DIR = source
SRCS_CPP = $(wildcard $(SRC_DIR)/*.cpp)

TARGET_EXE = test_armvm_perf


all: $(TARGET_EXE)


include ../../../makefile.setup
include ../../../makefile.build
