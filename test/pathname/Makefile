# $Id: Makefile,v 12.2 1994/07/16 20:11:01 tom Exp $
# make-file for TD_LIB common/test library

####### (Development) ##########################################################
THIS	= pathname
include ../td_defns.mk

####### (Standard Lists) #######################################################
REF_FILES = \
	abspath.ref \
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
	walktree.ref \
	which.ref

SCRIPTS	= \
	run_test.sh \
	abspath.sh \
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
	walktree.sh \
	which.sh

C_SRC	=

SOURCES	= Makefile $(C_SRC) $(SCRIPTS) $(REF_FILES) $(DATA_FILES)

PROGS	= \
	abspath \
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
	walktree \
	which

####### (Standard Productions) #################################################
include ../td_rules.mk

####### (Details of Productions) ###############################################

# rules to make SunOs 'make' "work" with VPATH

abspath.c \
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
walktree.c \
which.c :
