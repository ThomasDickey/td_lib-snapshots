# $Id: descrip.mms,v 12.2 1994/07/02 13:25:57 tom Exp $
# MMS-file for CM_FUNCS library routines	

####### (Command-line Options) #################################################
include [-]td_defns.mms

####### (Standard Lists) #######################################################
C_SRC	= \
	cutoff.c \
	newzone.c \
	next_ver.c \
	packdate.c \
	rcs_dir.c \
	rcsargpr.c \
	rcsdebug.c \
	rcsedit.c \
	rcskeys.c \
	rcslast.c \
	rcsload.c \
	rcslocks.c \
	rcsname.c \
	rcspath.c \
	rcsperm.c \
	rcssymbs.c \
	rcstemp.c \
	rcstime.c \
	samebr.c \
	sccs_dir.c \
	sccsdbug.c \
	sccslast.c \
	sccsname.c \
	vcs_file.c \
	vercmp.c

####### (Standard Productions) #################################################
include [-]td_rules.mms
