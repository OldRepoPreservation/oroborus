AM_CFLAGS = @CFLAGS@ @X_CFLAGS@ -DDATADIR=\"$(prefix)/share/oroborus\" $(DEBUG) -Wall
LIBS = @LIBS@ @X_LIBS@ -lSM -lICE

bin_PROGRAMS = oroborus
oroborus_SOURCES = client.c frame.c i18n.c misc.c parserc.c settings.c events.c \
		   hints.c keyboard.c oroborus.c pixmap.c workspaces.c hints.h \
		   i18n.h keyboard.h misc.h oroborus.h parserc.h pixmap.h \
		   xmalloc.c getopt.c getopt1.c getopt.h session.h session.c
