#TOP_DIR = $(PWD)
#GIT_GIT = $(TOP_DIR)/..

include makefile.setup

TARGET = libarmvm.a
OBJ_TARGET = $(OBJ_DIR)/$(TARGET)


all: $(OBJ_TARGET) $(TARGET)

OBJS = $(OBJ_DIR)/armvm.a \
	$(OBJ_DIR)/armvm_core.a \
	$(OBJ_DIR)/armvm_trace.a

$(TARGET): $(OBJ_TARGET)
	ln -sr $(OBJ_TARGET)

$(OBJ_TARGET): $(OBJ_TARGET)($(OBJS))

$(OBJ_DIR)/armvm.a:
	$(MAKE) -C armvm

$(OBJ_DIR)/armvm_core.a:
	$(MAKE) -C core

$(OBJ_DIR)/armvm_trace.a:
	$(MAKE) -C trace
	
clean:
	-rm -r */*.d */*.o

clean_all: clean clean_logs
	-rm -r */*.a

clean_logs:
	-rm *.debug *.log *.trace
