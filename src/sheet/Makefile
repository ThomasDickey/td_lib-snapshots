# $Id: Makefile,v 12.2 1995/01/28 21:10:25 tom Exp $
# MAKE-file for spreadsheet-data library routines

####### (Environment) ##########################################################
include ../td_defns.mk
include ../../support/td_lib.mk

####### (Standard Lists) #######################################################
CSRC	=\
	count_ss.c \
	field_of.c \
	ss_field.c

SOURCES	= Makefile descrip.mms $(CSRC)

OBJS	=\
	$Z(count_ss.o) \
	$Z(field_of.o) \
	$Z(ss_field.o)

####### (Standard Productions) #################################################
include ../td_rules.mk

####### (Details of Productions) ###############################################
$Z(count_ss.o)\
$Z(field_of.o)\
$Z(ss_field.o):	$I/td_sheet.h $I/dyn_str.h
