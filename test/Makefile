# $Id: Makefile,v 11.5 1992/11/18 09:41:56 dickey Exp $
# make-file for CM_TOOLS common/test library

####### (Development) ##########################################################
TOP	= ../..
include ../support/cm_library.mk

DEFINES	= -DTEST
VPATH	= ../src

.SUFFIXES:
.SUFFIXES: .c .lint
.c.lint:; tdlint $(CPP_OPTS) -ltd -lcurses $< >$@
.c:	; $(CC) -o $@ $(CFLAGS) $(CPP_OPTS) $< $(LIBS) -lcurses -ltermcap

####### (Standard Lists) #######################################################
REF_FILES =
SCRIPTS	= \
	run_tests.sh

SOURCES	= Makefile descrip.mms README $(SCRIPTS) $(REF_FILES)

PROGS	=\
	abspath\
	cutoff\
	diffload\
	doalloc\
	dotcmp\
	execute\
	failed\
	file2argv\
	file2mem\
	filecopy\
	fp2argv\
	gethome\
	gid2s\
	list_by_cols\
	ltostr\
	makeargv\
	name2s\
	newzone\
	packdate\
	padedit\
	pathcmp\
	pathhead\
	pathleaf\
	rawgets\
	rcsargpair\
	rcsload\
	rcsname\
	relpath\
	s2uid\
	samebranch\
	sameleaf\
	sccsname\
	scr_size\
	strbcmp\
	strucmp\
	strwcmp\
	trimpath\
	txtalloc\
	uid2s\
	usercopy\
	vcs_file\
	vecedit\
	vercmp\
	walktree\
	which

ALL	= $(PROGS)

####### (Standard Productions) #################################################
all:		$(SOURCES)	; @echo 'type "make run_tests"'
clean:				; rm -f $(CLEAN)
clobber:	clean		; rm -f $(ALL)
destroy:			; $(DESTROY)
sources:	$(SOURCES)
run_tests:	$(PROGS)	; $(RUN_TESTS)
lint.out:
lincnt.out:

####### (Details of Productions) ###############################################
#
$(SOURCES):		; $(GET) $@

# rules to make SunOs 'make' "work" with VPATH
abspath.c\
cutoff.c\
diffload.c\
doalloc.c\
dotcmp.c\
execute.c\
failed.c\
file2argv.c\
file2mem.c\
filecopy.c\
fp2argv.c\
gethome.c\
gid2s.c\
list_by_cols.c\
ltostr.c\
makeargv.c\
name2s.c\
newzone.c\
packdate.c\
padedit.c\
pathcmp.c\
pathhead.c\
pathleaf.c\
rawgets.c\
rcsargpair.c\
rcsload.c\
rcsname.c\
relpath.c\
s2uid.c\
samebranch.c\
sameleaf.c\
sccsname.c\
scr_size.c\
strbcmp.c\
strucmp.c\
strwcmp.c\
trimpath.c\
txtalloc.c\
uid2s.c\
usercopy.c\
vcs_file.c\
vecedit.c\
vercmp.c\
walktree.c\
which.c:
