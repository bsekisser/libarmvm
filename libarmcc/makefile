TOP_DIR = $(PWD)
GIT_DIR = $(TOP_DIR)/git

ARFLAGS += -U

CFLAGS += -g -Wall -Wextra
CFLAGS += -O2 -march=native
CFLAGS += -MMD -MP
CFLAGS += $(INCLUDE)

CPPFLAGS += $(CFLAGS)

INCLUDE += -Isource
INCLUDE += -Iinclude
INCLUDE += -I$(GIT_DIR)

LDLIBS += -L$(GIT_DIR)/libbse -lbse
LDLIBS += -L$(GIT_DIR)/libarm -larm
#LDLIBS += -lcapstone

SRC_DIR = source
SRCS = $(wildcard $(SRC_DIR)/*.c)
SRCS_CPP = $(wildcard $(SRC_DIR)/*.cpp)

OBJ_DIR = build-$(shell $(CC) -dumpmachine)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
OBJS_CPP = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS_CPP))

.SECONDARY: $(OBJS) $(OBJS_CPP)

DEP_DIR = $(OBJ_DIR)
DEPS = $(patsubst $(SRC_DIR)/%.c, $(DEP_DIR)/%.d, $(SRCS))
DEPS_CPP = $(patsubst $(SRC_DIR)/%.c, $(DEP_DIR)/%.d, $(SRCS_CPP))

VPATH += source
VPATH += $(TOP_DIR)

#TARGET = libarmcc.a

#OBJ_TARGET = $(OBJ_DIR)/$(TARGET)

#all: $(TARGET)
all: libarmcc.a($(OBJS) $(OBJS_CPP)) | $(OBJ_DIR)

#$(TARGET): $(OBJ_TARGET) | $(OBJ_DIR)

#$(OBJ_TARGET): libarmcc.a($(OBJS) $(OBJS_CPP)) | $(OBJ_DIR)

#$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
#	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(OBJ_DIR):
	-mkdir -p $(OBJ_DIR)

clean:
	-rm $(OBJ_DIR)/*.d $(OBJ_DIR)/*.o

clean_all: clean clean_logs
	-rm -r $(OBJ_DIR) *.a

clean_logs:
	-rm *.debug *.log *.trace

-include $(DEPS)/*.d
-include $(DEPS_CPP)/*.d
