TOP_DIR = $(PWD)
GIT_DIR = $(TOP_DIR)/git

ARFLAGS += -U

CFLAGS += -g -Wall -Wextra
#CFLAGS += -O2 -march=native
CFLAGS += -MMD -MP
CFLAGS += $(INCLUDE)

INCLUDE += -Iinclude
INCLUDE += -I$(GIT_DIR)

LDFLAGS += -lcapstone

SRC_DIR = source
SRCS = $(wildcard $(SRC_DIR)/*.c)

OBJ_DIR = build-$(shell $(CC) -dumpmachine)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

.SECONDARY: $(OBJS)

DEPS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.d, $(SRCS))

VPATH += source

all: libarm.a($(OBJS)) | $(OBJ_DIR)
#	$(CC) $(CFLAGS) -o $@ -c $^ -Wl,-lcapstone

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(OBJ_DIR):
	-mkdir -p $(OBJ_DIR)

clean:
	-rm $(OBJ_DIR)/*.d $(OBJ_DIR)/*.o

clean_all: clean clean_logs
	-rm -r $(OBJ_DIR) *.a

clean_logs:
	-rm *.debug *.log *.trace

-include $(DEPS)/*.d
