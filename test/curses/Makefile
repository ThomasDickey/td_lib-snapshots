# $Id: Makefile,v 12.3 1995/01/31 00:24:03 tom Exp $
# make-file for TD_LIB common/test library

####### (Development) ##########################################################
THIS	= curses
include ../../support/td_lib.mk
include ../td_defns.mk

####### (Standard Lists) #######################################################

REF_FILES = \
	padedit.ref \
	rawgets.ref \
	scr_size.ref

SCRIPTS	= \
	run_test.sh \
	padedit.sh \
	rawgets.sh \
	scr_size.sh

C_SRC	=

SOURCES	= Makefile $(C_SRC) $(SCRIPTS) $(REF_FILES) $(DATA_FILES)

PROGS	= \
	padedit \
	rawgets \
	scr_size

####### (Standard Productions) #################################################
include ../td_rules.mk

####### (Details of Productions) ###############################################

# rules to make SunOs 'make' "work" with VPATH

padedit.c \
rawgets.c \
scr_size.c :
