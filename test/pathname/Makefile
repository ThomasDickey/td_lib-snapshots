# $Id: Makefile,v 12.5 1995/01/31 00:24:03 tom Exp $
# make-file for TD_LIB common/test library

####### (Development) ##########################################################
THIS	= pathname
include ../../support/td_lib.mk
include ../td_defns.mk

####### (Standard Lists) #######################################################
REF_FILES = \
	abspath.ref \
	dlettree.ref \
	editfile.ref \
	edittree.ref \
	file2arg.ref \
	file2mem.ref \
	filecopy.ref \
	fp2argv.ref \
	name2s.ref \
	pathcmp.ref \
	pathhead.ref \
	pathleaf.ref \
	relpath.ref \
	sameleaf.ref \
	trimpath.ref \
	trnstree.ref \
	walktree.ref \
	which.ref

SCRIPTS	= \
	run_test.sh \
	abspath.sh \
	dlettree.sh \
	editfile.sh \
	edittree.sh \
	file2arg.sh \
	file2mem.sh \
	filecopy.sh \
	fp2argv.sh \
	name2s.sh \
	pathcmp.sh \
	pathhead.sh \
	pathleaf.sh \
	relpath.sh \
	sameleaf.sh \
	trimpath.sh \
	trnstree.sh \
	walktree.sh \
	which.sh

C_SRC	=

SOURCES	= Makefile $(C_SRC) $(SCRIPTS) $(REF_FILES) $(DATA_FILES)

PROGS	= \
	abspath \
	dlettree \
	editfile \
	edittree \
	file2arg \
	file2mem \
	fileblox \
	filecopy \
	fp2argv \
	name2s \
	pathcmp \
	pathhead \
	pathleaf \
	relpath \
	sameleaf \
	trimpath \
	trnstree \
	walktree \
	which

####### (Standard Productions) #################################################
include ../td_rules.mk

####### (Details of Productions) ###############################################

# rules to make SunOs 'make' "work" with VPATH

abspath.c \
dlettree.c \
editfile.c \
edittree.c \
file2arg.c \
file2mem.c \
fileblox.c \
filecopy.c \
fp2argv.c \
name2s.c \
pathcmp.c \
pathhead.c \
pathleaf.c \
relpath.c \
sameleaf.c \
trimpath.c \
trnstree.c \
walktree.c \
which.c :
