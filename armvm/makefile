ARFLAGS += -U

CFLAGS += -g -Wall
CFLAGS += -O2
CFLAGS += -MD -MP
CFLAGS += $(INCLUDE)

INCLUDE += -Iinclude
INCLUDE += -I../arm/include
INCLUDE += -I../libbse/include

VPATH += source
VPATH += include
VPATH += ../arm/source
VPATH += ../libbse/include

all: libarmvm.a

#libarmvm.so: libarmvm.so(libarmvm.a)

libarmvm.a: libarmvm.a( \
	arm_core_arm.o \
	arm_core_thumb.o \
	arm_core_trace.o \
	arm_exception.o \
	arm_mem.o \
	arm_trace_arm.o \
	arm_trace.o \
	arm_vm.o )
#	libarm_disasm.o )

test: test.o libarm_disasm.o

clean:
	-rm *.d *.o

clean_all: clean clean_logs
	-rm *.a

clean_logs:
	-rm *.debug *.log *.trace

-include *.d
