@SET_MAKE@

SUBDIRS = libs src tests
AUTOMAKE_OPTIONS = foreign subdir-objects
TESTS = tests/utilstest

test: $(TESTS)
	@for test in $(TESTS); do echo "\033[1m:. executing '$$test'...\033[m"; ./$$test; done

distclean-local:
	rm -rf *m4 *.cache *~ {src,libs,data}/*~ {src,libs}/*.{o,in,a} {src,libs}/{zt820,mlt_strlib}  $(DISTCLEANFILES)

EXTRA_DIST =				\
	LICENSE				\
	INSTALL				\
	README				\
	intltool-extract.in		\
	intltool-merge.in		\
	intltool-update.in		\
	$(desktop_in_files)

DISTCLEANFILES =			\
	aclocal.m4			\
	compile				\
	config.guess			\
	config.h.in			\
	config.log			\
	config.status			\
	config.sub			\
	configure			\
	depcomp				\
	install-sh			\
	ltmain.sh			\
	missing				\
	mkinstalldirs			\
	intltool-extract		\
	intltool-merge			\
	intltool-update			\
	Makefile.in			\
	src/Makefile.in			\
	src/Makefile			\
	Makefile
