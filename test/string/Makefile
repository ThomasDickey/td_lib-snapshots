# $Id: Makefile,v 12.3 1995/01/31 00:24:03 tom Exp $
# make-file for TD_LIB common/test library

####### (Development) ##########################################################
THIS	= string
include ../../support/td_lib.mk
include ../td_defns.mk

####### (Standard Lists) #######################################################
REF_FILES = \
	dotcmp.ref \
	lsbycols.ref \
	ltostr.ref \
	strbcmp.ref \
	strucmp.ref \
	strwcmp.ref

SCRIPTS	= \
	run_test.sh \
	dotcmp.sh \
	lsbycols.sh \
	ltostr.sh \
	strbcmp.sh \
	strucmp.sh \
	strwcmp.sh

C_SRC	=

SOURCES	= Makefile $(C_SRC) $(SCRIPTS) $(REF_FILES) $(DATA_FILES)

PROGS	= \
	dotcmp \
	lsbycols \
	ltostr \
	strbcmp \
	strucmp \
	strwcmp

####### (Standard Productions) #################################################
include ../td_rules.mk

####### (Details of Productions) ###############################################

# rules to make SunOs 'make' "work" with VPATH

dotcmp.c \
lsbycols.c \
ltostr.c \
strbcmp.c \
strucmp.c \
strwcmp.c :
