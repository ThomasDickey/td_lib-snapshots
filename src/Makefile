# @(#)Makefile	1.7 88/03/24 11:41:41
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
	erasechar.o\
	flushall.o\
	gid2s.o\
	killchar.o\
	modechar.o\
	okpnum.o\
	packdate.o\
	rawgets.o\
	rawterm.o\
	sccslast.o\
	sccszone.o\
	stralloc.o\
	strclean.o\
	s2gid.o\
	s2uid.o\
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
#
clean:
	rm *.o
clobber: clean
	rm *.a
#
llib-ltd.ln:	LINT
	makellib.sh LINT td
#
lint.out:	llib-ltd.ln
	tdlint -lcurses -ltd  -DNO_SCCS_ID *.c >lint.out
