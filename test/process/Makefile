# $Id: Makefile,v 12.3 1995/01/31 00:24:03 tom Exp $
# make-file for TD_LIB common/test library

####### (Development) ##########################################################
THIS	= process
include ../../support/td_lib.mk
include ../td_defns.mk

####### (Standard Lists) #######################################################
REF_FILES = \
	execute.ref \
	failed.ref \
	gethome.ref \
	makeargv.ref \
	usercopy.ref \
	walkback.ref

SCRIPTS	= \
	run_test.sh \
	execute.sh \
	failed.sh \
	gethome.sh \
	makeargv.sh \
	usercopy.sh \
	walkback.sh

C_SRC	=

SOURCES	= Makefile $(C_SRC) $(SCRIPTS) $(REF_FILES) $(DATA_FILES)

PROGS	= \
	execute \
	failed \
	gethome \
	makeargv \
	usercopy \
	walkback

####### (Standard Productions) #################################################
include ../td_rules.mk

####### (Details of Productions) ###############################################

# rules to make SunOs 'make' "work" with VPATH

execute.c \
failed.c \
gethome.c \
makeargv.c \
usercopy.c \
walkback.c :
