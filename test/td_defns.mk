# $Id: td_defns.mk,v 12.3 1994/07/02 19:19:39 tom Exp $
# common make-definitions for TD_LIB common/test library

####### (Development) ##########################################################
TOP	= ../../..
include ../../support/td_lib.mk

CPP_OPTS = $(DEFINES) $(INCLUDES) -DTEST
S	= ../../src/$(THIS)
VPATH	= $S

.SUFFIXES:
.SUFFIXES: .c .lint
.c:	; $(LINK) -o $@ $(CFLAGS) $(CPP_OPTS) $< $(LIBS) -lcurses -ltermcap
