# $Header: /users/source/archives/td_lib.vcs/src/RCS/descrip.mms,v 2.4 1989/05/17 10:02:35 dickey Exp $
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
	BEEP, -
	BLDARG, -
	BLIP, -
	CATARG, -
	CATCHALL, -
	COPYBACK, -
	DOALLOC, -
	DOTCMP, -
	DOTNAME, -
	DUMPCHR, -
	FAILED, -
	FILE2ARGV, -
	FILE2MEM, -
	FP2ARGV, -
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
	FTYPE, 			# unix-style suffix -
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
	RESIZEWIN,		# curses (apollo) -
	REVERT,			# unix setuid -
	S2GID,			# unix-passwd -
	S2UID,			# unix-passwd -
	SAVEWIN,		# curses -
	SCCS_DIR,		# sccs (unix-only) -
	SCCSLAST,		# sccs (unix-only) -
	SCR_SIZE,		# curses/apollo/termcap -
	SETMTIME,		# utime() call -
	UID2S,			# PORTUNIX -
	USERCOPY 		# uses FOR_USER -
	USERPROT 		# uses FOR_USER -
	VCS_FILE,		# rcs (unix-only) -
	WHICH,			# unix-style PATH-variable -
	WIN2FILE		# curses
#
C_SRC	=-
	ABSPATH.C -
	BEEP.C -
	BLDARG.C -
	BLIP.C -
	CATARG.C -
	CATCHALL.C -
	CMDCH.C -
	COPYBACK.C -
	CUTOFF.C -
	DENODE.C -
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
	TRIMPATH.C -
	TXTALLOC.C -
	UID2S.C -
	USERCOPY.C -
	USERPROT.C -
	VCS_FILE.C -
	VECALLOC.C -
	VERCMP.C -
	WALKTREE.C -
	WHICH.C -
	WIN2FILE.C
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
beep.obj :		$(PTYPES_H)
cmdch.obj :		$(PTYPES_H)	$(I)cmdch.h
copyback.obj :		$(PTYPES_H)
cutoff.obj :		$(PTYPES_H)
DOALLOC.OBJ :		$(PTYPES_H)
dumpwin.obj :		$(PTYPES_H)
erasechar.obj :		$(PTYPES_H)
execute.obj :		$(PTYPES_H)
failed.obj :		$(PTYPES_H)
file2argv.obj :		$(PTYPES_H)
file2mem.obj :		$(PTYPES_H)
filecopy.obj :		$(PTYPES_H)
for_user.obj :		$(PTYPES_H)
fp2argv.obj :		$(PTYPES_H)
gid2s.obj :		$(PTYPES_H)
killchar.obj :		$(PTYPES_H)
mem2file.obj :		$(PTYPES_H)
name2s.obj :		$(PTYPES_H)
newzone.obj :		$(PTYPES_H)
packdate.obj :		$(PTYPES_H)
padedit.obj :		$(PTYPES_H)
pathcmp.obj :		$(PTYPES_H)
rawgets.obj :		$(PTYPES_H)	$(I)cmdch.h
rawterm.obj :		$(PTYPES_H)
# rcsedit.obj :		$(PTYPES_H)	$(I)rcsdefs.h
# rcskeys.obj :		$(PTYPES_H)	$(I)rcsdefs.h
# rcslast.obj :		$(PTYPES_H)	$(I)rcsdefs.h
# rcsload.obj :		$(PTYPES_H)	$(I)rcsdefs.h
# rcslocks.obj :	$(PTYPES_H)	$(I)rcsdefs.h
# rcsname.obj :		$(PTYPES_H)	$(I)rcsdefs.h
# rcspath.obj :		$J/rcspath.h	; $(CC) -I$J -c $*.c
# rcspermit.obj :	$(PTYPES_H)	$(I)rcsdefs.h
resizewin.obj :		$(PTYPES_H)
sccslast.obj :		$(PTYPES_H)
setmtime.obj :		$(PTYPES_H)
savewin.obj :		$(PTYPES_H)
trimpath.obj :		$(PTYPES_H)
uid2s.obj :		$(PTYPES_H)
usercopy.obj :		$(PTYPES_H)
userprot.obj :		$(PTYPES_H)
# vcs_file.obj :	$(PTYPES_H)	$(I)rcsdefs.h
WALKTREE.OBJ :		$(PTYPES_H)
win2file.obj :		$(PTYPES_H)
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
