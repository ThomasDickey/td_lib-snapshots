# $Id: Makefile,v 12.3 1995/01/31 00:24:03 tom Exp $
# make-file for TD_LIB common/test library

####### (Development) ##########################################################
THIS	= memory
include ../../support/td_lib.mk
include ../td_defns.mk

####### (Standard Lists) #######################################################
REF_FILES = \
	doalloc.ref \
	txtalloc.ref \
	vecedit.ref

SCRIPTS	= \
	run_test.sh \
	doalloc.sh \
	txtalloc.sh \
	vecedit.sh

C_SRC	=

SOURCES	= Makefile $(C_SRC) $(SCRIPTS) $(REF_FILES) $(DATA_FILES)

PROGS	=\
	doalloc \
	txtalloc \
	vecedit

####### (Standard Productions) #################################################
include ../td_rules.mk

# rules to make SunOs 'make' "work" with VPATH

doalloc.c \
txtalloc.c \
vecedit.c :
