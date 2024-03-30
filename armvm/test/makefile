CFLAGS += -g -Wall
CFLAGS += -O2
CFLAGS += $(INCLUDE)

INCLUDE += -Iinclude
INCLUDE += -I../include
INCLUDE += -I../../arm/include
INCLUDE += -I../../libbse/include

VPATH += source
VPATH += ../../arm/source
VPATH += ../../libbse/include

all: test.o libarmvm.a

clean:
	-rm *.d *.o

clean_all: clean clean_logs
	-rm *.a

clean_logs:
	-rm *.debug *.log *.trace
