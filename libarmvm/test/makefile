ARFLAGS += -U

CFLAGS += -g -Wall -Wextra
CFLAGS += -O2
CFLAGS += -MMD -MP
CFLAGS += $(INCLUDE)

CPPFLAGS += $(CFLAGS)

LDLIBS += -Lgit/libarmvm -larmvm
LDLIBS += -Lgit/libarm -larm
LDLIBS += -Lgit/libbse -lbse
LDLIBS += -lcapstone

INCLUDE += -Iinclude
INCLUDE += -Igit
INCLUDE += -I..

VPATH += source
VPATH += ../

all: test

test: test.o test_arm.a

TEST_ARM_OBJS = \
	test_arm_adc.o \
	test_arm_add.o \
	test_arm_and.o \
	test_arm_b_bl.o \
	test_arm_bic.o \
	test_arm_mov.o \
	test_arm_sub.o \
	test_arm_rsb.o \
	test_arm_shift.o \
	test_arm.o

test_arm.a: test_arm.a($(TEST_ARM_OBJS))

clean:
	-rm *.d *.o

clean_all: clean clean_logs
	-rm *.a

clean_logs:
	-rm *.debug *.log *.trace

-include *.d
