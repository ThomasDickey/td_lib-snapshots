# $Id: td_defns.mk,v 12.4 1994/07/16 20:19:19 tom Exp $
# common make-definitions for TD_LIB common/test library

####### (Development) ##########################################################
TOP	= ../../..
include ../../support/td_lib.mk

CPP_OPTS = $(DEFINES) $(INCLUDES) -DTEST
S	= ../../src/$(THIS)
VPATH	= $S

.SUFFIXES:
.SUFFIXES: .c .lint
.c:	; $(LINK) -o $@ $(CFLAGS) $(CPP_OPTS) $< $(LIB_ARGS) -lcurses -ltermcap
