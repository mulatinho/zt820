bin_PROGRAMS = utilstest
utilstest_CFLAGS = -I../src -L../libs -DTEST=1 -W -std=c99 -g -ggdb
utilstest_LDFLAGS = -lmlt_strlib
utilstest_LDADD = ../src/zt820-utils.o ../src/zt820-feelings.o -lmlt_strlib
utilstest_SOURCES = utilstest.c
