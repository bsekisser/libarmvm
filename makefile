all: libarm.a libarmcc.a libarmvm.a

libarm.a:
	$(MAKE) -C libarm && ln -sr ./libarm/libarm.a .

libarmcc.a:
	$(MAKE) -C libarmcc && ln -sr ./libarmcc/libarmcc.a .

libarmvm.a:
	$(MAKE) -C libarmvm && ln -sr ./libarmvm/libarmvm.a .

clean:
	$(MAKE) -C libarm clean
	$(MAKE) -C libarmcc clean
	$(MAKE) -C libarmvm clean

clean_all:
	$(MAKE) -C libarm clean_all
	$(MAKE) -C libarmcc clean_all
	$(MAKE) -C libarmvm clean_all
