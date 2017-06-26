AM_CFLAGS = -I../libs
bin_PROGRAMS = utilstest
utilstest_CFLAGS = -I../libs -L ../libs -I../src -DTEST=1 -W -std=c99 -g -ggdb
utilstest_LDFLAGS = -lmlt_strlib
utilstest_LDADD = ../libs/libmlt_strlib.a ../src/utils.o ../src/feelings.o
utilstest_SOURCES = utilstest.c
