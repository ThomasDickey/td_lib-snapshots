# @(#)Makefile	1.26 88/06/13 09:28:26
# MAKE-file for miscellaneous library routines
L=lib.a
#
OBJS	=\
	abspath.o\
	beep.o\
	bldarg.o\
	catarg.o\
	closeall.o\
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
	flushall.o\
	gethome.o\
	getuser.o\
	gid2s.o\
	killchar.o\
	ltostr.o\
	modechar.o\
	newzone.o\
	okpnum.o\
	packdate.o\
	padedit.o\
	rawgets.o\
	rawterm.o\
	rcsedit.o\
	rcskeys.o\
	rcslast.o\
	rcsname.o\
	resizewin.o\
	savewin.o\
	sccslast.o\
	sccszone.o\
	setmtime.o\
	stralloc.o\
	strclean.o\
	s2gid.o\
	s2uid.o\
	txtalloc.o\
	uid2s.o\
	vecalloc.o\
	which.o\
	win2file.o
#
all:	$L\
	llib-ltd.ln
$L:	$(OBJS)
#
$L:
	ar rv $L $?
	ranlib $L
#
cmdch.o:	cmdch.h
cutoff.o:	ptypes.h
newzone.o:	ptypes.h
packdate.o:	ptypes.h
rawgets.o:	cmdch.h
rcslast.o:	rcsdefs.h
rcsedit.o:	rcsdefs.h
rcskeys.o:	rcsdefs.h
rcsname.o:	rcsdefs.h
sccszone.o:	ptypes.h
setmtime.o:	ptypes.h
win2file.o:	ptypes.h
#
clean:
	rm *.o
clobber: clean
	rm *.a
destroy:
	rm -f *
#
install: all
#
llib-ltd.ln:	LLIB
	makellib.sh LLIB td
#
lint.out:	llib-ltd.ln
	lintit.sh *.c >lint.out
