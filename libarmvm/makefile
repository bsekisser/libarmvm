#TOP_DIR = $(PWD)
#GIT_GIT = $(TOP_DIR)/..

include makefile.setup

TARGET = libarmvm.a
OBJ_TARGET = $(OBJ_DIR)/$(TARGET)

all: $(OBJ_TARGET) $(TARGET)

$(TARGET): $(OBJ_TARGET)
	ln -sr $(OBJ_TARGET)

ARMVM_SRCS = $(wildcard armvm/*.c)
ARMVM_OBJS = $(patsubst armvm/%.c, $(OBJ_DIR)/%.o, $(ARMVM_SRCS))

#armvm.a: $(OBJ_DIR)/armvm.a

$(ARMVM_OBJS): $(ARMVM_SRCS)

#$(OBJ_DIR)/armvm.a: $(ARMVM_OBJS)
armvm.a: $(ARMVM_OBJS)
	$(MAKE) -C armvm

ARMVM_CORE_SRCS = $(wildcard core/*.c)
ARMVM_CORE_OBJS = $(patsubst core/%.c, $(OBJ_DIR)/%.o, $(ARMVM_CORE_SRCS))

$(ARMVM_CORE_OBJS): $(ARMVM_CORE_SRCS)

#$(OBJ_DIR)/armvm_core.a: $(ARMVM_CORE_OBJS)
armvm_core.a: $(ARMVM_CORE_OBJS)
	$(MAKE) -C core

ARMVM_TRACE_SRCS = $(wildcard trace/*.c)
ARMVM_TRACE_OBJS = $(patsubst trace/%.c, $(OBJ_DIR)/%.o, $(ARMVM_TRACE_SRCS))

$(ARMVM_TRACE_OBJS): $(ARMVM_TRACE_SRCS)

#$(OBJ_DIR)/armvm_trace.a: $(ARMVM_TRACE_OBJS)
armvm_trace.a: $(ARMVM_TRACE_OBJS)
	$(MAKE) -C trace

#LIBS = armvm.a armvm_core.a armvm_trace.a
LIBS = armvm.a armvm_core.a

LIBARMVM_OBJS = $(ARMVM_OBJS) \
	$(ARMVM_CORE_OBJS)
#	$(ARMVM_TRACE_OBJS)

$(OBJ_TARGET): $(LIBS) $(OBJ_TARGET)($(LIBARMVM_OBJS))


	
clean:
	-rm -r */*.d */*.o

clean_all: clean clean_logs
	-rm -r */*.a

clean_logs:
	-rm *.debug *.log *.trace
