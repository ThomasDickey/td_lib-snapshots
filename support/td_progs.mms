# $Header: /users/source/archives/td_lib.vcs/support/RCS/td_progs.mms,v 3.0 1989/04/13 10:41:36 ste_cm Rel $
# MMS script for program development with PORTUNIX
#
# $Log: td_progs.mms,v $
# Revision 3.0  1989/04/13 10:41:36  ste_cm
# BASELINE Fri Oct  6 15:23:19 EDT 1989 -- support:ada_pits_000(rel3)
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
ALL :		$(ALL)	; @ WRITE SYS$OUTPUT "** produced $@"
CLEAN :			; - remove -f *.LIS;* *.LOG;* *.MAP;* TEST_*.*;*
CLOBBER :	CLEAN	; - remove -f *.OBJ;* $(ALL) [-.BIN]*.OBJ;*
DESTROY :		; - remove -f *.*;*
INSTALL :	$(B)$(PROGRAM).EXE
		@ WRITE SYS$OUTPUT "** completed $@"
PURGE :
	- purge [-.*]*.*
DEINSTALL :
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
