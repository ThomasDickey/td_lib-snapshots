# @(#)Makefile	1.33 88/07/29 10:45:06
# MAKE-file for miscellaneous library routines
L=lib.a
GET=getdelta
#
SRCS	=\
	abspath.c\
	beep.c\
	bldarg.c\
	catarg.c\
	closeall.c\
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
	flushall.c\
	fp2argv.c\
	gethome.c\
	getuser.c\
	gid2s.c\
	killchar.c\
	ltostr.c\
	modechar.c\
	newzone.c\
	okpnum.c\
	packdate.c\
	padedit.c\
	rawgets.c\
	rawterm.c\
	rcsedit.c\
	rcskeys.c\
	rcslast.c\
	rcsname.c\
	resizewin.c\
	savewin.c\
	sccslast.c\
	scr_size.c\
	setmtime.c\
	stralloc.c\
	strclean.c\
	s2gid.c\
	s2uid.c\
	txtalloc.c\
	uid2s.c\
	vecalloc.c\
	which.c\
	win2file.c
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
	fp2argv.o\
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
	scr_size.o\
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
#
$L:	$(OBJS)
	ar rv $L $?
	ranlib $L
#
archive: $(OBJS)
	rm -f $L
	ar rv $L ($OBJS)
#
cmdch.o:	cmdch.h
cutoff.o:	ptypes.h
execute.o:	ptypes.h
newzone.o:	ptypes.h
packdate.o:	ptypes.h
rawgets.o:	cmdch.h
rcslast.o:	rcsdefs.h
rcsedit.o:	rcsdefs.h
rcskeys.o:	rcsdefs.h
rcsname.o:	rcsdefs.h
setmtime.o:	ptypes.h
win2file.o:	ptypes.h
#
clean:
	rm -f *.o *.out *.ln
clobber: clean
	rm -f *.a
destroy:
	rm -f *
#
install: all
#
cmdch.h\
ptypes.h\
rcsdefs.h\
lintit.sh\
makellib.sh:
LLIB\
$(SRCS):
	$(GET) -s sccs/s.$@
#
llib-ltd.ln:	makellib.sh LLIB
	makellib.sh LLIB td
#
lint.out:	lintit.sh llib-ltd.ln
	lintit.sh *.c >lint.out
