# $Id: td_defns.mk,v 12.10 2020/12/06 21:18:54 tom Exp $
# common make-definitions for TD_LIB common/test library

####### (Development) ##########################################################
TOP	= ../..

CPP_OPTS = $(CPP_CONFIG) -DTEST
S	= $(TOP_SRCDIR)/src/$(THIS)
VPATH	= $S
SOURCES	= $(SCRIPTS) $(REF_FILES)

.SUFFIXES:
.SUFFIXES: .c .lint
.c:	; $(LINK) -o $@ $(CFLAGS) $(CPP_OPTS) $< $(CURSES_LIBS)
