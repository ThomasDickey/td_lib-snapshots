# $Id: Makefile,v 12.3 1995/01/28 21:10:25 tom Exp $
# MAKE-file for comparison/sorting library routines

####### (Environment) ##########################################################
include ../td_defns.mk
include ../../support/td_lib.mk

####### (Standard Lists) #######################################################

CSRC	=\
	cmpqsort.c \
	diffload.c \
	m2comp.c \
	scomp.c

SOURCES	= Makefile descrip.mms $(CSRC)

OBJS	=\
	$Z(cmpqsort.o) \
	$Z(diffload.o) \
	$Z(m2comp.o) \
	$Z(scomp.o)

####### (Standard Productions) #################################################
include ../td_rules.mk

####### (Details of Productions) ###############################################

$Z(cm_qsort.o):	$(QSORT_H)

$Z(m2comp.o)\
$Z(scomp.o):	$I/td_scomp.h

$Z(diffload.o):	$I/dyn_str.h
