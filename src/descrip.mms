# $Id: descrip.mms,v 12.1 1993/11/05 17:57:35 dickey Exp $
# MMS-file for miscellaneous library routines	
#
####### (Development) ##########################################################
#
####### (Command-line Options) #################################################
A	=	[-.LIB]TD_LIB.OLB
I	=	[-.INCLUDE]
INCLUDES =	/Include=($(I),PORTUNIX_ROOT:[INCLUDE])
CFLAGS =	$(INCLUDES) /LIST /OBJECT=$@
LINKFLAGS =	/MAP/CROSS_REFERENCE/EXEC=$@
C_RTL =		SYS$LIBRARY:VAXCRTL.OLB		! C runtime library
TEST_CC =	$(CC) $(CFLAGS) /DEFINE="TEST" $(MMS$SOURCE)
#
####### (Standard Lists) #######################################################
LIBRARYMODULES = -
	ACC_MODE, -
	ADD2FNAM, -
	ARG2FILE, -
	BEEP, -
	BLDARG, -
	BLIP, -
	CATARG, -
	CATCHALL, -
	CMPQSORT, -
	COPYBACK, -
	COUNT_SS, -
	DFTENV, -
	DOALLOC, -
	DOTCMP, -
	DOTNAME, -
	DUMPCHR, -
	DYN_APP, -
	DYN_APPC, -
	DYN_CATA, -
	DYN_COPY, -
	DYN_STR, -
	EXECUTE, -
	FAILED, -
	FIELD_OF, -
	FILE2ARG, -
	FILE2MEM, -
	FILESIZE, -
	FP2ARGV, -
	FTYPE, -
	IS_SPATH, -
	ISTEXTFL, -
	LSBYCOLS, -
	LTOSTR, -
	M2COMP, -
	MAKEARGV, -
	MEM2FILE, -
	MODECHAR, -
	NEXT_VER, -
	PATHCAT, -
	PATHLEAF, -
	SAMELEAF, -
	SCOMP, -
	SHOARG, -
	SHOWUIDS, -
	SS_FIELD, -
	STAT_DIR, -
	STAT_FIL, -
	STRALLOC, -
	STRBCMP, -
	STRCLEAN, -
	STRCOUNT, -
	STRTRIM, -
	STRUCMP, -
	STRUCPY, -
	STRWCMP, -
	TRIMPATH, -
	TXTALLOC, -
	VECALLOC, -
	VECEDIT, -
	VECLEN, -
	VERCMP, -
	WALKBACK
UNUSED_MODULES = -
	ABSPATH, 		# unix-style 'getwd()' -
	CMDCH, 			# curses -
	CUTOFF, 		# uses PACKDATE -
	DENODE, 		# only for ABSPATH -
	DIFFLOAD,		# unix-diff -
	DUMPWIN, 		# curses -
	ERASECHR, 		# curses -
	FILECOPY, 		# needs unix system-I/O -
	FOR_ADMN, 		# only for setuid programs -
	FOR_USER, 		# only for setuid programs -
	FTYPE2, 		# unix-style suffix -
	GETHOME, 		# unix-passwd -
	GETUSER, 		# unix-passwd -
	GID2S, 			# PORTUNIX -
	INTERACT,		# unix filesystem -
	KILLCHAR, 		# curses -
	LOGCH, 			# curses/ded -
	NAME2S,			# unix-style names only -
	NEWZONE,		# unix environment -
	PACKDATE,		# localtime() -
	PADEDIT,		# apollo or termcap -
	PATHCMP,		# uses ABSPATH -
	PATHHEAD,		# unix-style pathnames -
	RAWGETS,		# curses -
	RAWTERM,		# curses -
	RCSDEBUG,		# rcs (unix-only) -
	RCS_DIR,		# rcs (unix-only) -
	RCSARGPR,		# rcs (unix-only) -
	RCSEDIT,		# rcs (unix-only) -
	RCSKEYS,		# rcs (unix-only) -
	RCSLAST,		# rcs (unix-only) -
	RCSLOAD,		# rcs (unix-only) -
	RCSLOCKS,		# rcs (unix-only) -
	RCSNAME,		# rcs (unix-only) -
	RCSPATH,		# rcs (unix-only) -
	RCSPERM,		# rcs (unix-only) -
	RCSSYMBS,		# rcs (unix-only) -
	RCSTEMP,		# rcs (unix-only) -
	RCSTIME,		# rcs (unix-only) -
	RELPATH,		# unix-style 'getwd()' -
	RESIZWIN,		# curses (apollo) -
	REVERT,			# unix setuid -
	S2GID,			# unix-passwd -
	S2UID,			# unix-passwd -
	SAVESUID,		# unix -
	SAVEWIN,		# curses -
	SCCS_DIR,		# sccs (unix-only) -
	SCCSLAST,		# sccs (unix-only) -
	SCCSNAME,		# sccs (unix-only) -
	SCR_SIZE,		# curses/apollo/termcap -
	SETMTIME,		# utime() call -
	TRACK_WD,		# unix-style 'getwd()' -
	UID2S,			# PORTUNIX -
	USERCOPY, 		# uses FOR_USER -
	USERPROT, 		# uses FOR_USER -
	VCS_FILE,		# rcs (unix-only) -
	VIEWFILE,		# curses/unix -
	WHICH,			# unix-style PATH-variable -
	WIN2FILE,		# curses -
	WREPAINT,		# curses -
	WALKTREE
#
C_SRC	=-
	ABSPATH.C -
	ACC_MODE.C -
	ADD2FNAM.C -
	ARG2FILE.C -
	BEEP.C -
	BLDARG.C -
	BLIP.C -
	CATARG.C -
	CATCHALL.C -
	CMDCH.C -
	CMPQSORT.C, -
	COPYBACK.C -
	COUNT_SS.C -
	CUTOFF.C -
	DENODE.C -
	DFTENV.C -
	DIFFLOAD.C -
	DOALLOC.C -
	DOTCMP.C -
	DOTNAME.C -
	DUMPCHR.C -
	DUMPWIN.C -
	DYN_APP.C -
	DYN_APPC.C -
	DYN_CATA.C -
	DYN_COPY.C -
	DYN_STR.C -
	ERASECHR.C -
	EXECUTE.C -
	FAILED.C -
	FIELD_OF.C -
	FILE2ARG.C -
	FILE2MEM.C -
	FILECOPY.C -
	FILESIZE.C -
	FOR_ADMN.C -
	FOR_USER.C -
	FP2ARGV.C -
	FTYPE.C -
	FTYPE2.C -
	GETHOME.C -
	GETUSER.C -
	GID2S.C -
	IS_SPATH.C -
	INTERACT.C -
	ISTEXTFL.C -
	KILLCHAR.C -
	LSBYCOLS.C -
	LOGCH.C -
	LTOSTR.C -
	M2COMP.C -
	MAKEARGV.C -
	MEM2FILE.C -
	MODECHAR.C -
	NEXT_VER.C -
	NAME2S.C -
	NEWZONE.C -
	PACKDATE.C -
	PADEDIT.C -
	PATHCAT.C -
	PATHCMP.C -
	PATHHEAD.C -
	PATHLEAF.C -
	RAWGETS.C -
	RAWTERM.C -
	RCSDEBUG.C -
	RCS_DIR.C -
	RCSARGPR.C -
	RCSEDIT.C -
	RCSKEYS.C -
	RCSLAST.C -
	RCSLOAD.C -
	RCSLOCKS.C -
	RCSNAME.C -
	RCSPATH.C -
	RCSPERM.C -
	RCSSYMBS.C -
	RCSTEMP.C -
	RCSTIME.C -
	RELPATH.C -
	RESIZWIN.C -
	REVERT.C -
	S2GID.C -
	S2UID.C -
	SAMEBR.C -
	SAMELEAF.C -
	SAVEWIN.C -
	SCCS_DIR.C -
	SCCSLAST.C -
	SCCSNAME.C -
	SCOMP.C -
	SCR_SIZE.C -
	SETMTIME.C -
	SHOARG.C -
	SHOWUIDS.C -
	SS_FIELD.C -
	STAT_DIR.C -
	STAT_FIL.C -
	STRALLOC.C -
	STRBCMP.C -
	STRCLEAN.C -
	STRCOUNT.C -
	STRTRIM.C -
	STRUCMP.C -
	STRUCPY.C -
	STRWCMP.C -
	TRACK_WD.C -
	TRIMPATH.C -
	TXTALLOC.C -
	UID2S.C -
	USERCOPY.C -
	USERPROT.C -
	VCS_FILE.C -
	VECALLOC.C -
	VECEDIT.C -
	VECLEN.C -
	VERCMP.C -
	VIEWFILE.C -
	WALKBACK.C -
	WALKTREE.C -
	WHICH.C -
	WIN2FILE.C -
	WREPAINT.C
#
PTYPES_H = $(I)PTYPES.H $(I)TD_LIB.H
#
####### (Standard Productions) #################################################
LIBRARIES =	$(A)($(LIBRARYMODULES)) -	! project library
		$(C_RTL)
!
.FIRST :
	@ WRITE SYS$OUTPUT "** support library make: $@"
.LAST :
	- purge *.obj
	- remove -f *.dia;*
	@ WRITE SYS$OUTPUT "** done"
#
ALL	= $(A)
#
ALL :		$(A)($(LIBRARYMODULES))
		@ WRITE SYS$OUTPUT "** produced $@"
CLEAN :
		- purge
		- remove -vf *.LIS;* *.LOG;* TEST_*.*;*
		- remove -vf *.OUT;* [-.LIB]*.A;* [-.LIB]*.LN;* $(I)*.LN;*
CLOBBER :	CLEAN
		- remove -vf *.OBJ;* $(ALL)
DESTROY :	
		- remove -vf *.*;*
INSTALL :	
		@ WRITE SYS$OUTPUT "** no action for $@"
#
#
####### (Details of Productions) ###############################################
! Dependencies to archive are done by default rules, e.g.,
!'$(A)(UNIXDIR) :	UNIXDIR.OBJ
$(A)($(LIBRARYMODULES)) : $(PTYPES_H)
ACC_MODE.obj :		$(PTYPES_H)
ADD2FNAM.obj :		$(PTYPES_H)
ARG2FILE.obj :		$(PTYPES_H)
BEEP.obj :		$(PTYPES_H)
CATARG.obj :		$(PTYPES_H)
CATCHALL.obj :		$(PTYPES_H)
CMDCH.obj :		$(PTYPES_H)	$(I)td_curse.h
CMPQSORT.obj :		$(PTYPES_H)	$(I)td_qsort.h
COPYBACK.obj :		$(PTYPES_H)
CUTOFF.obj :		$(PTYPES_H)
DOALLOC.OBJ :		$(PTYPES_H)
DUMPWIN.obj :		$(PTYPES_H)
DYN_APP.obj \
DYN_APPC.obj \
DYN_CATA.obj \
DYN_COPY.obj \
DYN_STR.obj :		$(PTYPES_H)	$(I)dyn_str.h
ERASECHR.obj :		$(PTYPES_H)
EXECUTE.obj :		$(PTYPES_H)
FAILED.obj :		$(PTYPES_H)
FILE2ARG.obj :		$(PTYPES_H)
FILE2MEM.obj :		$(PTYPES_H)
FILECOPY.obj :		$(PTYPES_H)
FILESIZE.obj :		$(PTYPES_H)
FOR_ADMN.obj :		$(PTYPES_H)
FOR_USER.obj :		$(PTYPES_H)
FP2ARGV.obj :		$(PTYPES_H)
FTYPE.obj :		$(PTYPES_H)
FTYPE2.obj :		$(PTYPES_H)
GID2S.obj :		$(PTYPES_H)
IS_SPATH.obj :		$(PTYPES_H)
ISTEXTFL.obj :		$(PTYPES_H)
KILLCHAR.obj :		$(PTYPES_H)
LSBYCOLS.obj :		$(PTYPES_H)
LOGCH.obj :		$(PTYPES_H)	$(I)td_curse.h
MEM2FILE.obj :		$(PTYPES_H)
NAME2S.obj :		$(PTYPES_H)
NEWZONE.obj :		$(PTYPES_H)
PACKDATE.obj :		$(PTYPES_H)
PADEDIT.obj :		$(PTYPES_H)
PATHCAT.obj :		$(PTYPES_H)
PATHCMP.obj :		$(PTYPES_H)
PATHLEAF.obj :		$(PTYPES_H)
RAWGETS.obj :		$(PTYPES_H)	$(I)td_curse.h
RAWTERM.obj :		$(PTYPES_H)
# rcsargpr.obj :	$(PTYPES_H)	$(I)rcsdefs.h
# rcsedit.obj :		$(PTYPES_H)	$(I)rcsdefs.h
# rcskeys.obj :		$(PTYPES_H)	$(I)rcsdefs.h
# rcslast.obj :		$(PTYPES_H)	$(I)rcsdefs.h
# rcsload.obj :		$(PTYPES_H)	$(I)rcsdefs.h
# rcslocks.obj :	$(PTYPES_H)	$(I)rcsdefs.h
# rcsname.obj :		$(PTYPES_H)	$(I)rcsdefs.h
# rcspath.obj :		$J/rcspath.h	; $(CC) -I$J -c $*.c
# rcsperm.obj :		$(PTYPES_H)	$(I)rcsdefs.h
RESIZWWIN.obj :		$(PTYPES_H)
# samebr.obj :		$(PTYPES_H)	$(I)rcsdefs.h
SAMELEAF.obj :		$(PTYPES_H)
SAVEWIN.obj :		$(PTYPES_H)
SCCSLAST.obj :		$(PTYPES_H)
SETMTIME.obj :		$(PTYPES_H)
SHOARG.obj :		$(PTYPES_H)
SHOWUIDS.obj :		$(PTYPES_H)
STAT_DIR.obj :		$(PTYPES_H)
STAT_FIL.obj :		$(PTYPES_H)
STRALLOC.obj :		$(PTYPES_H)
STRCLEAN.obj :		$(PTYPES_H)
STRCOUNT.obj :		$(PTYPES_H)
STRUCPY.obj :		$(PTYPES_H)
STRWCMP.obj :		$(PTYPES_H)
TRIMPATH.obj :		$(PTYPES_H)
TXTALLOC.obj :		$(PTYPES_H)
UID2S.obj :		$(PTYPES_H)
USERCOPY.obj :		$(PTYPES_H)
USERPROT.obj :		$(PTYPES_H)
# vcs_file.obj :	$(PTYPES_H)	$(I)rcsdefs.h
WALKBACK.OBJ :		$(PTYPES_H)
WALKTREE.OBJ :		$(PTYPES_H)
WIN2FILE.obj :		$(PTYPES_H)
#
M2COMP.obj :		$(PTYPES_H)	$(I)td_scomp.h
SCOMP.obj :		$(PTYPES_H)	$(I)td_scomp.h
#
COUNT_SS.obj :		$(PTYPES_H)	$(I)td_sheet.h	$(I)dyn_str.h
FIELD_OF.obj :		$(PTYPES_H)	$(I)td_sheet.h	$(I)dyn_str.h
SS_FIELD.obj :		$(PTYPES_H)	$(I)td_sheet.h	$(I)dyn_str.h
#
####### (Development) ##########################################################
!
.OBJ.EXE :
		$(LINK) $(LINKFLAGS) $(MMS$TARGET_NAME),$(A)/LIB,$(C_RTL)/LIB
!
! Rules for test-programs
TEST_EXECUTE.EXE :	TEST_EXECUTE.OBJ
TEST_LTOSTR.EXE :	TEST_LTOSTR.OBJ
TEST_TXTALLOC.EXE :	TEST_TXTALLOC.OBJ
TEST_VERCMP.EXE :	TEST_VERCMP.OBJ
TEST_WALKTREE.EXE :	TEST_WALKTREE.OBJ
!
TEST_EXECUTE.OBJ :	EXECUTE.C		; $(TEST_CC)
TEST_LTOSTR.OBJ :	LTOSTR.C		; $(TEST_CC)
TEST_TXTALLOC.OBJ :	TXTALLOC.C		; $(TEST_CC)
TEST_VERCMP.OBJ :	VERCMP.C		; $(TEST_CC)
TEST_WALKTREE.OBJ :	WALKTREE.C		; $(TEST_CC)
