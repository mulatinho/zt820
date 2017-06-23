AM_CFLAGS = -I../libs
bin_PROGRAMS = test
test_CFLAGS = -I../libs -L ../libs -I../src -DTEST=1 -W -std=c99 -g -ggdb
test_LDFLAGS = -lmlt_strlib
test_LDADD = ../libs/libmlt_strlib.a ../src/utils.o ../src/feelings.o
test_SOURCES = test.c
