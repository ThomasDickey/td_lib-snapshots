# $Id: Makefile,v 12.2 1995/01/29 00:47:37 tom Exp $
# make-file for TD_LIB common/test library

####### (Development) ##########################################################
THIS	= unix
include ../td_defns.mk
include ../../support/td_lib.mk

####### (Standard Lists) #######################################################
REF_FILES = \
	gid2s.ref \
	s2uid.ref \
	uid2s.ref

SCRIPTS	= \
	gid2s.sh \
	s2uid.sh \
	uid2s.sh

C_SRC	=

SOURCES	= Makefile $(C_SRC) $(SCRIPTS) $(REF_FILES) $(DATA_FILES)

PROGS	= \
	gid2s \
	s2uid \
	uid2s

####### (Standard Productions) #################################################
include ../td_rules.mk

####### (Details of Productions) ###############################################

# rules to make SunOs 'make' "work" with VPATH

gid2s.c \
s2uid.c \
uid2s.c :
