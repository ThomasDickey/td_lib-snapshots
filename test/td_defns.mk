# $Id: td_defns.mk,v 12.5 1995/01/29 00:47:16 tom Exp $
# common make-definitions for TD_LIB common/test library

####### (Development) ##########################################################
TOP	= ../../..

CPP_OPTS = $(DEFINES) $(INCLUDES) -DTEST
S	= ../../src/$(THIS)
VPATH	= $S

.SUFFIXES:
.SUFFIXES: .c .lint
.c:	; $(LINK) -o $@ $(CFLAGS) $(CPP_OPTS) $< $(LIB_ARGS)
