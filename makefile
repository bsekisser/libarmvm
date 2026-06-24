TARGETs = libarm libarmcc libarmvm


.PHONY: all
all: $(addprefix all-, $(TARGETs))

.PHONY: all-%
all-%:
	$(MAKE) -C $* all


.PHONY: clean
clean: $(addprefix clean-, $(TARGETs))

.PHONY: clean-%
clean-%:
	$(MAKE) -C $* clean


all-libarmvm: all-libarm


all-libarmcc: all-libarmvm
