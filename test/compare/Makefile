# $Id: Makefile,v 12.3 1995/01/31 00:24:03 tom Exp $
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

m2comp:	test_cmp.c $S/m2comp.c ; $(LINK) -o $@ $(CFLAGS) $(CPP_OPTS) $S/$@.c $(LIBS)
scomp:	test_cmp.c $S/scomp.c  ; $(LINK) -o $@ $(CFLAGS) $(CPP_OPTS) $S/$@.c $(LIBS)

diffload.c \
m2comp.c \
scomp.c  :
