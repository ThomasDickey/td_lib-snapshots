# $Id: td_defns.mk,v 12.3 1997/09/08 00:29:39 tom Exp $
# Common make-definitions for TD_LIB

.SUFFIXES:
.SUFFIXES: .c .o .a

####### (Environment) ##########################################################
TOP	= ../../..

LINTLIBS = -ltd -lcurses -ltermcap

Z	= $L/libtd.a

SOURCES	= Makefile descrip.mms $(CSRC)

ALL	= $Z
