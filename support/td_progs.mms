# $Id: td_progs.mms,v 12.1 1994/08/21 23:02:36 tom Exp $
# MMS script for program development with TD_LIB
#
# (originally named PROGRAM_RULES.MMS)

ALL		= [-.BIN]$(PROGRAM).EXE
.INCLUDE	TD_LIB_ROOT:[SUPPORT]TD_LIB
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
# Build the program using the TD_LIB library
# If other objects are necessary, define them as "OBJ_ARGS", with a leading
# comma to keep the syntax straight.
[-.BIN]$(PROGRAM).EXE :	[-.BIN]$(PROGRAM).OBJ $(OBJ_ARGS), $(LIB_DEPS)
#
# Put ".H" dependencies on the "$(PROGRAM).OBJ" line.
[-.BIN]$(PROGRAM).OBJ :	$(PROGRAM).C
