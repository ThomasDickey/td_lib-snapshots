# $Id: td_defns.mk,v 12.4 1997/09/09 22:11:03 tom Exp $
# Common make-definitions for TD_LIB

.SUFFIXES:
.SUFFIXES: .c .o .a

####### (Environment) ##########################################################
TOP	= ../../..

LINTLIBS = -ltd -lcurses -ltermcap

Z	= $L/libtd.a

SOURCES	= modules descrip.mms $(CSRC)

ALL	= $Z
