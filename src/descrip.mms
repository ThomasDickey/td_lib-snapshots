# $Id: descrip.mms,v 10.9 1992/02/07 15:03:55 dickey Exp $
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
	ACCESS_MODE, -
	ADD2FNAME, -
	ARGV2FILE, -
	BEEP, -
	BLDARG, -
	BLIP, -
	CATARG, -
	CATCHALL, -
	COPYBACK, -
	COUNT_SS, -
	DFTENV, -
	DOALLOC, -
	DOTCMP, -
	DOTNAME, -
	DUMPCHR, -
	DYN_APPEND, -
	DYN_CATARG, -
	DYN_STRING, -
	EXECUTE, -
	FAILED, -
	FIELD_OF, -
	FILE2ARGV, -
	FILE2MEM, -
	FILESIZE, -
	FP2ARGV, -
	FTYPE, -
	ISTEXTFILE, -
	LIST_BY_COLS, -
	LTOSTR, -
	MAKEARGV, -
	MEM2FILE, -
	MODECHAR, -
	NEXT_VERSION, -
	PATHCAT, -
	PATHLEAF, -
	SAMELEAF, -
	SHOARG, -
	SHOW_UIDS, -
	SS_FIELD, -
	STAT_DIR, -
	STAT_FILE, -
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
	VECEDIT, -
	VECLEN, -
	VERCMP, -
	WALKTREE
UNUSED_MODULES = -
	ABSPATH, 		# unix-style 'getwd()' -
	CMDCH, 			# curses -
	CUTOFF, 		# uses PACKDATE -
	DENODE, 		# only for ABSPATH -
	DUMPWIN, 		# curses -
	ERASECHAR, 		# curses -
	FILECOPY, 		# needs unix system-I/O -
	FOR_ADMIN, 		# only for setuid programs -
	FOR_USER, 		# only for setuid programs -
	FTYPE2, 		# unix-style suffix -
	GETHOME, 		# unix-passwd -
	GETUSER, 		# unix-passwd -
	GID2S, 			# PORTUNIX -
	INTERACTIVE,		# unix filesystem
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
	RCSARGPAIR,		# rcs (unix-only) -
	RCSEDIT,		# rcs (unix-only) -
	RCSKEYS,		# rcs (unix-only) -
	RCSLAST,		# rcs (unix-only) -
	RCSLOAD,		# rcs (unix-only) -
	RCSLOCKS,		# rcs (unix-only) -
	RCSNAME,		# rcs (unix-only) -
	RCSPATH,		# rcs (unix-only) -
	RCSPERMIT,		# rcs (unix-only) -
	RCSSYMBOLS,		# rcs (unix-only) -
	RCSTEMP,		# rcs (unix-only) -
	RCSTIME,		# rcs (unix-only) -
	RELPATH,		# unix-style 'getwd()' -
	RESIZEWIN,		# curses (apollo) -
	REVERT,			# unix setuid -
	S2GID,			# unix-passwd -
	S2UID,			# unix-passwd -
	SAVEWIN,		# curses -
	SCCS_DIR,		# sccs (unix-only) -
	SCCSLAST,		# sccs (unix-only) -
	SCCSNAME,		# sccs (unix-only) -
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
	ACCESS_MODE.C -
	ADD2FNAME.C -
	ARGV2FILE.C -
	BEEP.C -
	BLDARG.C -
	BLIP.C -
	CATARG.C -
	CATCHALL.C -
	CMDCH.C -
	COPYBACK.C -
	COUNT_SS.C -
	CUTOFF.C -
	DENODE.C -
	DFTENV.C -
	DOALLOC.C -
	DOTCMP.C -
	DOTNAME.C -
	DUMPCHR.C -
	DUMPWIN.C -
	DYN_APPEND.C -
	DYN_CATARG.C -
	DYN_STRING.C -
	ERASECHAR.C -
	EXECUTE.C -
	FAILED.C -
	FIELD_OF.C -
	FILE2ARGV.C -
	FILE2MEM.C -
	FILECOPY.C -
	FILESIZE.C -
	FOR_ADMIN.C -
	FOR_USER.C -
	FP2ARGV.C -
	FTYPE.C -
	FTYPE2.C -
	GETHOME.C -
	GETUSER.C -
	GID2S.C -
	INTERACTIVE.C -
	ISTEXTFILE.C -
	KILLCHAR.C -
	LIST_BY_COLS.C -
	LTOSTR.C -
	MAKEARGV.C -
	MEM2FILE.C -
	MODECHAR.C -
	NEXT_VERSION.C -
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
	RCSARGPAIR.C -
	RCSEDIT.C -
	RCSKEYS.C -
	RCSLAST.C -
	RCSLOAD.C -
	RCSLOCKS.C -
	RCSNAME.C -
	RCSPATH.C -
	RCSPERMIT.C -
	RCSSYMBOLS.C -
	RCSTEMP.C -
	RCSTIME.C -
	RELPATH.C -
	RESIZEWIN.C -
	REVERT.C -
	S2GID.C -
	S2UID.C -
	SAMELEAF.C -
	SAVEWIN.C -
	SCCS_DIR.C -
	SCCSLAST.C -
	SCCSNAME.C -
	SCR_SIZE.C -
	SETMTIME.C -
	SHOARG.C -
	SHOW_UIDS.C -
	SS_FIELD.C -
	STAT_DIR.C -
	STAT_FILE.C -
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
	VECEDIT.C -
	VECLEN.C -
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
ACCESS_MODE.obj :	$(PTYPES_H)
ADD2FNAME.obj :		$(PTYPES_H)
ARGV2FILE.obj :		$(PTYPES_H)
BEEP.obj :		$(PTYPES_H)
CATARG.obj :		$(PTYPES_H)
CATCHALL.obj :		$(PTYPES_H)
CMDCH.obj :		$(PTYPES_H)	$(I)cmdch.h
COPYBACK.obj :		$(PTYPES_H)
CUTOFF.obj :		$(PTYPES_H)
DOALLOC.OBJ :		$(PTYPES_H)
DUMPWIN.obj :		$(PTYPES_H)
DYN_APPEND.obj :	$(PTYPES_H)
DYN_CATARG.obj :	$(PTYPES_H)
DYN_STRING.obj :	$(PTYPES_H)
ERASECHAR.obj :		$(PTYPES_H)
EXECUTE.obj :		$(PTYPES_H)
FAILED.obj :		$(PTYPES_H)
FILE2ARGV.obj :		$(PTYPES_H)
FILE2MEM.obj :		$(PTYPES_H)
FILECOPY.obj :		$(PTYPES_H)
FILESIZE.obj :		$(PTYPES_H)
FOR_ADMIN.obj :		$(PTYPES_H)
FOR_USER.obj :		$(PTYPES_H)
FP2ARGV.obj :		$(PTYPES_H)
FTYPE.obj :		$(PTYPES_H)
FTYPE2.obj :		$(PTYPES_H)
GID2S.obj :		$(PTYPES_H)
ISTEXTFILE.obj :	$(PTYPES_H)
KILLCHAR.obj :		$(PTYPES_H)
LIST_BY_COLS.obj :	$(PTYPES_H)
MEM2FILE.obj :		$(PTYPES_H)
NAME2S.obj :		$(PTYPES_H)
NEWZONE.obj :		$(PTYPES_H)
PACKDATE.obj :		$(PTYPES_H)
PADEDIT.obj :		$(PTYPES_H)
PATHCAT.obj :		$(PTYPES_H)
PATHCMP.obj :		$(PTYPES_H)
PATHLEAF.obj :		$(PTYPES_H)
RAWGETS.obj :		$(PTYPES_H)	$(I)cmdch.h
RAWTERM.obj :		$(PTYPES_H)
# rcsargpair.obj :	$(PTYPES_H)	$(I)rcsdefs.h
# rcsedit.obj :		$(PTYPES_H)	$(I)rcsdefs.h
# rcskeys.obj :		$(PTYPES_H)	$(I)rcsdefs.h
# rcslast.obj :		$(PTYPES_H)	$(I)rcsdefs.h
# rcsload.obj :		$(PTYPES_H)	$(I)rcsdefs.h
# rcslocks.obj :	$(PTYPES_H)	$(I)rcsdefs.h
# rcsname.obj :		$(PTYPES_H)	$(I)rcsdefs.h
# rcspath.obj :		$J/rcspath.h	; $(CC) -I$J -c $*.c
# rcspermit.obj :	$(PTYPES_H)	$(I)rcsdefs.h
RESIZWWIN.obj :		$(PTYPES_H)
SAMELEAF.obj :		$(PTYPES_H)
SAVEWIN.obj :		$(PTYPES_H)
SCCSLAST.obj :		$(PTYPES_H)
SETMTIME.obj :		$(PTYPES_H)
SHOARG.obj :		$(PTYPES_H)
SHOW_UIDS.obj :		$(PTYPES_H)
STAT_DIR.obj :		$(PTYPES_H)
STAT_FILE.obj :		$(PTYPES_H)
STRALLOC.obj :		$(PTYPES_H)
STRCLEAN.obj :		$(PTYPES_H)
STRUCPY.obj :		$(PTYPES_H)
STRWCMP.obj :		$(PTYPES_H)
TRIMPATH.obj :		$(PTYPES_H)
TXTALLOC.obj :		$(PTYPES_H)
UID2S.obj :		$(PTYPES_H)
USERCOPY.obj :		$(PTYPES_H)
USERPROT.obj :		$(PTYPES_H)
# vcs_file.obj :	$(PTYPES_H)	$(I)rcsdefs.h
WALKTREE.OBJ :		$(PTYPES_H)
WIN2FILE.obj :		$(PTYPES_H)
#
COUNT_SS.obj :		$(PTYPES_H)	$(I)spreadsheet.h
FIELD_OF.obj :		$(PTYPES_H)	$(I)spreadsheet.h
SS_FIELD.obj :		$(PTYPES_H)	$(I)spreadsheet.h
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
