# $Id: Makefile,v 12.5 1995/01/28 21:10:25 tom Exp $
# MAKE-file for process-management library routines

####### (Environment) ##########################################################
include ../td_defns.mk
include ../../support/td_lib.mk

####### (Standard Lists) #######################################################
CSRC	=\
	bldarg.c \
	catarg.c \
	catchall.c \
	egress.c \
	execute.c \
	failed.c \
	for_admn.c \
	for_user.c \
	gethome.c \
	getopt.c \
	getuser.c \
	in_group.c \
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
	$Z(egress.o) \
	$Z(execute.o) \
	$Z(failed.o) \
	$Z(for_admn.o) \
	$Z(for_user.o) \
	$Z(gethome.o) \
	$Z(getopt.o) \
	$Z(getuser.o) \
	$Z(in_group.o) \
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
