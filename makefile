all: libarm.a libarmvm.a

libarm.a:
	$(MAKE) -C libarm && ln -sr ./libarm/libarm.a .

libarmvm.a:
	$(MAKE) -C libarmvm && ln -sr ./armvm/libarmvm.a .

clean:
	$(MAKE) -C libarm clean
	$(MAKE) -C libarmvm clean

clean_all:
	$(MAKE) -C libarm clean_all
	$(MAKE) -C libarmvm clean_all
