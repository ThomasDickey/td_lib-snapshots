# $Id: Makefile,v 12.4 1994/08/02 16:48:58 tom Exp $
# make-file for TD_LIB common/test library

####### (Development) ##########################################################
THIS=cm_funcs
include ../td_defns.mk

####### (Standard Lists) #######################################################
REF_FILES = \
	cutoff.ref \
	newzone.ref \
	packdate.ref \
	rcsargpr.ref \
	rcsload.ref \
	rcsname.ref \
	samebr.ref \
	sccsname.ref \
	vcs_file.ref \
	vercmp.ref

SCRIPTS	= \
	run_test.sh \
	cutoff.sh \
	newzone.sh \
	packdate.sh \
	rcsargpr.sh \
	rcsload.sh \
	rcsname.sh \
	samebr.sh \
	sccsname.sh \
	vcs_file.sh \
	vercmp.sh

C_SRC	=

SOURCES	= Makefile $(SCRIPTS) $(REF_FILES) $(DATA_FILES)

PROGS	=\
	cmv_dir \
	cmv_last \
	cutoff \
	newzone \
	packdate \
	rcsargpr \
	rcsload \
	rcsname \
	samebr \
	sccsname \
	vcs_file \
	vercmp

####### (Standard Productions) #################################################
include ../td_rules.mk

# rules to make SunOs 'make' "work" with VPATH

cmv_dir.c \
cmv_last.c \
cutoff.c \
newzone.c \
packdate.c \
rcsargpr.c \
rcsload.c \
rcsname.c \
samebr.c \
sccsname.c \
vcs_file.c \
vercmp.c :
