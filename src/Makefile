# @(#)Makefile	1.3 87/11/25 11:30:15
# MAKE-file for miscellaneous library routines
L=lib.a
#
OBJS	=\
	bldarg.o\
	closeall.o\
	flushall.o\
	gid2s.o\
	modechar.o\
	packdate.o\
	rawterm.o\
	sccslast.o\
	sccszone.o\
	strclean.o\
	s2gid.o\
	s2uid.o\
	uid2s.o\
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
lint.out:	LINT
	tdlint -lcurses -ltd  -DNO_SCCS_ID *.c >lint.out
