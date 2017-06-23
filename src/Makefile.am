AM_CFLAGS = -I../libs
bin_PROGRAMS = zt820

zt820_LDADD = -lpthread ../libs/libmlt_strlib.a
zt820_CFLAGS = -W -std=c99 -g -ggdb

zt820_SOURCES =	feelings.c 	\
				utils.c 	\
				main.c

noinst_HEADERS = feelings.h	\
				 utils.h	\
				 main.h
