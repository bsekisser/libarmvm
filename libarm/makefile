ARFLAGS += -U

CFLAGS += -g -Wall
CFLAGS += -O2
CFLAGS += $(INCLUDE)

INCLUDE += -Iinclude
INCLUDE += -Igit

LDFLAGS += -lcapstone

VPATH += source

all: libarm.a libarm_disasm.a

libarm.a: libarm.a( \
	arm_alubox.o \
	arm_shiftbox.o \
	arm_strings.o )

libarm_disasm.a: libarm_disasm.a(arm_disasm.o)
	$(CC) $(CFLAGS) -o $@ -c $^ -Wl,-lcapstone
#	$(CC) $(CFLAGS) -o $@ -Wl,-lcapstone -c $^
#	$(CC) $(CFLAGS) -o $@ -Wl,-lcapstone -c $^ -Wl,-lcapstone

clean:
	-rm *.d *.o

clean_all: clean clean_logs
	-rm *.a

clean_logs:
	-rm *.debug *.log *.trace
