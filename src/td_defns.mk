# $Id: td_defns.mk,v 12.2 1995/01/28 21:09:29 tom Exp $
# Common make-definitions for TD_LIB

.SUFFIXES:
.SUFFIXES: .c .o .a

####### (Environment) ##########################################################
TOP	= ../../..

LINTLIBS = -ltd -lcurses -ltermcap

Z	= $L/$(TD_LIB).a

SOURCES	= Makefile descrip.mms $(CSRC)

ALL	= $Z
