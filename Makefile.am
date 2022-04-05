lib_LTLIBRARIES = libwow.la

libwow_la_SOURCES = src/adt.c src/blp.c src/bls.c src/common.c src/dbc.c \
                    src/m2.c src/mpq.c src/toc.c src/trs.c src/wdl.c \
                    src/wdt.c src/wmo.c src/wmo_group.c

libwow_la_LDFLAGS = 

libwow_la_CFLAGS = -std=c99

pkgincludedir = $(includedir)/libwow

pkginclude_HEADERS = src/adt.h src/blp.h src/bls.h src/common.h src/dbc.h \
                     src/m2.h src/mpq.h src/toc.h src/trs.h src/wdl.h \
                     src/wdt.h src/wmo.h src/wmo_group.h

AUTOMAKE_OPTIONS = subdir-objects
