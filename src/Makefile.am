#AM_YFLAGS = -d  -v

SUBDIRS = .

bin_PROGRAMS = alobe testfifo testlist

testfifo_SOURCES = \
	fifo.c \
	testfifo.c

testfifo_SOURCES += \
	fifo.h \
	units.h

testlist_SOURCES = \
	list.c \
	testlist.c

testlist_SOURCES += \
	list.h \
	units.h
	
alobe_SOURCES = 	\
	alobe.c \
	config.c \
	connexcomponent.c \
	console.c \
	defi.c \
	degree.c \
	degenerator.c \
	distance.c \
	fifo.c \
	filter.c \
	generator.c \
	list.c \
	nodeset.c \
	progressindicator.c \
	store.c

alobe_SOURCES += \
	alobe.h \
	config.h \
	connexcomponent.h \
	console.h \
	degree.h \
	defi.h \
	degenerator.h \
	distance.h \
	fifo.h
	filter.h \
	generator.h \
	list.h \
	nodeset.h \
	progressindicator.h \
	store.h \
	units.h

EXTRA_DIST = mini.0.20  mini.0.60

INCLUDES= 	-I../lib/ -I./ -I/usr/include

alobe_LDFLAGS = 	@LDFLAGS@ -lz

