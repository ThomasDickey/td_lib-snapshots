# $Id: descrip.mms,v 12.1 1994/07/02 15:51:21 tom Exp $
# MMS-file for string-handling library routines	

####### (Development) ##########################################################
include [-]td_defns.mms

####### (Standard Lists) #######################################################
LIBRARYMODULES = \
	DFTENV, \
	DOTCMP, \
	DYN_APP, \
	DYN_APPC, \
	DYN_CATA, \
	DYN_COPY, \
	DYN_STR, \
	LSBYCOLS, \
	LTOSTR, \
	STRBCMP, \
	STRCLEAN, \
	STRCOUNT, \
	STRERROR, \
	STRTRIM, \
	STRUCMP, \
	STRUCPY, \
	STRWCMP

C_SRC	= \
	dftenv.c \
	dotcmp.c \
	dyn_app.c \
	dyn_appc.c \
	dyn_cata.c \
	dyn_copy.c \
	dyn_str.c \
	lsbycols.c \
	ltostr.c \
	strbcmp.c \
	strclean.c \
	strcount.c \
	strerror.c \
	strtrim.c \
	strucmp.c \
	strucpy.c \
	strwcmp.c

####### (Standard Productions) #################################################
include [-]td_rules.mms

####### (Details of Productions) ###############################################
# Dependencies to archive are done by default rules, e.g.,
#'$(A)(UNIXDIR) :	UNIXDIR.OBJ
$(A)($(LIBRARYMODULES)) : $(PTYPES_H)
DYN_APP.obj \
DYN_APPC.obj \
DYN_CATA.obj \
DYN_COPY.obj \
DYN_STR.obj :		$(PTYPES_H)	$(I)dyn_str.h
STRCLEAN.obj :		$(PTYPES_H)
STRCOUNT.obj :		$(PTYPES_H)
STRUCPY.obj :		$(PTYPES_H)
STRWCMP.obj :		$(PTYPES_H)
