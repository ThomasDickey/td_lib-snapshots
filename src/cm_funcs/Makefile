# $Id: Makefile,v 12.1 1994/07/02 12:56:29 tom Exp $
# MAKE-file for miscellaneous library routines

####### (Environment) ##########################################################
include ../td_defns.mk

####### (Standard Lists) #######################################################
CSRC	=\
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

SOURCES	= Makefile descrip.mms $(CSRC)

OBJS	=\
	$Z(cutoff.o) \
	$Z(newzone.o) \
	$Z(next_ver.o) \
	$Z(packdate.o) \
	$Z(rcs_dir.o) \
	$Z(rcsargpr.o) \
	$Z(rcsdebug.o) \
	$Z(rcsedit.o) \
	$Z(rcskeys.o) \
	$Z(rcslast.o) \
	$Z(rcsload.o) \
	$Z(rcslocks.o) \
	$Z(rcsname.o) \
	$Z(rcspath.o) \
	$Z(rcsperm.o) \
	$Z(rcssymbs.o) \
	$Z(rcstemp.o) \
	$Z(rcstime.o) \
	$Z(samebr.o) \
	$Z(sccs_dir.o) \
	$Z(sccsdbug.o) \
	$Z(sccslast.o) \
	$Z(sccsname.o) \
	$Z(vcs_file.o) \
	$Z(vercmp.o)

ALL	= $Z

####### (Standard Productions) #################################################
include ../td_rules.mk

####### (Details of Productions) ###############################################
$Z(rcsargpr.o)\
$Z(rcsedit.o)\
$Z(rcskeys.o)\
$Z(rcslast.o)\
$Z(rcsload.o)\
$Z(rcslocks.o)\
$Z(rcsname.o)\
$Z(rcspath.o)\
$Z(rcsperm.o)\
$Z(rcssymbs.o)\
$Z(rcstime.o)\
$Z(samebr.o)\
$Z(vcs_file.o):	$(RCSDEFS_H)

$Z(sccsdbug.o)\
$Z(sccslast.o)\
$Z(sccsname.o):	$(SCCSDEFS_H)
