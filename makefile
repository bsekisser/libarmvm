TARGETS = libarm libarmcc libarmvm

.PHONY: all
all: libarm.a libarmcc.a libarmvm.a

.PHONY: libarm.a
libarm.a: libarm/libarm.a
	@[ -L $@ ] || ln -sr $^

.PHONY: libarm/libarm.a
libarm/libarm.a:
	$(MAKE) -C libarm

.PHONY: libarmcc.a
libarmcc.a: libarmcc/libarmcc.a
	@[ -L $@ ] || ln -sr $^

.PHONY: libarmcc/libarmcc.a
libarmcc/libarmcc.a: libarmvm.a libarm.a
	$(MAKE) -C libarmcc

.PHONY: libarmvm.a
libarmvm.a: libarmvm/libarmvm.a
	@[ -L $@ ] || ln -sr $^

.PHONY: libarmvm/libarmvm.a
libarmvm/libarmvm.a: libarm.a
	$(MAKE) -C libarmvm

.PHONY: clean
clean: $(addsuffix .clean,$(TARGETS))

%.clean:
	$(MAKE) -C $* clean

.PHONY: clean_all
clean_all: $(addsuffix .clean_all,$(TARGETS))

%.clean_all:
	$(MAKE) -C $* clean_all
	-rm $*.a

.PHONY: clean_all
clean_logs: $(addsuffix .clean_logs,$(TARGETS))

%.clean_logs:
	$(MAKE) -C $* clean_logs
