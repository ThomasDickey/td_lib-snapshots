# $Id: Makefile,v 9.0 1991/06/06 16:57:46 ste_cm Rel $
# MAKE-file for miscellaneous library routines
#
# $Log: Makefile,v $
# Revision 9.0  1991/06/06 16:57:46  ste_cm
# BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
#
#	Revision 8.3  91/06/06  16:57:46  dickey
#	make ".i" rule work on apollo sr10.2
#	
#	Revision 8.2  91/06/05  16:56:18  dickey
#	corrected typo in ".i" rule
#	
#	Revision 8.1  91/06/05  11:06:19  dickey
#	added rule for ".i"
#	
#	Revision 8.0  90/05/08  13:28:12  ste_cm
#	BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
#	
####### (Development) ##########################################################
GET	= checkout
#
####### (Command-line Options) #################################################
I	= ../interface
J	= ../../../interface
INCLUDES= -I$I
CFLAGS  =
DATE	= echo '****'`date` >> $@
LINTOPT	= $(DEFINES) $(INCLUDES) -I$J -ltd -lcurses -lapollo
LIBS	= -lcurses -ltermcap
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
	sccsname.c\
	scr_size.c\
	setmtime.c\
	stralloc.c\
	strbcmp.c\
	strclean.c\
	strtrim.c\
	strucmp.c\
	strucpy.c\
	strwcmp.c\
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
	sccsname.o\
	scr_size.o\
	setmtime.o\
	stralloc.o\
	strbcmp.o\
	strclean.o\
	strtrim.o\
	strucmp.o\
	strucpy.o\
	strwcmp.o\
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
clean:				; rm -f core *.o *.out *.ln *.lint *.tst
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
.PRECIOUS: $A
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
sccsname.o:	$I/sccsdefs.h
vcs_file.o:	$I/rcsdefs.h
#
####### (Development) ##########################################################
$(SOURCES):			; $(GET) $@
lint.out:	$(CSRC)		; $(DATE); tdlint $(LINTOPT) $? >>$@
lint2.out:	$(CSRC)
	$(DATE)
	sh -c 'for i in $?;do echo $$i: | tee -a $@;\
		tdlint $(LINTOPT) $$i >>$@;done'
lincnt.out:	$(CSRC)		; $(DATE); lincnt $(CSRC) >>$@
#
.SUFFIXES: .lint .tst .i
.c.lint:
	tdlint $(LINTOPT) $< >$@
.c.tst:
	$(CC) -o $@ -DTEST $(CFLAGS) $(INCLUDES) $(DEFINES) $< $A $(LIBS)
	rm -f $*.o
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c $<
.c.i:
	$(CC) -E -C $(INCLUDES) $(DEFINES) $< >$@
