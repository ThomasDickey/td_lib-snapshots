# $Id: Makefile,v 12.1 1994/07/02 15:17:59 tom Exp $
# MAKE-file for process-management library routines

####### (Environment) ##########################################################
include ../td_defns.mk

####### (Standard Lists) #######################################################
CSRC	=\
	bldarg.c \
	catarg.c \
	catchall.c \
	execute.c \
	failed.c \
	for_admn.c \
	for_user.c \
	gethome.c \
	getuser.c \
	interact.c \
	makeargv.c \
	revert.c \
	savesuid.c \
	shoarg.c \
	showuids.c \
	usercopy.c \
	userprot.c \
	walkback.c

SOURCES	= Makefile descrip.mms $(CSRC)

OBJS	=\
	$Z(bldarg.o) \
	$Z(catarg.o) \
	$Z(catchall.o) \
	$Z(execute.o) \
	$Z(failed.o) \
	$Z(for_admn.o) \
	$Z(for_user.o) \
	$Z(gethome.o) \
	$Z(getuser.o) \
	$Z(interact.o) \
	$Z(makeargv.o) \
	$Z(revert.o) \
	$Z(savesuid.o) \
	$Z(shoarg.o) \
	$Z(showuids.o) \
	$Z(usercopy.o) \
	$Z(userprot.o) \
	$Z(walkback.o)

####### (Standard Productions) #################################################
include ../td_rules.mk
