# $Id: Makefile,v 12.1 1993/09/21 18:10:49 dickey Exp $
# make-file for TD_LIB common/test library

####### (Development) ##########################################################
TOP	= ../..
include ../support/td_lib.mk

DEFINES	= -DTEST
VPATH	= ../src

.SUFFIXES:
.SUFFIXES: .c .lint
.c.lint:; tdlint $(CPP_OPTS) -ltd -lcurses $< >$@
.c:	; $(LINK) -o $@ $(CFLAGS) $(CPP_OPTS) $< $(LIBS) -lcurses -ltermcap

####### (Standard Lists) #######################################################
DATA_FILES = \
	dummy.txt \
	dummy2.txt \
	dummy3.txt \
	dummy4.txt

REF_FILES = \
	abspath.ref \
	cutoff.ref \
	diffload.ref \
	doalloc.ref \
	dotcmp.ref \
	execute.ref \
	failed.ref \
	file2arg.ref \
	file2mem.ref \
	filecopy.ref \
	fp2argv.ref \
	gethome.ref \
	gid2s.ref \
	lsbycols.ref \
	ltostr.ref \
	m2comp.ref \
	makeargv.ref \
	name2s.ref \
	newzone.ref \
	packdate.ref \
	padedit.ref \
	pathcmp.ref \
	pathhead.ref \
	pathleaf.ref \
	rawgets.ref \
	rcsargpr.ref \
	rcsload.ref \
	rcsname.ref \
	relpath.ref \
	s2uid.ref \
	samebr.ref \
	sameleaf.ref \
	sccsname.ref \
	scomp.ref \
	scr_size.ref \
	strbcmp.ref \
	strucmp.ref \
	strwcmp.ref \
	trimpath.ref \
	txtalloc.ref \
	uid2s.ref \
	usercopy.ref \
	vcs_file.ref \
	vecedit.ref \
	vercmp.ref \
	walkback.ref \
	walktree.ref \
	which.ref

SCRIPTS	= \
	run_test.sh \
	abspath.sh \
	cutoff.sh \
	diffload.sh \
	doalloc.sh \
	dotcmp.sh \
	execute.sh \
	failed.sh \
	file2arg.sh \
	file2mem.sh \
	filecopy.sh \
	fp2argv.sh \
	gethome.sh \
	gid2s.sh \
	lsbycols.sh \
	ltostr.sh \
	m2comp.sh \
	makeargv.sh \
	name2s.sh \
	newzone.sh \
	packdate.sh \
	padedit.sh \
	pathcmp.sh \
	pathhead.sh \
	pathleaf.sh \
	rawgets.sh \
	rcsargpr.sh \
	rcsload.sh \
	rcsname.sh \
	relpath.sh \
	s2uid.sh \
	samebr.sh \
	sameleaf.sh \
	sccsname.sh \
	scomp.sh \
	scr_size.sh \
	strbcmp.sh \
	strucmp.sh \
	strwcmp.sh \
	trimpath.sh \
	txtalloc.sh \
	uid2s.sh \
	usercopy.sh \
	vcs_file.sh \
	vecedit.sh \
	vercmp.sh \
	walkback.sh \
	walktree.sh \
	which.sh

C_SRC	= \
	test_cmp.c

SOURCES	= Makefile README $(C_SRC) $(SCRIPTS) $(REF_FILES) $(DATA_FILES)

PROGS	=\
	abspath\
	cutoff\
	diffload\
	doalloc\
	dotcmp\
	execute\
	failed\
	file2arg\
	file2mem\
	filecopy\
	fp2argv\
	gethome\
	gid2s\
	lsbycols\
	ltostr\
	m2comp\
	makeargv\
	name2s\
	newzone\
	packdate\
	padedit\
	pathcmp\
	pathhead\
	pathleaf\
	rawgets\
	rcsargpr\
	rcsload\
	rcsname\
	relpath\
	s2uid\
	samebr\
	sameleaf\
	sccsname\
	scomp\
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
	walkback\
	walktree\
	which

ALL	= $(PROGS)

####### (Standard Productions) #################################################
all:		$(SOURCES)	; @echo 'type "make run_test"'
clean:				; rm -f $(CLEAN) *.tmp *.pure*
clobber:	clean		; rm -f $(ALL)
destroy:			; $(DESTROY)
sources:	$(SOURCES)
programs:	$(PROGS)
run_test:	$(SOURCES) $(PROGS) ; $(RUN_TESTS)
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
file2arg.c\
file2mem.c\
filecopy.c\
fp2argv.c\
gethome.c\
gid2s.c\
lsbycols.c\
ltostr.c\
m2comp.c\
makeargv.c\
name2s.c\
newzone.c\
packdate.c\
padedit.c\
pathcmp.c\
pathhead.c\
pathleaf.c\
rawgets.c\
rcsargpr.c\
rcsload.c\
rcsname.c\
relpath.c\
s2uid.c\
samebr.c\
sameleaf.c\
sccsname.c\
scomp.c\
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
walkback.c\
walktree.c\
which.c:
