# $Id: descrip.mms,v 12.1 1994/07/02 15:34:08 tom Exp $
# MMS-file for spreadsheet-data library routines	

####### (Development) ##########################################################
include [-]td_defns.mms

####### (Standard Lists) #######################################################
LIBRARYMODULES = \
	COUNT_SS, \
	FIELD_OF, \
	SS_FIELD

C_SRC	= \
	count_ss.c \
	field_of.c \
	ss_field.c

####### (Standard Productions) #################################################
include [-]td_rules.mms

####### (Details of Productions) ###############################################
# Dependencies to archive are done by default rules, e.g.,
#'$(A)(UNIXDIR) :	UNIXDIR.OBJ
$(A)($(LIBRARYMODULES)) : $(PTYPES_H) $(I)td_sheet.h $(I)dyn_str.h
