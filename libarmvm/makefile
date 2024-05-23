CFLAGS += -O2 -march=native

SRC_DIR=.
SRCS_ARMVM = $(wildcard armvm/*.c)
SRCS_CORE += $(wildcard core/*.c)

TARGET = libarmvm.a

VPATH = armvm
VPATH += core



include ../makefile.setup


OBJS = $(patsubst armvm/%.c, $(OBJ_DIR)/%.o, $(SRCS_ARMVM))
OBJS += $(patsubst core/%.c, $(OBJ_DIR)/%.o, $(SRCS_CORE))


.PHONY: all
all: $(TARGET)


include ../makefile.build
