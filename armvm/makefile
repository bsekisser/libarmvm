ARFLAGS += -U

CFLAGS += -g -Wall -Wextra
CFLAGS += -O2
#CFLAGS += -fpic -shared
CFLAGS += -MD -MP
CFLAGS += $(INCLUDE)

INCLUDE += -Iinclude
INCLUDE += -I..

VPATH += source

all: libarmvm.a

#libarmvm.so: libarmvm.so(libarmvm.a)
libarmvm.so: libarmvm.a
	$(CC) -o libarmvm.so -fpic -shared libarmvm.a

ARMVM_CORE_OBJS = armvm_core.o \
	armvm_core_arm.o \
	armvm_core_thumb.o \
	armvm_core_trace.o

ARMVM_TRACE_OBJS = armvm_trace.o \
	armvm_trace_arm.o

ARMVM_OBJS = armvm.o \
	armvm_coprocessor.o \
	armvm_exception.o \
	armvm_mem.o \
	armvm_tlb.o

libarmvm.a: libarmvm.a( \
	$(ARMVM_OBJS) \
	$(ARMVM_CORE_OBJS) \
	$(ARMVM_TRACE_OBJS))

test: test.o libarm_disasm.o

clean:
	-rm *.d *.o

clean_all: clean clean_logs
	-rm *.a

clean_logs:
	-rm *.debug *.log *.trace

-include *.d
