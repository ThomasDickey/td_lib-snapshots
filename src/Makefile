# $Id: Makefile,v 11.4 1992/08/06 11:20:21 dickey Exp $
# MAKE-file for miscellaneous library routines

####### (Environment) ##########################################################
include ../../common.mk

####### (Command-line Options) #################################################


####### (Standard Lists) #######################################################
Z	= $L/$(THAT).a

CSRC	=\
	abspath.c\
	access_mode.c\
	add2fname.c\
	argv2file.c\
	beep.c\
	bldarg.c\
	blip.c\
	catarg.c\
	catchall.c\
	cmdch.c\
	copyback.c\
	count_ss.c\
	cutoff.c\
	denode.c\
	dftenv.c\
	diffload.c\
	doalloc.c\
	dotcmp.c\
	dotname.c\
	dumpchr.c\
	dumpwin.c\
	dyn_append.c\
	dyn_append_c.c\
	dyn_catarg.c\
	dyn_copy.c\
	dyn_string.c\
	erasechar.c\
	execute.c\
	failed.c\
	field_of.c\
	file2argv.c\
	file2mem.c\
	filecopy.c\
	filesize.c\
	for_admin.c\
	for_user.c\
	fp2argv.c\
	ftype.c\
	ftype2.c\
	gethome.c\
	getuser.c\
	gid2s.c\
	interactive.c\
	is_subpath.c\
	istextfile.c\
	killchar.c\
	list_by_cols.c\
	logch.c\
	ltostr.c\
	makeargv.c\
	mem2file.c\
	modechar.c\
	name2s.c\
	newzone.c\
	next_version.c\
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
	rcsargpair.c\
	rcsedit.c\
	rcskeys.c\
	rcslast.c\
	rcsload.c\
	rcslocks.c\
	rcsname.c\
	rcspath.c\
	rcspermit.c\
	rcssymbols.c\
	rcstemp.c\
	rcstime.c\
	relpath.c\
	resizewin.c\
	revert.c\
	samebranch.c\
	sameleaf.c\
	savewin.c\
	sccs_dir.c\
	sccslast.c\
	sccsname.c\
	scr_size.c\
	setmtime.c\
	shoarg.c\
	show_uids.c\
	ss_field.c\
	stat_dir.c\
	stat_file.c\
	stralloc.c\
	strbcmp.c\
	strclean.c\
	strcount.c\
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
	vecedit.c\
	veclen.c\
	vercmp.c\
	view_file.c\
	walktree.c\
	which.c\
	win2file.c\
	wrepaint.c

SOURCES	= Makefile descrip.mms $(CSRC)

OBJS	=\
	$Z(abspath.o)\
	$Z(access_mode.o)\
	$Z(add2fname.o)\
	$Z(argv2file.o)\
	$Z(beep.o)\
	$Z(bldarg.o)\
	$Z(blip.o)\
	$Z(catarg.o)\
	$Z(catchall.o)\
	$Z(cmdch.o)\
	$Z(copyback.o)\
	$Z(count_ss.o)\
	$Z(cutoff.o)\
	$Z(denode.o)\
	$Z(dftenv.o)\
	$Z(diffload.o)\
	$Z(doalloc.o)\
	$Z(dotcmp.o)\
	$Z(dotname.o)\
	$Z(dumpchr.o)\
	$Z(dumpwin.o)\
	$Z(dyn_append.o)\
	$Z(dyn_append_c.o)\
	$Z(dyn_catarg.o)\
	$Z(dyn_copy.o)\
	$Z(dyn_string.o)\
	$Z(erasechar.o)\
	$Z(execute.o)\
	$Z(failed.o)\
	$Z(field_of.o)\
	$Z(file2argv.o)\
	$Z(file2mem.o)\
	$Z(filecopy.o)\
	$Z(filesize.o)\
	$Z(for_admin.o)\
	$Z(for_user.o)\
	$Z(fp2argv.o)\
	$Z(ftype.o)\
	$Z(ftype2.o)\
	$Z(gethome.o)\
	$Z(getuser.o)\
	$Z(gid2s.o)\
	$Z(interactive.o)\
	$Z(is_subpath.o)\
	$Z(istextfile.o)\
	$Z(killchar.o)\
	$Z(list_by_cols.o)\
	$Z(logch.o)\
	$Z(ltostr.o)\
	$Z(makeargv.o)\
	$Z(mem2file.o)\
	$Z(modechar.o)\
	$Z(name2s.o)\
	$Z(newzone.o)\
	$Z(next_version.o)\
	$Z(packdate.o)\
	$Z(padedit.o)\
	$Z(pathcat.o)\
	$Z(pathcmp.o)\
	$Z(pathhead.o)\
	$Z(pathleaf.o)\
	$Z(rawgets.o)\
	$Z(rawterm.o)\
	$Z(rcs_debug.o)\
	$Z(rcs_dir.o)\
	$Z(rcsargpair.o)\
	$Z(rcsedit.o)\
	$Z(rcskeys.o)\
	$Z(rcslast.o)\
	$Z(rcsload.o)\
	$Z(rcslocks.o)\
	$Z(rcsname.o)\
	$Z(rcspath.o)\
	$Z(rcspermit.o)\
	$Z(rcssymbols.o)\
	$Z(rcstemp.o)\
	$Z(rcstime.o)\
	$Z(relpath.o)\
	$Z(resizewin.o)\
	$Z(revert.o)\
	$Z(samebranch.o)\
	$Z(sameleaf.o)\
	$Z(savewin.o)\
	$Z(sccs_dir.o)\
	$Z(sccslast.o)\
	$Z(sccsname.o)\
	$Z(scr_size.o)\
	$Z(setmtime.o)\
	$Z(shoarg.o)\
	$Z(show_uids.o)\
	$Z(ss_field.o)\
	$Z(stat_dir.o)\
	$Z(stat_file.o)\
	$Z(stralloc.o)\
	$Z(strbcmp.o)\
	$Z(strclean.o)\
	$Z(strcount.o)\
	$Z(strtrim.o)\
	$Z(strucmp.o)\
	$Z(strucpy.o)\
	$Z(strwcmp.o)\
	$Z(s2gid.o)\
	$Z(s2uid.o)\
	$Z(track_wd.o)\
	$Z(trimpath.o)\
	$Z(txtalloc.o)\
	$Z(uid2s.o)\
	$Z(usercopy.o)\
	$Z(userprot.o)\
	$Z(vcs_file.o)\
	$Z(vecalloc.o)\
	$Z(vecedit.o)\
	$Z(veclen.o)\
	$Z(vercmp.o)\
	$Z(view_file.o)\
	$Z(walktree.o)\
	$Z(which.o)\
	$Z(win2file.o)\
	$Z(wrepaint.o)

ALL	= $Z

####### (Standard Productions) #################################################
all\
install:	sources $(ALL)

clean:				; rm -f $(CLEAN)
clobber:	clean		; rm -f $(ALL)
destroy:			; $(DESTROY)
sources:	$(SOURCES)

####### (Details of Productions) ###############################################
$Z:	$(OBJS)
	ranlib $Z
#
.PRECIOUS: $Z
#
$(OBJS):	$(PTYPES_H)

$Z(cmdch.o)\
$Z(logch.o)\
$Z(rawgets.o):	$I/cmdch.h

$Z(dyn_append.o)\
$Z(dyn_append_c.o)\
$Z(dyn_catarg.o)\
$Z(dyn_copy.o)\
$Z(dyn_string.o): $I/dyn_string.h

$Z(rcsargpair.o)\
$Z(rcsedit.o)\
$Z(rcskeys.o)\
$Z(rcslast.o)\
$Z(rcsload.o)\
$Z(rcslocks.o)\
$Z(rcsname.o)\
$Z(rcspermit.o)\
$Z(samebranch.o)\
$Z(vcs_file.o):	$(RCSDEFS_H)
#
$Z(rcspath.o):	$J/rcspath.h
	$(CC) $(CPP_OPTS) -I$J $(CFLAGS) -c $*.c
	@ar r $Z $*.o
	@rm   -f $*.o
#
$Z(sccslast.o)\
$Z(sccsname.o):	$(SCCSDEFS_H)
#
$Z(count_ss.o)\
$Z(field_of.o)\
$Z(ss_field.o):	$I/spreadsheet.h $I/dyn_string.h
#
####### (Development) ##########################################################
$(SOURCES):			; $(GET) $@
lint.out:	$(CSRC)		; $(DATE); $(LINT_EACH)
lincnt.out:	$(CSRC)		; $(DATE); lincnt $(CSRC) >>$@
