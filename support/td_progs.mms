# $Id: td_progs.mms,v 7.0 1991/12/13 09:18:32 ste_cm Rel $
# MMS script for program development with PORTUNIX
#
# $Log: td_progs.mms,v $
# Revision 7.0  1991/12/13 09:18:32  ste_cm
# BASELINE Thu Jul 16 16:14:02 1992 -- CM_TOOLS #11
#
#	Revision 6.0  91/12/13  09:18:32  ste_cm
#	BASELINE Fri Dec 13 13:18:38 1991
#	
#	Revision 5.1  91/12/13  09:18:32  dickey
#	changed standard-rules to double-colon form so I can append to them
#	
#	Revision 5.0  90/06/26  10:16:46  ste_cm
#	BASELINE Tue Jun 11 16:44:53 1991 -- apollo sr10.3
#	
#	Revision 4.0  90/06/26  10:16:46  ste_cm
#	BASELINE Tue Aug 14 16:27:44 1990
#	
#	Revision 3.1  90/06/26  10:16:46  dickey
#	added to 'clean' rule some stuff that might be accidentally copied
#	from UNIX in a tar-archive
#	
#	Revision 3.0  89/04/13  10:41:36  ste_cm
#	BASELINE Fri Oct  6 15:23:19 EDT 1989 -- support:ada_pits_000(rel3)
#	
#	Revision 2.0  89/04/13  10:41:36  ste_cm
#	BASELINE Mon Jun 12 09:07:53 EDT 1989
#	
#	Revision 1.1  89/04/13  10:41:36  dickey
#	Initial revision
#	
#
ALL		= [-.BIN]$(PROGRAM).EXE
.INCLUDE	PORTUNIX_ROOT:[SUPPORT]LIBRARY_RULES
#
ALL ::		$(ALL)	; @ WRITE SYS$OUTPUT "** produced $@"
CLEAN ::
	- remove -f *.LIS;* *.LOG;* *.MAP;* TEST_*.*;*
	- remove -f *.OUT;* [-.BIN]$(PROGRAM).;*
CLOBBER ::	CLEAN	; - remove -f *.OBJ;* $(ALL) [-.BIN]*.OBJ;*
DESTROY ::		; - remove -f *.*;*
INSTALL ::	$(B)$(PROGRAM).EXE
		@ WRITE SYS$OUTPUT "** completed $@"
PURGE :
	- purge [-.*]*.*
DEINSTALL ::
	- remove -f $(B)$(PROGRAM).EXE
#
# Install the program, cleanup (since testing is done)
$(B)$(PROGRAM).EXE :	[-.BIN]$(PROGRAM).EXE
	COPY [-.BIN]$(PROGRAM).EXE $(B)
	- PURGE [-.BIN]
#
# Build the program using the PORTUNIX library
# If other objects are necessary, define them as "OBJ_ARGS", with a leading
# comma to keep the syntax straight.
[-.BIN]$(PROGRAM).EXE :	[-.BIN]$(PROGRAM).OBJ $(OBJ_ARGS), $(LIB_DEPS)
#
# Put ".H" dependencies on the "$(PROGRAM).OBJ" line.
[-.BIN]$(PROGRAM).OBJ :	$(PROGRAM).C
