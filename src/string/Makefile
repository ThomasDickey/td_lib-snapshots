# $Id: Makefile,v 12.1 1994/07/02 15:43:49 tom Exp $
# MAKE-file for string-handling library routines

####### (Environment) ##########################################################
include ../td_defns.mk

####### (Standard Lists) #######################################################
CSRC	=\
	dftenv.c \
	dotcmp.c \
	dyn_app.c \
	dyn_appc.c \
	dyn_cata.c \
	dyn_copy.c \
	dyn_str.c \
	lsbycols.c \
	ltostr.c \
	strbcmp.c \
	strclean.c \
	strcount.c \
	strerror.c \
	strtrim.c \
	strucmp.c \
	strucpy.c \
	strwcmp.c

SOURCES	= Makefile descrip.mms $(CSRC)

OBJS	=\
	$Z(dftenv.o) \
	$Z(dotcmp.o) \
	$Z(dyn_app.o) \
	$Z(dyn_appc.o) \
	$Z(dyn_cata.o) \
	$Z(dyn_copy.o) \
	$Z(dyn_str.o) \
	$Z(lsbycols.o) \
	$Z(ltostr.o) \
	$Z(strbcmp.o) \
	$Z(strclean.o) \
	$Z(strcount.o) \
	$Z(strerror.o) \
	$Z(strtrim.o) \
	$Z(strucmp.o) \
	$Z(strucpy.o) \
	$Z(strwcmp.o)

####### (Standard Productions) #################################################
include ../td_rules.mk

$Z(dotcmp.o):	 $I/td_qsort.h
$Z(dyn_app.o) \
$Z(dyn_appc.o) \
$Z(dyn_cata.o) \
$Z(dyn_copy.o) \
$Z(dyn_str.o):	 $I/dyn_str.h

