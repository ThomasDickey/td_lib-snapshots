# $Id: td_defns.mk,v 12.7 1997/09/11 14:48:10 tom Exp $
# common make-definitions for TD_LIB common/test library

####### (Development) ##########################################################
TOP	= ../..

CPP_OPTS = $(DEFINES) $(INCLUDES) -DTEST
S	= ../../src/$(THIS)
VPATH	= $S
SOURCES	= $(SCRIPTS) $(REF_FILES)

.SUFFIXES:
.SUFFIXES: .c .lint
.c:	; $(LINK) -o $@ $(CFLAGS) $(CPP_OPTS) $< $(LIB_ARGS)
