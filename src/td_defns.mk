# $Id: td_defns.mk,v 12.6 2000/01/17 22:41:01 tom Exp $
# Common make-definitions for TD_LIB

.SUFFIXES:
.SUFFIXES: .c .o .a

####### (Environment) ##########################################################
TOP	= ../..

LINTLIBS = -ltd -lcurses -ltermcap

Z	= $(LIB_DEPS)

SOURCES	= modules descrip.mms $(CSRC)

ALL	= $Z
