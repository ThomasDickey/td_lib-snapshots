# @(#)Makefile	1.4 87/12/01 10:02:35
# MAKE-file for miscellaneous library routines
L=lib.a
#
OBJS	=\
	bldarg.o\
	closeall.o\
	cmdch.o\
	doalloc.o\
	flushall.o\
	gid2s.o\
	modechar.o\
	packdate.o\
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
llib-ltd.ln:	LINT
	makellib.sh LINT td
#
lint.out:	llib-ltd.ln
	tdlint -lcurses -ltd  -DNO_SCCS_ID *.c >lint.out
