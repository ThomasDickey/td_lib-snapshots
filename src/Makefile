# $Id: Makefile,v 12.5 1994/05/30 18:35:32 tom Exp $
# MAKE-file for miscellaneous library routines

####### (Environment) ##########################################################
TOP	= ../..
include ../support/td_lib.mk
LINTLIBS = -ltd -lcurses -ltermcap -lapollo

RCS_PATH= `which rcs | sed -e s+/rcs$$+/+`

####### (Standard Lists) #######################################################
Z	= $L/$(TD_LIB).a

CSRC	=\
	abspath.c\
	acc_mode.c\
	add2fnam.c\
	arg2file.c\
	beep.c\
	bldarg.c\
	blip.c\
	catarg.c\
	catchall.c\
	cmdch.c\
	cmpqsort.c\
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
	dyn_app.c\
	dyn_appc.c\
	dyn_cata.c\
	dyn_copy.c\
	dyn_str.c\
	erasechr.c\
	execute.c\
	failed.c\
	field_of.c\
	file2arg.c\
	file2mem.c\
	filecopy.c\
	filesize.c\
	fleaf.c\
	for_admn.c\
	for_user.c\
	fp2argv.c\
	ftype.c\
	ftype2.c\
	gethome.c\
	getuser.c\
	gid2s.c\
	interact.c\
	is_spath.c\
	istextfl.c\
	killchar.c\
	lsbycols.c\
	logch.c\
	ltostr.c\
	m2comp.c\
	makeargv.c\
	mem2file.c\
	modechar.c\
	name2s.c\
	newzone.c\
	next_ver.c\
	packdate.c\
	padedit.c\
	pathcat.c\
	pathcmp.c\
	pathhead.c\
	pathleaf.c\
	rawgets.c\
	rawterm.c\
	rcsdebug.c\
	rcs_dir.c\
	rcsargpr.c\
	rcsedit.c\
	rcskeys.c\
	rcslast.c\
	rcsload.c\
	rcslocks.c\
	rcsname.c\
	rcspath.c\
	rcsperm.c\
	rcssymbs.c\
	rcstemp.c\
	rcstime.c\
	relpath.c\
	resizwin.c\
	revert.c\
	samebr.c\
	sameleaf.c\
	savesuid.c\
	savewin.c\
	scomp.c\
	sccs_dir.c\
	sccsdbug.c\
	sccslast.c\
	sccsname.c\
	scr_size.c\
	setmtime.c\
	shoarg.c\
	showuids.c\
	ss_field.c\
	stat_dir.c\
	stat_fil.c\
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
	viewfile.c\
	walkback.c\
	walktree.c\
	which.c\
	win2file.c\
	wrepaint.c

SOURCES	= Makefile descrip.mms $(CSRC)

OBJS	=\
	$Z(abspath.o)\
	$Z(acc_mode.o)\
	$Z(add2fnam.o)\
	$Z(arg2file.o)\
	$Z(beep.o)\
	$Z(bldarg.o)\
	$Z(blip.o)\
	$Z(catarg.o)\
	$Z(catchall.o)\
	$Z(cmdch.o)\
	$Z(cmpqsort.o)\
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
	$Z(dyn_app.o)\
	$Z(dyn_appc.o)\
	$Z(dyn_cata.o)\
	$Z(dyn_copy.o)\
	$Z(dyn_str.o)\
	$Z(erasechr.o)\
	$Z(execute.o)\
	$Z(failed.o)\
	$Z(field_of.o)\
	$Z(file2arg.o)\
	$Z(file2mem.o)\
	$Z(filecopy.o)\
	$Z(filesize.o)\
	$Z(fleaf.o)\
	$Z(for_admn.o)\
	$Z(for_user.o)\
	$Z(fp2argv.o)\
	$Z(ftype.o)\
	$Z(ftype2.o)\
	$Z(gethome.o)\
	$Z(getuser.o)\
	$Z(gid2s.o)\
	$Z(interact.o)\
	$Z(is_spath.o)\
	$Z(istextfl.o)\
	$Z(killchar.o)\
	$Z(lsbycols.o)\
	$Z(logch.o)\
	$Z(ltostr.o)\
	$Z(m2comp.o)\
	$Z(makeargv.o)\
	$Z(mem2file.o)\
	$Z(modechar.o)\
	$Z(name2s.o)\
	$Z(newzone.o)\
	$Z(next_ver.o)\
	$Z(packdate.o)\
	$Z(padedit.o)\
	$Z(pathcat.o)\
	$Z(pathcmp.o)\
	$Z(pathhead.o)\
	$Z(pathleaf.o)\
	$Z(rawgets.o)\
	$Z(rawterm.o)\
	$Z(rcsdebug.o)\
	$Z(rcs_dir.o)\
	$Z(rcsargpr.o)\
	$Z(rcsedit.o)\
	$Z(rcskeys.o)\
	$Z(rcslast.o)\
	$Z(rcsload.o)\
	$Z(rcslocks.o)\
	$Z(rcsname.o)\
	$Z(rcspath.o)\
	$Z(rcsperm.o)\
	$Z(rcssymbs.o)\
	$Z(rcstemp.o)\
	$Z(rcstime.o)\
	$Z(relpath.o)\
	$Z(resizwin.o)\
	$Z(revert.o)\
	$Z(samebr.o)\
	$Z(sameleaf.o)\
	$Z(savesuid.o)\
	$Z(savewin.o)\
	$Z(sccs_dir.o)\
	$Z(sccsdbug.o)\
	$Z(sccslast.o)\
	$Z(sccsname.o)\
	$Z(scomp.o)\
	$Z(scr_size.o)\
	$Z(setmtime.o)\
	$Z(shoarg.o)\
	$Z(showuids.o)\
	$Z(ss_field.o)\
	$Z(stat_dir.o)\
	$Z(stat_fil.o)\
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
	$Z(viewfile.o)\
	$Z(walkback.o)\
	$Z(walktree.o)\
	$Z(which.o)\
	$Z(win2file.o)\
	$Z(wrepaint.o)

ALL	= $Z

####### (Standard Productions) #################################################
all\
run_test\
install:	$(SOURCES) $(ALL)

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

$Z(beep.o)\
$Z(cmdch.o)\
$Z(dumpwin.o)\
$Z(erasechr.o)\
$Z(killchar.o)\
$Z(logch.o)\
$Z(rawgets.o)\
$Z(rawterm.o)\
$Z(resizwin.o)\
$Z(savewin.o)\
$Z(viewfile.o)\
$Z(win2file.o)\
$Z(wrepaint.o):	$(CURSES_H)

$Z(cm_qsort.o):	$(QSORT_H)

$Z(m2comp.o)\
$Z(scomp.o):	$I/td_scomp.h

$Z(diffload.o)\
$Z(dyn_app.o)\
$Z(dyn_appc.o)\
$Z(dyn_cata.o)\
$Z(dyn_copy.o)\
$Z(dyn_str.o)\
$Z(fp2argv.o)\
$Z(rawgets.o)\
$Z(rcsperm.o):	$I/dyn_str.h

$Z(rcsargpr.o)\
$Z(rcsedit.o)\
$Z(rcskeys.o)\
$Z(rcslast.o)\
$Z(rcsload.o)\
$Z(rcslocks.o)\
$Z(rcsname.o)\
$Z(rcspath.o)\
$Z(rcsperm.o)\
$Z(rcssymbs.o)\
$Z(rcstime.o)\
$Z(samebr.o)\
$Z(vcs_file.o):	$(RCSDEFS_H)
#
$Z(sccsdbug.o)\
$Z(sccslast.o)\
$Z(sccsname.o):	$(SCCSDEFS_H)
#
$Z(count_ss.o)\
$Z(field_of.o)\
$Z(ss_field.o):	$I/td_sheet.h $I/dyn_str.h
#
####### (Development) ##########################################################
$(SOURCES):			; $(GET) $@
lint.out:	$(CSRC)		; $(DATE); $(LINT_EACH)
lincnt.out:	$(CSRC)		; $(DATE); lincnt $(CSRC) >>$@
tags:		$(CSRC)		; dotags $(CSRC)
