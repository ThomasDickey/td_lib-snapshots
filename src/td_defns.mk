# $Id: td_defns.mk,v 12.5 1997/09/11 10:41:43 tom Exp $
# Common make-definitions for TD_LIB

.SUFFIXES:
.SUFFIXES: .c .o .a

####### (Environment) ##########################################################
TOP	= ../..

LINTLIBS = -ltd -lcurses -ltermcap

Z	= $(TD_LIB)/libtd.a

SOURCES	= modules descrip.mms $(CSRC)

ALL	= $Z
