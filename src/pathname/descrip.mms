# $Id: descrip.mms,v 12.2 1994/07/02 15:37:39 tom Exp $
# MMS-file for filename & I/O library routines	

####### (Development) ##########################################################
include [-]td_defns.mms

####### (Standard Lists) #######################################################
LIBRARYMODULES = \
	ACC_MODE, \
	ADD2FNAM, \
	ARG2FILE, \
	COPYBACK, \
	DENODE, \
	DOTNAME, \
	FILE2ARG, \
	FILE2MEM, \
	FILESIZE, \
	FLEAF, \
	FP2ARGV, \
	FTYPE, \
	FTYPE2, \
	IS_SPATH, \
	ISTEXTFL, \
	MEM2FILE, \
	MODECHAR, \
	NAME2S, \
	PATHCAT, \
	PATHLEAF, \
	RELPATH, \
	SAMELEAF, \
	STAT_DIR, \
	STAT_FIL, \
	TRIMPATH, \
	WALKTREE

UNUSED_MODULES = \
	ABSPATH, 		# unix-style 'getwd()' -
	FILECOPY, 		# needs unix system-I/O -
	FTYPE2, 		# unix-style suffix -
	PATHCMP,		# uses ABSPATH -
	PATHHEAD,		# unix-style pathnames -
	RELPATH,		# unix-style 'getwd()' -
	TRACK_WD,		# unix-style 'getwd()' -
	WHICH			# unix-style PATH-variable -
#
C_SRC	= \
	abspath.c \
	acc_mode.c \
	add2fnam.c \
	arg2file.c \
	copyback.c \
	denode.c \
	dotname.c \
	file2arg.c \
	file2mem.c \
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
	walktree.c \
	which.c

####### (Standard Productions) #################################################
include [-]td_rules.mms

####### (Details of Productions) ###############################################
# Dependencies to archive are done by default rules, e.g.,
#'$(A)(UNIXDIR) :	UNIXDIR.OBJ
$(A)($(LIBRARYMODULES)) : $(PTYPES_H)
