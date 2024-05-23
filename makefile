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
libarmcc/libarmcc.a:
	$(MAKE) -C libarmcc

.PHONY: libarmvm.a
libarmvm.a: libarmvm/libarmvm.a
	@[ -L $@ ] || ln -sr $^

.PHONY: libarmvm/libarmvm.a
libarmvm/libarmvm.a:
	$(MAKE) -C libarmvm

.PHONY: clean
clean:
	$(MAKE) -C libarm clean
	$(MAKE) -C libarmcc clean
	$(MAKE) -C libarmvm clean

.PHONY: clean_all
clean_all: clean clean_logs
	$(MAKE) -C libarm clean_all
	$(MAKE) -C libarmcc clean_all
	$(MAKE) -C libarmvm clean_all

.PHONY: clean_logs
clean_logs:
	$(MAKE) -C libarm clean_logs
	$(MAKE) -C libarmcc clean_logs
	$(MAKE) -C libarmvm clean_logs
