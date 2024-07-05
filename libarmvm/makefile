CFLAGS += -O1 -march=native

LDLIBS += -Lgit/libarm -larm
LDLIBS += -Lgit/libbse -lbse
LDLIBS += -lcapstone

SRC_DIR=.
SRCS_ARMVM = $(wildcard armvm/*.c)
SRCS_CORE += $(wildcard core/*.c)

TARGET_LIB = libarmvm.a

VPATH = armvm
VPATH += core



include git/libbse/makefile.setup


OBJS = $(patsubst armvm/%.c, $(OBJ_DIR)/%.o, $(SRCS_ARMVM))
OBJS += $(patsubst core/%.c, $(OBJ_DIR)/%.o, $(SRCS_CORE))


.PHONY: all
all: $(TARGET_LIB)


$(OBJ_TARGET_LIB): git/libarm/libarm.a

$(OBJ_TARGET_LIB): git/libbse/libbse.a


include git/libbse/makefile.build
