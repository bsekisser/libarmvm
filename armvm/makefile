ARFLAGS += -U

CFLAGS += -g -Wall -Wextra
CFLAGS += -O2 -march=native
#CFLAGS += -fpic -shared
CFLAGS += -MMD -MP
CFLAGS += $(INCLUDE)

LDFLAGS += -Lgit/libarm -larm

INCLUDE += -Iinclude
INCLUDE += -Igit

VPATH += source

all: libarmvm.a

#libarmvm.so: libarmvm.so(libarmvm.a)
libarmvm.so: libarmvm.a
	$(CC) -o libarmvm.so -fpic -shared libarmvm.a

ARMVM_CORE_OBJS = armvm_core.o \
	armvm_core_arm.o \
	armvm_core_exception.o \
	armvm_core_thumb.o \
	armvm_core_trace.o

ARMVM_TRACE_OBJS = armvm_trace.o \
	armvm_trace_arm.o

ARMVM_OBJS = \
	armvm_coprocessor.o \
	armvm_exception.o \
	armvm_mem.o \
	armvm_mmu.o \
	armvm_tlb.o \
	armvm.o

libarmvm.a: libarmvm.a( \
	$(ARMVM_OBJS) \
	$(ARMVM_CORE_OBJS) \
	$(ARMVM_TRACE_OBJS))

clean:
	-rm *.d *.o

clean_all: clean clean_logs
	-rm *.a *.so

clean_logs:
	-rm *.debug *.log *.trace

-include *.d
