# $Header: /users/source/archives/td_lib.vcs/src/RCS/Makefile,v 2.5 1989/05/18 11:12:40 dickey Exp $
# MAKE-file for miscellaneous library routines
#
# $Log: Makefile,v $
# Revision 2.5  1989/05/18 11:12:40  dickey
# added argv2file
#
#	Revision 2.4  89/05/11  15:32:12  dickey
#	added file2mem & mem2file modules.  added ptypes.h-dependency for
#	catchall.c; revised destroy-rule.
#	
#	Revision 2.3  89/04/27  11:22:49  dickey
#	added 'failed.c' to simplify use of this library
#	
#	Revision 2.2  89/04/20  16:09:45  dickey
#	added MMS script to controlled-sources.
#	
#	Revision 2.1  89/04/20  12:34:27  dickey
#	absorbed a bunch of library routines from MDIFF and PORTUNIX
#	
#	Revision 2.0  89/03/29  11:41:11  ste_cm
#	BASELINE Thu Apr  6 09:45:13 EDT 1989
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
	trimpath.c\
	txtalloc.c\
	uid2s.c\
	usercopy.c\
	userprot.c\
	vcs_file.c\
	vecalloc.c\
	vercmp.c\
	walktree.c\
	which.c\
	win2file.c
#
SOURCES	= Makefile descrip.mms $(CSRC)
#
OBJS	=\
	abspath.o\
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
	trimpath.o\
	txtalloc.o\
	uid2s.o\
	usercopy.o\
	userprot.o\
	vcs_file.o\
	vecalloc.o\
	vercmp.o\
	walktree.o\
	which.o\
	win2file.o
#
ALL	= $A
#
####### (Standard Productions) #################################################
all:		$(ALL)
#
clean:				; rm -f *.o *.out *.ln
clobber:	clean		; rm -f $(ALL)
destroy:
	sh -c 'for i in *;do case $$i in RCS);; *) rm -f $$i;;esac;done'
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
#
argv2file.o:	$I/ptypes.h
beep.o:		$I/ptypes.h
catchall.o:	$I/ptypes.h
cmdch.o:	$I/ptypes.h	$I/cmdch.h
copyback.o:	$I/ptypes.h
cutoff.o:	$I/ptypes.h
doalloc.o:	$I/ptypes.h
dumpwin.o:	$I/ptypes.h
erasechar.o:	$I/ptypes.h
execute.o:	$I/ptypes.h
failed.o:	$I/ptypes.h
file2argv.o:	$I/ptypes.h
file2mem.o:	$I/ptypes.h
filecopy.o:	$I/ptypes.h
for_user.o:	$I/ptypes.h
fp2argv.o:	$I/ptypes.h
gid2s.o:	$I/ptypes.h
killchar.o:	$I/ptypes.h
mem2file.o:	$I/ptypes.h
name2s.o:	$I/ptypes.h
newzone.o:	$I/ptypes.h
packdate.o:	$I/ptypes.h
padedit.o:	$I/ptypes.h
pathcmp.o:	$I/ptypes.h
rawgets.o:	$I/ptypes.h	$I/cmdch.h
rawterm.o:	$I/ptypes.h
rcsedit.o:	$I/ptypes.h	$I/rcsdefs.h
rcskeys.o:	$I/ptypes.h	$I/rcsdefs.h
rcslast.o:	$I/ptypes.h	$I/rcsdefs.h
rcsload.o:	$I/ptypes.h	$I/rcsdefs.h
rcslocks.o:	$I/ptypes.h	$I/rcsdefs.h
rcsname.o:	$I/ptypes.h	$I/rcsdefs.h
rcspath.o:	$J/rcspath.h	; $(CC) -I$J -c $*.c
rcspermit.o:	$I/ptypes.h	$I/rcsdefs.h
resizewin.o:	$I/ptypes.h
sccslast.o:	$I/ptypes.h
setmtime.o:	$I/ptypes.h
savewin.o:	$I/ptypes.h
trimpath.o:	$I/ptypes.h
uid2s.o:	$I/ptypes.h
usercopy.o:	$I/ptypes.h
userprot.o:	$I/ptypes.h
vcs_file.o:	$I/ptypes.h	$I/rcsdefs.h
walktree.o:	$I/ptypes.h
win2file.o:	$I/ptypes.h
#
####### (Development) ##########################################################
$(SOURCES):			; $(GET) $@
lint.out:	$(CSRC)		; $(DATE); tdlint $(LINTOPT) $? >>$@
lincnt.out:	$(CSRC)		; $(DATE); lincnt $(CSRC) >>$@
