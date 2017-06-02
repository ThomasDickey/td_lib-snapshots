# $Id: Makefile,v 12.4 2010/05/24 23:36:30 tom Exp $
# make-file for TD_LIB common/test library

####### (Development) ##########################################################
THIS=compare
include ../../support/td_lib.mk
include ../td_defns.mk

####### (Standard Lists) #######################################################
REF_FILES = \
	diffload.ref \
	m2comp.ref \
	scomp.ref

SCRIPTS	= \
	run_test.sh \
	diffload.sh \
	m2comp.sh \
	scomp.sh

C_SRC	=

SOURCES	= Makefile $(C_SRC) $(SCRIPTS) $(REF_FILES) $(DATA_FILES)

PROGS	= \
	diffload \
	m2comp \
	scomp

####### (Standard Productions) #################################################
include ../td_rules.mk

####### (Details of Productions) ###############################################

# rules to make SunOs 'make' "work" with VPATH

m2comp:	test_cmp.c $S/m2comp.c ; $(LINK) -o $@ $(CFLAGS) $(CPP_OPTS) $S/$@.c $(LIB_ARGS)
scomp:	test_cmp.c $S/scomp.c  ; $(LINK) -o $@ $(CFLAGS) $(CPP_OPTS) $S/$@.c $(LIB_ARGS)

diffload.c \
m2comp.c \
scomp.c  :
