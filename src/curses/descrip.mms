# $Id: descrip.mms,v 12.4 2002/01/13 16:27:30 tom Exp $
# MMS-file for curses library routines	

####### (Command-line Options) #################################################
include [-]td_defns.mms

####### (Standard Lists) #######################################################
LIBRARYMODULES = \
	ADDCHNST, \
	BEEP, \
	BLIP, \
	CMDCH, \
	DUMPCHR, \
	DUMPWIN, \
	ERASECHR, \
	KILLCHAR, \
	LOGCH, \
	ON_WINCH, \
	PADEDIT, \
	RAWGETS, \
	RAWTERM, \
	RESIZWIN, \
	SAVEWIN, \
	SCR_SIZE, \
	VIEWFILE, \
	WIN2FILE, \
	WREPAINT
#
C_SRC	= \
	addchnst.c \
	beep.c \
	blip.c \
	cmdch.c \
	dumpchr.c \
	dumpwin.c \
	erasechr.c \
	killchar.c \
	logch.c \
	on_winch.c \
	padedit.c \
	rawgets.c \
	rawterm.c \
	resizwin.c \
	savewin.c \
	scr_size.c \
	viewfile.c \
	win2file.c \
	wrepaint.c

####### (Standard Productions) #################################################
include [-]td_rules.mms

####### (Details of Productions) ###############################################
! Dependencies to archive are done by default rules, e.g.,
!'$(A)(UNIXDIR) :	UNIXDIR.OBJ
$(A)($(LIBRARYMODULES)) :	$(PTYPES_H) $(I)td_curse.h
$(A)(RAWGETS) :			$(I)dyni_str.h
