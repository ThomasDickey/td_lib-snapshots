# @(#)Makefile	1.50 88/09/09 09:29:22
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
	blip.c\
	catarg.c\
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
	filecopy.c\
	fp2argv.c\
	gethome.c\
	getuser.c\
	gid2s.c\
	killchar.c\
	ltostr.c\
	modechar.c\
	name2s.c\
	newzone.c\
	packdate.c\
	padedit.c\
	pathhead.c\
	pathleaf.c\
	rawgets.c\
	rawterm.c\
	rcs_dir.c\
	rcsedit.c\
	rcskeys.c\
	rcslast.c\
	rcsload.c\
	rcslocks.c\
	rcsname.c\
	rcspath.c\
	rcstemp.c\
	resizewin.c\
	savewin.c\
	sccs_dir.c\
	sccslast.c\
	scr_size.c\
	setmtime.c\
	stralloc.c\
	strclean.c\
	s2gid.c\
	s2uid.c\
	txtalloc.c\
	uid2s.c\
	usercopy.c\
	userprot.c\
	vecalloc.c\
	walktree.c\
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
	blip.o\
	catarg.o\
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
	filecopy.o\
	fp2argv.o\
	gethome.o\
	getuser.o\
	gid2s.o\
	killchar.o\
	ltostr.o\
	modechar.o\
	name2s.o\
	newzone.o\
	packdate.o\
	padedit.o\
	pathhead.o\
	pathleaf.o\
	rawgets.o\
	rawterm.o\
	rcs_dir.o\
	rcsedit.o\
	rcskeys.o\
	rcslast.o\
	rcsload.o\
	rcslocks.o\
	rcsname.o\
	rcspath.o\
	rcstemp.o\
	resizewin.o\
	savewin.o\
	sccs_dir.o\
	sccslast.o\
	scr_size.o\
	setmtime.o\
	stralloc.o\
	strclean.o\
	s2gid.o\
	s2uid.o\
	txtalloc.o\
	uid2s.o\
	usercopy.o\
	userprot.o\
	vecalloc.o\
	walktree.o\
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
beep.o:		ptypes.h
cmdch.o:	ptypes.h	cmdch.h
copyback.o:	ptypes.h
cutoff.o:	ptypes.h
doalloc.o:	ptypes.h
dumpwin.o:	ptypes.h
erasechar.o:	ptypes.h
execute.o:	ptypes.h
filecopy.o:	ptypes.h
fp2argv.o:	ptypes.h
gid2s.o:	ptypes.h
killchar.o:	ptypes.h
name2s.o:	ptypes.h
newzone.o:	ptypes.h
packdate.o:	ptypes.h
padedit.o:	ptypes.h
rawgets.o:	ptypes.h	cmdch.h
rawterm.o:	ptypes.h
rcsedit.o:	ptypes.h	rcsdefs.h
rcskeys.o:	rcsdefs.h
rcslast.o:	ptypes.h	rcsdefs.h
rcsload.o:	ptypes.h	rcsdefs.h
rcslocks.o:	ptypes.h	rcsdefs.h
rcsname.o:	ptypes.h	rcsdefs.h
resizewin.o:	ptypes.h
sccslast.o:	ptypes.h
setmtime.o:	ptypes.h
savewin.o:	ptypes.h
uid2s.o:	ptypes.h
usercopy.o:	ptypes.h
userprot.o:	ptypes.h
walktree.o:	ptypes.h
win2file.o:	ptypes.h
#
####### (Development) ##########################################################
$(LLIB).ln:	ptypes.h rcsdefs.h $(LLIB) ; makellib.sh $(LLIB) $(llib)
$(SRCS):			; $(GET) -s sccs/s.$@
lint.out:	$(CSRC)		; $(DATE); tdlint $(LINTOPT) $? >>$@
lincnt.out:	$(CSRC)		; $(DATE); lincnt $(CSRC) >>$@
