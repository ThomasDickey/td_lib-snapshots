# $Id: Makefile,v 12.2 1994/07/02 14:52:28 tom Exp $
# MAKE-file for memory-allocation library routines

####### (Environment) ##########################################################
include ../td_defns.mk

####### (Standard Lists) #######################################################
CSRC	=\
	doalloc.c \
	memmove.c \
	stralloc.c \
	txtalloc.c \
	vecalloc.c \
	vecedit.c \
	veclen.c

SOURCES	= Makefile descrip.mms $(CSRC)

OBJS	=\
	$Z(doalloc.o) \
	$Z(memmove.o) \
	$Z(stralloc.o) \
	$Z(txtalloc.o) \
	$Z(vecalloc.o) \
	$Z(vecedit.o) \
	$Z(veclen.o)

####### (Standard Productions) #################################################
include ../td_rules.mk
