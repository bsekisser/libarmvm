TOP_DIR = $(PWD)
GIT_DIR = $(TOP_DIR)/git

ARFLAGS += -U

CFLAGS += -g -Wall -Wextra
CFLAGS += -O2
CFLAGS += -MMD -MP
CFLAGS += $(INCLUDE)

CPPFLAGS += $(CFLAGS)

LDLIBS += -L$(GIT_DIR)/libarmvm -larmvm
LDLIBS += -L$(GIT_DIR)/libarmcc -larmcc
LDLIBS += -L$(GIT_DIR)/libarm -larm
LDLIBS += -L$(GIT_DIR)/libbse -lbse
LDLIBS += -lcapstone
#LDLIBS += -lstdc++

INCLUDE += -Isource
INCLUDE += -I$(GIT_DIR)
#INCLUDE += -I..

VPATH += $(SRC_DIR)
VPATH += $(GIT_DIR)/libarmvm
VPATH += $(GIT_DIR)/libarm
VPATH += $(GIT_DIR)/libbse

SRC_DIR = source
SRCS = $(wildcard $(SRC_DIR)/*.c)
SRCS_CPP = $(wildcard $(SRC_DIR)/*.cpp)

OBJ_DIR = build-$(shell $(CC) -dumpmachine)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
OBJS_CPP = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS_CPP))

TARGET = test_armvm_perf
OBJ_TARGET = $(OBJ_DIR)/$(TARGET)

all: $(TARGET)

LIBS = libbse.a libarm.a libarmvm.a

$(TARGET): $(OBJ_TARGET) $(LIBS)
	-ln -sr $(OBJ_DIR)/$(TARGET)

$(OBJ_TARGET): $(OBJS) $(OBJS_CPP) | $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(OBJ_DIR):
	-mkdir -p $(OBJ_DIR)

clean:
	-rm $(OBJ_DIR)/*.d $(OBJ_DIR)/*.o

clean_all: clean clean_logs
	-rm */*.a
#	-rm */*.d */*.o

clean_logs:
	-rm */*.debug */*.log */*.trace

-include $(OBJ_DIR)/*.d
