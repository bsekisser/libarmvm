include ../makefile.setup

$(info $(TOP_DIR))
$(info $(OBJ_DIR))

CFLAGS += -O2 -march=native

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))
DEPS = $(patsubst %.c, $(OBJ_DIR)/%.d, $(SRCS))

TARGET = $(OBJ_DIR)/armvm_trace.a

.SECONDARY: $(OBJS)

#$(info $(SRCS))

#$(info $(OBJS))

all: $(TARGET)

include ../makefile.common

$(TARGET): $(TARGET)($(OBJS))

-include $(DEPS)
