# @(#)Makefile	1.13 88/05/11 09:19:25
# MAKE-file for miscellaneous library routines
L=lib.a
#
OBJS	=\
	abspath.o\
	beep.o\
	bldarg.o\
	closeall.o\
	cmdch.o\
	denode.o\
	doalloc.o\
	dotname.o\
	dumpchr.o\
	dumpwin.o\
	erasechar.o\
	flushall.o\
	gid2s.o\
	killchar.o\
	modechar.o\
	okpnum.o\
	packdate.o\
	padedit.o\
	rawgets.o\
	rawterm.o\
	resizewin.o\
	savewin.o\
	sccslast.o\
	sccszone.o\
	stralloc.o\
	strclean.o\
	s2gid.o\
	s2uid.o\
	txtalloc.o\
	uid2s.o\
	vecalloc.o\
	which.o
#
it:	$L\
	llib-ltd.ln
$L:	$(OBJS)
#
$L:
	ar rv $L $?
	ranlib $L
#
cmdch.o:	cmdch.h
rawgets.o:	cmdch.h
#
clean:
	rm *.o
clobber: clean
	rm *.a
#
llib-ltd.ln:	LLIB
	makellib.sh LLIB td
#
lint.out:	llib-ltd.ln
	tdlint -lcurses -ltd -lapollo -DNO_SCCS_ID *.c >lint.out
