# $Id: descrip.mms,v 12.2 1994/07/02 16:00:22 tom Exp $
# MMS-file for memory-allocation library routines	

####### (Development) ##########################################################
include [-]td_defns.mms

####### (Standard Lists) #######################################################
LIBRARYMODULES = \
	DOALLOC, \
	MEMMOVE, \
	STRALLOC, \
	TXTALLOC, \
	VECALLOC, \
	VECEDIT, \
	VECLEN

C_SRC	= \
	doalloc.c
	memmove.c
	stralloc.c
	txtalloc.c
	vecalloc.c
	vecedit.c
	veclen.c

####### (Standard Productions) #################################################
include [-]td_rules.mms

####### (Details of Productions) ###############################################
! Dependencies to archive are done by default rules, e.g.,
!'$(A)(UNIXDIR) :	UNIXDIR.OBJ
$(A)($(LIBRARYMODULES)) : $(PTYPES_H)
