# $Id: Makefile,v 12.2 1995/01/28 21:10:25 tom Exp $
# MAKE-file for UNIX-oriented library routines

####### (Environment) ##########################################################
include ../td_defns.mk
include ../../support/td_lib.mk

####### (Standard Lists) #######################################################
CSRC	=\
	gid2s.c \
	s2gid.c \
	s2uid.c \
	setmtime.c \
	uid2s.c

SOURCES	= Makefile descrip.mms $(CSRC)

OBJS	=\
	$Z(gid2s.o) \
	$Z(s2gid.o) \
	$Z(s2uid.o) \
	$Z(setmtime.o) \
	$Z(uid2s.o)

####### (Standard Productions) #################################################
include ../td_rules.mk
