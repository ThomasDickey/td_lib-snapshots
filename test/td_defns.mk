# $Id: td_defns.mk,v 12.6 1997/09/11 10:51:07 tom Exp $
# common make-definitions for TD_LIB common/test library

####### (Development) ##########################################################
TOP	= ../..

CPP_OPTS = $(DEFINES) $(INCLUDES) -DTEST
S	= ../../src/$(THIS)
VPATH	= $S

.SUFFIXES:
.SUFFIXES: .c .lint
.c:	; $(LINK) -o $@ $(CFLAGS) $(CPP_OPTS) $< $(LIB_ARGS)
