# $Id: Makefile,v 12.4 1995/01/28 21:10:25 tom Exp $
# MAKE-file for filename and I/O library routines

####### (Environment) ##########################################################
include ../td_defns.mk
include ../../support/td_lib.mk

####### (Standard Lists) #######################################################
CSRC	=\
	abspath.c \
	acc_mode.c \
	add2fnam.c \
	arg2file.c \
	copyback.c \
	denode.c \
	dlettree.c \
	dotname.c \
	editfile.c \
	edittree.c \
	file2arg.c \
	file2mem.c \
	fileblox.c \
	filecopy.c \
	filesize.c \
	fleaf.c \
	fp2argv.c \
	ftype.c \
	ftype2.c \
	is_spath.c \
	istextfl.c \
	mem2file.c \
	modechar.c \
	name2s.c \
	pathcat.c \
	pathcmp.c \
	pathhead.c \
	pathleaf.c \
	relpath.c \
	sameleaf.c \
	stat_dir.c \
	stat_fil.c \
	track_wd.c \
	trimpath.c \
	trnstree.c \
	walktree.c \
	which.c

SOURCES	= Makefile descrip.mms $(CSRC)

OBJS	=\
	$Z(abspath.o) \
	$Z(acc_mode.o) \
	$Z(add2fnam.o) \
	$Z(arg2file.o) \
	$Z(copyback.o) \
	$Z(denode.o) \
	$Z(dlettree.o) \
	$Z(dotname.o) \
	$Z(editfile.o) \
	$Z(edittree.o) \
	$Z(file2arg.o) \
	$Z(file2mem.o) \
	$Z(fileblox.o) \
	$Z(filecopy.o) \
	$Z(filesize.o) \
	$Z(fleaf.o) \
	$Z(fp2argv.o) \
	$Z(ftype.o) \
	$Z(ftype2.o) \
	$Z(is_spath.o) \
	$Z(istextfl.o) \
	$Z(mem2file.o) \
	$Z(modechar.o) \
	$Z(name2s.o) \
	$Z(pathcat.o) \
	$Z(pathcmp.o) \
	$Z(pathhead.o) \
	$Z(pathleaf.o) \
	$Z(relpath.o) \
	$Z(sameleaf.o) \
	$Z(stat_dir.o) \
	$Z(stat_fil.o) \
	$Z(track_wd.o) \
	$Z(trimpath.o) \
	$Z(trnstree.o) \
	$Z(walktree.o) \
	$Z(which.o)

####### (Standard Productions) #################################################
include ../td_rules.mk

$Z(dlettree.o) \
$Z(edittree.o) \
$Z(trnstree.o) : $I/port2vms.h
