# $Id: Makefile,v 5.0 1989/10/26 08:54:17 ste_cm Rel $
# MAKE-file for miscellaneous library routines
#
# $Log: Makefile,v $
# Revision 5.0  1989/10/26 08:54:17  ste_cm
# BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
#
#	Revision 4.7  89/10/26  08:54:17  dickey
#	added TRACK_WD module
#	
#	Revision 4.6  89/10/24  11:48:43  dickey
#	added module 'istextfile.c'
#	
#	Revision 4.5  89/10/04  12:13:16  dickey
#	made "ptypes.h" and "common.h" dependencies for all objects (most
#	needed it anyway).
#	
#	Revision 4.4  89/09/07  15:06:47  dickey
#	added module "relpath.c"
#	
#	Revision 4.3  89/09/06  15:22:34  dickey
#	use "ptypes.h" in 'abspath.c'; added missing ranlib to 'archive' rule
#	
#	Revision 4.2  89/08/25  09:56:12  dickey
#	added ptypes.h dependency to which.o
#	
#	Revision 4.1  89/08/24  16:50:43  dickey
#	added 'wrepaint.c'
#	
#	Revision 4.0  89/08/23  13:49:43  ste_cm
#	BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
#	
####### (Development) ##########################################################
GET	= checkout
#
####### (Command-line Options) #################################################
I	= ../interface
J	= ../../../interface
INCLUDES= -I$I
CFLAGS  = $(DEFINES) $(INCLUDES) -O
DATE	= echo '****'`date` >> $@
LINTOPT	= $(DEFINES) $(INCLUDES) -I$J -ltd -lcurses -lapollo
#
####### (Standard Lists) #######################################################
A	= ../lib/lib.a
CSRC	=\
	abspath.c\
	add2fname.c\
	argv2file.c\
	beep.c\
	bldarg.c\
	blip.c\
	catarg.c\
	catchall.c\
	cmdch.c\
	copyback.c\
	cutoff.c\
	denode.c\
	dftenv.c\
	doalloc.c\
	dotcmp.c\
	dotname.c\
	dumpchr.c\
	dumpwin.c\
	erasechar.c\
	execute.c\
	failed.c\
	file2argv.c\
	file2mem.c\
	filecopy.c\
	for_user.c\
	fp2argv.c\
	ftype.c\
	ftype2.c\
	gethome.c\
	getuser.c\
	gid2s.c\
	istextfile.c\
	killchar.c\
	list_by_cols.c\
	ltostr.c\
	makeargv.c\
	mem2file.c\
	modechar.c\
	name2s.c\
	newzone.c\
	packdate.c\
	padedit.c\
	pathcat.c\
	pathcmp.c\
	pathhead.c\
	pathleaf.c\
	rawgets.c\
	rawterm.c\
	rcs_debug.c\
	rcs_dir.c\
	rcsedit.c\
	rcskeys.c\
	rcslast.c\
	rcsload.c\
	rcslocks.c\
	rcsname.c\
	rcspath.c\
	rcspermit.c\
	rcstemp.c\
	relpath.c\
	resizewin.c\
	revert.c\
	sameleaf.c\
	savewin.c\
	sccs_dir.c\
	sccslast.c\
	scr_size.c\
	setmtime.c\
	stralloc.c\
	strbcmp.c\
	strclean.c\
	strtrim.c\
	strucmp.c\
	strucpy.c\
	s2gid.c\
	s2uid.c\
	track_wd.c\
	trimpath.c\
	txtalloc.c\
	uid2s.c\
	usercopy.c\
	userprot.c\
	vcs_file.c\
	vecalloc.c\
	vercmp.c\
	view_file.c\
	walktree.c\
	which.c\
	win2file.c\
	wrepaint.c
#
SOURCES	= Makefile descrip.mms $(CSRC)
#
OBJS	=\
	abspath.o\
	add2fname.o\
	argv2file.o\
	beep.o\
	bldarg.o\
	blip.o\
	catarg.o\
	catchall.o\
	cmdch.o\
	copyback.o\
	cutoff.o\
	denode.o\
	dftenv.o\
	doalloc.o\
	dotcmp.o\
	dotname.o\
	dumpchr.o\
	dumpwin.o\
	erasechar.o\
	execute.o\
	failed.o\
	file2argv.o\
	file2mem.o\
	filecopy.o\
	for_user.o\
	fp2argv.o\
	ftype.o\
	ftype2.o\
	gethome.o\
	getuser.o\
	gid2s.o\
	istextfile.o\
	killchar.o\
	list_by_cols.o\
	ltostr.o\
	makeargv.o\
	mem2file.o\
	modechar.o\
	name2s.o\
	newzone.o\
	packdate.o\
	padedit.o\
	pathcat.o\
	pathcmp.o\
	pathhead.o\
	pathleaf.o\
	rawgets.o\
	rawterm.o\
	rcs_debug.o\
	rcs_dir.o\
	rcsedit.o\
	rcskeys.o\
	rcslast.o\
	rcsload.o\
	rcslocks.o\
	rcsname.o\
	rcspath.o\
	rcspermit.o\
	rcstemp.o\
	relpath.o\
	resizewin.o\
	revert.o\
	sameleaf.o\
	savewin.o\
	sccs_dir.o\
	sccslast.o\
	scr_size.o\
	setmtime.o\
	stralloc.o\
	strbcmp.o\
	strclean.o\
	strtrim.o\
	strucmp.o\
	strucpy.o\
	s2gid.o\
	s2uid.o\
	track_wd.o\
	trimpath.o\
	txtalloc.o\
	uid2s.o\
	usercopy.o\
	userprot.o\
	vcs_file.o\
	vecalloc.o\
	vercmp.o\
	view_file.o\
	walktree.o\
	which.o\
	win2file.o\
	wrepaint.o
#
ALL	= $A
#
####### (Standard Productions) #################################################
all:		sources $(ALL)
#
clean:				; rm -f *.o *.out *.ln
clobber:	clean		; rm -f $(ALL)
destroy:
	sh -c 'for i in *;do case $$i in RCS);; *) rm -f $$i;;esac;done;exit 0'
sources:	$(SOURCES)
#
install: all
#
####### (Details of Productions) ###############################################
$A:	$(OBJS)
	ar rv $A $?
	ranlib $A
#
archive: $(OBJS)
	rm -f $A
	ar rv $A $(OBJS)
	ranlib $A
#
$(OBJS):	$I/ptypes.h	$I/common.h
cmdch.o:	$I/cmdch.h
rawgets.o:	$I/cmdch.h
rcsedit.o:	$I/rcsdefs.h
rcskeys.o:	$I/rcsdefs.h
rcslast.o:	$I/rcsdefs.h
rcsload.o:	$I/rcsdefs.h
rcslocks.o:	$I/rcsdefs.h
rcsname.o:	$I/rcsdefs.h
rcspath.o:	$J/rcspath.h	; $(CC) -I$J -c $*.c
rcspermit.o:	$I/rcsdefs.h
vcs_file.o:	$I/rcsdefs.h
#
####### (Development) ##########################################################
$(SOURCES):			; $(GET) $@
lint.out:	$(CSRC)		; $(DATE); tdlint $(LINTOPT) $? >>$@
lincnt.out:	$(CSRC)		; $(DATE); lincnt $(CSRC) >>$@
