# $Id: Makefile,v 12.1 1994/07/02 18:00:49 tom Exp $
# make-file for TD_LIB common/test library

####### (Development) ##########################################################
THIS	= memory
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
