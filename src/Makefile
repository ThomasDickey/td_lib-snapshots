# @(#)Makefile	1.37 88/08/09 09:37:56
# MAKE-file for miscellaneous library routines
#
####### (Development) ##########################################################
GET	= getdelta
#
####### (Command-line Options) #################################################
llib	= td
LLIB	= llib-l$(llib)
CFLAGS  = $(DEFINES) $(INCLUDES) -O
DATE	= echo '****'`date` >> $@
LINTOPT	= $(DEFINES) $(INCLUDES) -ltd -lcurses -lapollo
#
####### (Standard Lists) #######################################################
ALL	= $A $(LLIB).ln
A	= lib.a
CSRC	=\
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
HSRC	= cmdch.h ptypes.h rcsdefs.h
SCRIPTS	= makellib.sh
SRCS	= $(CSRC) $(HSRC) $(SCRIPTS) $(LLIB)
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
####### (Standard Productions) #################################################
all:		$(ALL)
#
clean:				; rm -f *.o *.out *.ln
clobber:	clean		; rm -f $(ALL)
destroy:			; rm -f *
sources:	$(SRCS)
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
cmdch.o:	cmdch.h
cutoff.o:	ptypes.h
execute.o:	ptypes.h
fp2argv.o:	ptypes.h
gid2s.o:	ptypes.h
newzone.o:	ptypes.h
packdate.o:	ptypes.h
rawgets.o:	cmdch.h
rcslast.o:	rcsdefs.h
rcsedit.o:	rcsdefs.h
rcskeys.o:	rcsdefs.h
rcsname.o:	rcsdefs.h
setmtime.o:	ptypes.h
uid2s.o:	ptypes.h
win2file.o:	ptypes.h
#
####### (Development) ##########################################################
$(LLIB).ln:	$(LLIB)		; makellib.sh $(LLIB) $(llib)
$(SRCS):			; $(GET) -s sccs/s.$@
lint.out:	$(CSRC)		; $(DATE); tdlint $(LINTOPT) $? >>$@
lincnt.out:	$(CSRC)		; $(DATE); lincnt $(CSRC) >>$@
