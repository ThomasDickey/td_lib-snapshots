# $Id: descrip.mms,v 12.1 1994/07/02 15:58:40 tom Exp $
# MMS-file for UNIX-oriented library routines	

####### (Development) ##########################################################
include [-]td_defns.mms

LIBRARYMODULES = \
	SETMTIME

UNUSED_MODULES = \
	GID2S, 			# PORTUNIX -
	S2GID, 			# PORTUNIX -
	S2UID, 			# PORTUNIX -
	UID2S,			# PORTUNIX -

C_SRC	= \
	gid2s.c \
	s2gid.c \
	s2uid.c \
	setmtime.c \
	uid2s.c

####### (Standard Productions) #################################################
include [-]td_rules.mms
