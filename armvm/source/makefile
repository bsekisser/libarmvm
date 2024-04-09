include ../makefile.setup

CFLAGS += -O2 -march=native

SRC_DIR = .

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
DEPS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.d, $(SRCS))

.SECONDARY: $(OBJS)

#$(info $(TOP_DIR))

#$(info $(OBJ_DIR))

#$(info $(SRCS))

#$(info $(OBJS))

all: armvm.a

include ../makefile.common

armvm.a: armvm.a($(OBJS))

-include $(DEPS)

