# $Id: td_progs.mms,v 8.0 1992/11/04 07:18:57 ste_cm Rel $
# MMS script for program development with PORTUNIX
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
