# $Id: td_defns.mk,v 12.8 2000/01/01 00:59:37 tom Exp $
# common make-definitions for TD_LIB common/test library

####### (Development) ##########################################################
TOP	= ../..

CPP_OPTS = $(CPP_CONFIG) -DTEST
S	= ../../src/$(THIS)
VPATH	= $S
SOURCES	= $(SCRIPTS) $(REF_FILES)

.SUFFIXES:
.SUFFIXES: .c .lint
.c:	; $(LINK) -o $@ $(CFLAGS) $(CPP_OPTS) $< $(LIB_ARGS)
