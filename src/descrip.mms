# $Id: descrip.mms,v 6.0 1990/03/28 08:26:37 ste_cm Rel $
# MMS-file for miscellaneous library routines	
#
####### (Development) ##########################################################
#
####### (Command-line Options) #################################################
A	=	[-.LIB]COMMON.OLB
I	=	[-.INTERFACE]
INCLUDES =	/Include=($(I),PORTUNIX_ROOT:[INTERFACE])
CFLAGS =	$(INCLUDES) /LIST /OBJECT=$@
LINKFLAGS =	/MAP/CROSS_REFERENCE/EXEC=$@
C_RTL =		SYS$LIBRARY:VAXCRTL.OLB		! C runtime library
TEST_CC =	$(CC) $(CFLAGS) /DEFINE="TEST" $(MMS$SOURCE)
#
####### (Standard Lists) #######################################################
LIBRARYMODULES = -
	ADD2FNAME, -
	ARGV2FILE, -
	BEEP, -
	BLDARG, -
	BLIP, -
	CATARG, -
	CATCHALL, -
	COPYBACK, -
	DFTENV, -
	DOALLOC, -
	DOTCMP, -
	DOTNAME, -
	DUMPCHR, -
	FAILED, -
	FILE2ARGV, -
	FILE2MEM, -
	FP2ARGV, -
	FTYPE, -
	ISTEXTFILE, -
	LIST_BY_COLS, -
	LTOSTR, -
	MAKEARGV, -
	MEM2FILE, -
	MODECHAR -
	PATHCAT, -
	PATHLEAF, -
	SAMELEAF -
	STRALLOC, -
	STRBCMP, -
	STRCLEAN, -
	STRTRIM, -
	STRUCMP, -
	STRUCPY, -
	STRWCMP, -
	TRIMPATH, -
	TXTALLOC, -
	VECALLOC, -
	VERCMP, -
	WALKTREE
UNUSED_MODULES = -
	ABSPATH, 		# unix-style 'getwd()' -
	CMDCH, 			# curses -
	CUTOFF, 		# uses PACKDATE -
	DENODE, 		# only for ABSPATH -
	DUMPWIN, 		# curses -
	ERASECHAR, 		# curses -
	EXECUTE, 		# unix-style fork/exec -
	FILECOPY, 		# needs unix system-I/O -
	FOR_USER, 		# only for setuid programs -
	FTYPE2, 		# unix-style suffix -
	GETHOME, 		# unix-passwd -
	GETUSER, 		# unix-passwd -
	GID2S, 			# PORTUNIX -
	KILLCHAR 		# curses -
	NAME2S,			# unix-style names only -
	NEWZONE,		# unix environment -
	PACKDATE,		# localtime() -
	PADEDIT,		# apollo or termcap -
	PATHCMP,		# uses ABSPATH -
	PATHHEAD,		# unix-style pathnames -
	RAWGETS,		# curses -
	RAWTERM,		# curses -
	RCS_DEBUG,		# rcs (unix-only) -
	RCS_DIR,		# rcs (unix-only) -
	RCSEDIT,		# rcs (unix-only) -
	RCSKEYS,		# rcs (unix-only) -
	RCSLAST,		# rcs (unix-only) -
	RCSLOAD,		# rcs (unix-only) -
	RCSLOCKS,		# rcs (unix-only) -
	RCSNAME,		# rcs (unix-only) -
	RCSPATH,		# rcs (unix-only) -
	RCSPERMIT,		# rcs (unix-only) -
	RCSTEMP,		# rcs (unix-only) -
	RELPATH,		# unix-style 'getwd()' -
	RESIZEWIN,		# curses (apollo) -
	REVERT,			# unix setuid -
	S2GID,			# unix-passwd -
	S2UID,			# unix-passwd -
	SAVEWIN,		# curses -
	SCCS_DIR,		# sccs (unix-only) -
	SCCSLAST,		# sccs (unix-only) -
	SCR_SIZE,		# curses/apollo/termcap -
	SETMTIME,		# utime() call -
	TRACK_WD,		# unix-style 'getwd()' -
	UID2S,			# PORTUNIX -
	USERCOPY 		# uses FOR_USER -
	USERPROT 		# uses FOR_USER -
	VCS_FILE,		# rcs (unix-only) -
	VIEW_FILE,		# curses/unix -
	WHICH,			# unix-style PATH-variable -
	WIN2FILE,		# curses -
	WREPAINT		# curses
#
C_SRC	=-
	ABSPATH.C -
	ADD2FNAME.C -
	ARGV2FILE.C -
	BEEP.C -
	BLDARG.C -
	BLIP.C -
	CATARG.C -
	CATCHALL.C -
	CMDCH.C -
	COPYBACK.C -
	CUTOFF.C -
	DENODE.C -
	DFTENV.C -
	DOALLOC.C -
	DOTCMP.C -
	DOTNAME.C -
	DUMPCHR.C -
	DUMPWIN.C -
	ERASECHAR.C -
	EXECUTE.C -
	FAILED.C -
	FILE2ARGV.C -
	FILE2MEM.C -
	FILECOPY.C -
	FOR_USER.C -
	FP2ARGV.C -
	FTYPE.C -
	FTYPE2.C -
	GETHOME.C -
	GETUSER.C -
	GID2S.C -
	ISTEXTFILE.C -
	KILLCHAR.C -
	LIST_BY_COLS.C -
	LTOSTR.C -
	MAKEARGV.C -
	MEM2FILE.C -
	MODECHAR.C -
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
	RCS_DEBUG.C -
	RCS_DIR.C -
	RCSEDIT.C -
	RCSKEYS.C -
	RCSLAST.C -
	RCSLOAD.C -
	RCSLOCKS.C -
	RCSNAME.C -
	RCSPATH.C -
	RCSPERMIT.C -
	RCSTEMP.C -
	RELPATH.C -
	RESIZEWIN.C -
	REVERT.C -
	S2GID.C -
	S2UID.C -
	SAMELEAF.C -
	SAVEWIN.C -
	SCCS_DIR.C -
	SCCSLAST.C -
	SCR_SIZE.C -
	SETMTIME.C -
	STRALLOC.C -
	STRBCMP.C -
	STRCLEAN.C -
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
	VERCMP.C -
	VIEW_FILE.C -
	WALKTREE.C -
	WHICH.C -
	WIN2FILE.C -
	WREPAINT.C
#
PTYPES_H = $(I)PTYPES.H $(I)COMMON.H
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
		- remove -vf *.LIS;* *.LOG;* TEST_*.*;*
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
ADD2FNAME.obj :		$(PTYPES_H)
ARGV2FILE.obj :		$(PTYPES_H)
BEEP.obj :		$(PTYPES_H)
CMDCH.obj :		$(PTYPES_H)	$(I)cmdch.h
COPYBACK.obj :		$(PTYPES_H)
CUTOFF.obj :		$(PTYPES_H)
DOALLOC.OBJ :		$(PTYPES_H)
DUMPWIN.obj :		$(PTYPES_H)
ERASECHAR.obj :		$(PTYPES_H)
EXECUTE.obj :		$(PTYPES_H)
FAILED.obj :		$(PTYPES_H)
FILE2ARGV.obj :		$(PTYPES_H)
FILE2MEM.obj :		$(PTYPES_H)
FILECOPY.obj :		$(PTYPES_H)
FOR_USER.obj :		$(PTYPES_H)
FP2ARGV.obj :		$(PTYPES_H)
GID2S.obj :		$(PTYPES_H)
ISTEXTFILE.obj :	$(PTYPES_H)
KILLCHAR.obj :		$(PTYPES_H)
MEM2FILE.obj :		$(PTYPES_H)
NAME2S.obj :		$(PTYPES_H)
NEWZONE.obj :		$(PTYPES_H)
PACKDATE.obj :		$(PTYPES_H)
PADEDIT.obj :		$(PTYPES_H)
PATHCMP.obj :		$(PTYPES_H)
RAWGETS.obj :		$(PTYPES_H)	$(I)cmdch.h
RAWTERM.obj :		$(PTYPES_H)
# rcsedit.obj :		$(PTYPES_H)	$(I)rcsdefs.h
# rcskeys.obj :		$(PTYPES_H)	$(I)rcsdefs.h
# rcslast.obj :		$(PTYPES_H)	$(I)rcsdefs.h
# rcsload.obj :		$(PTYPES_H)	$(I)rcsdefs.h
# rcslocks.obj :	$(PTYPES_H)	$(I)rcsdefs.h
# rcsname.obj :		$(PTYPES_H)	$(I)rcsdefs.h
# rcspath.obj :		$J/rcspath.h	; $(CC) -I$J -c $*.c
# rcspermit.obj :	$(PTYPES_H)	$(I)rcsdefs.h
RESIZWWIN.obj :		$(PTYPES_H)
SCCSLAST.obj :		$(PTYPES_H)
SETMTIME.obj :		$(PTYPES_H)
SAVEWIN.obj :		$(PTYPES_H)
TRIMPATH.obj :		$(PTYPES_H)
UID2S.obj :		$(PTYPES_H)
USERCOPY.obj :		$(PTYPES_H)
USERPROT.obj :		$(PTYPES_H)
# vcs_file.obj :	$(PTYPES_H)	$(I)rcsdefs.h
WALKTREE.OBJ :		$(PTYPES_H)
WIN2FILE.obj :		$(PTYPES_H)
#
####### (Development) ##########################################################
!
.OBJ.EXE :
		$(LINK) $(LINKFLAGS) $(MMS$TARGET_NAME),$(A)/LIB,$(C_RTL)/LIB
!
! Rules for test-programs
TEST_LTOSTR.EXE :	TEST_LTOSTR.OBJ
TEST_TXTALLOC.EXE :	TEST_TXTALLOC.OBJ
TEST_VERCMP.EXE :	TEST_VERCMP.OBJ
TEST_WALKTREE.EXE :	TEST_WALKTREE.OBJ
!
TEST_LTOSTR.OBJ :	LTOSTR.C		; $(TEST_CC)
TEST_TXTALLOC.OBJ :	TXTALLOC.C		; $(TEST_CC)
TEST_VERCMP.OBJ :	VERCMP.C		; $(TEST_CC)
TEST_WALKTREE.OBJ :	WALKTREE.C		; $(TEST_CC)
