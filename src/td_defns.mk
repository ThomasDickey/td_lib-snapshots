# $Id: td_defns.mk,v 12.1 1994/07/02 12:56:39 tom Exp $
# Common make-definitions for TD_LIB

.SUFFIXES:
.SUFFIXES: .c .o .a

####### (Environment) ##########################################################
TOP	= ../../..
include ../../support/td_lib.mk
LINTLIBS = -ltd -lcurses -ltermcap

Z	= $L/$(TD_LIB).a

SOURCES	= Makefile descrip.mms $(CSRC)

ALL	= $Z
