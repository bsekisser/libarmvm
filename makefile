all: libarm.a libarmvm.a

libarm.a:
	$(MAKE) -C libarm
	-ln -sr ./libarm/libarm.a .

libarmvm.a:
	$(MAKE) -C armvm
	-ln -sr ./armvm/libarmvm.a .

clean:
	$(MAKE) -C libarm clean
	$(MAKE) -C armvm clean

clean_all:
	$(MAKE) -C libarm clean_all
	$(MAKE) -C armvm clean_all
