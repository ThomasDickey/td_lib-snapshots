# $Id: Makefile,v 12.5 1995/07/30 20:32:41 tom Exp $
# MAKE-file for comparison/sorting library routines

####### (Environment) ##########################################################
include ../td_defns.mk
include ../../support/td_lib.mk

####### (Standard Lists) #######################################################

CSRC	=\
	btree.c \
	cmpqsort.c \
	diffload.c \
	m2comp.c \
	scomp.c

SOURCES	= Makefile descrip.mms $(CSRC)

OBJS	=\
	$Z(btree.o) \
	$Z(cmpqsort.o) \
	$Z(diffload.o) \
	$Z(m2comp.o) \
	$Z(scomp.o)

####### (Standard Productions) #################################################
include ../td_rules.mk

####### (Details of Productions) ###############################################

$Z(btree.o):	$(PTYPES_H) $I/td_btree.h

$Z(cm_qsort.o):	$(QSORT_H)

$Z(m2comp.o)\
$Z(scomp.o):	$I/td_scomp.h

$Z(diffload.o):	$I/dyn_str.h
