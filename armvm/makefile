TOP_DIR = $(PWD)
GIT_DIR = $(TOP_DIR)/git

include makefile.setup

all: armvm.a armvm_core.a

include makefile.common

#libarmvm.a: libarmvm.a(armvm.a armvm_core.a armvm_trace.a)

armvm.a: libarmvm.a(armvm.a)
	$(MAKE) -C source

armvm_core.a: libarmvm.a(armvm_core.a)
	$(MAKE) -C source/core

armvm_trace.a: libarmvm.a(armvm_trace.a)
	$(MAKE) -C source/trace

livarmvm.so: libarmvm.a
	$(CC) -o libarmvm.so -fpic -shared libarmvm.a

clean:
	-rm -r */*.d */*.o

clean_all: clean clean_logs
	-rm -r *.a *.so */build*

clean_logs:
	-rm -r *.debug *.log *.trace
