# $Id: descrip.mms,v 12.4 1994/11/12 22:37:59 tom Exp $
# MMS-file for miscellaneous library routines	

####### (Development) ##########################################################
include [-]td_defns.mms

####### (Standard Lists) #######################################################
LIBRARYMODULES = \
	BLDARG, \
	CATARG, \
	CATCHALL, \
	EXECUTE, \
	FAILED, \
	MAKEARGV, \
	SHOARG, \
	SHOWUIDS, \
	WALKBACK

UNUSED_MODULES = -
	EGRESS,			# unix process -
	FOR_ADMN, 		# only for setuid programs -
	FOR_USER, 		# only for setuid programs -
	GETHOME, 		# unix-passwd -
	GETUSER, 		# unix-passwd -
	IN_GROUP,		# unix process -
	INTERACT,		# unix filesystem -
	REVERT,			# unix setuid -
	SAVESUID,		# unix -
	USERCOPY, 		# uses FOR_USER -
	USERPROT, 		# uses FOR_USER -
	WALKBACK

C_SRC	= \
	bldarg.c \
	catarg.c \
	catchall.c \
	execute.c \
	failed.c \
	for_admn.c \
	for_user.c \
	gethome.c \
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

####### (Standard Productions) #################################################
include [-]td_rules.mms

####### (Details of Productions) ###############################################
# Dependencies to archive are done by default rules, e.g.,
#'$(A)(UNIXDIR) :	UNIXDIR.OBJ
$(A)($(LIBRARYMODULES)) : $(PTYPES_H)
