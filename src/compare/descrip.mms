# $Id: descrip.mms,v 12.3 1995/07/30 20:28:58 tom Exp $
# MMS-file for comparison/sorting library routines	

####### (Command-line Options) #################################################
include [-]td_defns.mms

####### (Standard Lists) #######################################################
LIBRARYMODULES = \
	BTREE, \
	CMPQSORT, \
	DIFFLOAD, \
	M2COMP, \
	SCOMP
#
C_SRC	= \
	btree.c \
	cmpqsort.c \
	diffload.c \
	m2comp.c \
	scomp.c

####### (Standard Productions) #################################################
include [-]td_rules.mms

####### (Details of Productions) ###############################################
! Dependencies to archive are done by default rules, e.g.,
!'$(A)(UNIXDIR) :	UNIXDIR.OBJ
$(A)($(LIBRARYMODULES)) : $(PTYPES_H)
BTREE.obj :		$(PTYPES_H)	$(I)td_btree.h
CMPQSORT.obj :		$(PTYPES_H)	$(I)td_qsort.h
DIFFLOAD.obj :		$(PTYPES_H)	$(I)dyn_str.h

M2COMP.obj :		$(PTYPES_H)	$(I)td_scomp.h
SCOMP.obj :		$(PTYPES_H)	$(I)td_scomp.h
