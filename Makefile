# $Id: Makefile,v 10.0 1991/10/21 09:17:30 ste_cm Rel $
# Top-level makefile for CM_TOOLS common library

####### (Development) ##########################################################
TOP	= ../..
B	= $(TOP)/bin
I	= $(TOP)/interface
L	= $(TOP)/lib

COPY	= cp -p
MAKE	= make $(MFLAGS) -k$(MAKEFLAGS)	CFLAGS="$(CFLAGS)" COPY="$(COPY)"

####### (Standard Lists) #######################################################
SOURCES	= Makefile descrip.mms README

MFILES	=\
	interface/Makefile\
	src/Makefile\
	test/Makefile

ALL	=\
	$I/cmdch.h\
	$I/common.h\
	$I/deltree.h\
	$I/ptypes.h\
	$I/rcsdefs.h\
	$I/sccsdefs.h\
	$L/lib.a

####### (Standard Productions) #################################################
all\
install::	lib

all\
clean\
clobber\
destroy\
sources\
lincnt.out\
lint.out::	$(MFILES)
	cd interface;	$(MAKE) $@
	cd src;		$(MAKE) $@
	cd test;	$(MAKE) $@

sources::	$(SOURCES)

clobber\
destroy::
	rm -rf lib
destroy::
	sh -c 'for i in *;do case $$i in RCS);; *) rm -f $$i;;esac;done;exit 0'

run_tests::
	@echo '** no test suite available for this module'

install::	all $(ALL)
deinstall::		; rm -f $(ALL)

####### (Details of Productions) ###############################################
$(MFILES)\
$(SOURCES):				; checkout -x $@
lib:					; mkdir $@

$I/cmdch.h:	interface/cmdch.h	; $(COPY) $? $@
$I/common.h:	interface/common.h	; $(COPY) $? $@
$I/deltree.h:	interface/deltree.h	; $(COPY) $? $@
$I/ptypes.h:	interface/ptypes.h	; $(COPY) $? $@
$I/rcsdefs.h:	interface/rcsdefs.h	; $(COPY) $? $@
$I/sccsdefs.h:	interface/sccsdefs.h	; $(COPY) $? $@
$L/lib.a:	lib/lib.a		; $(COPY) $? $@; ranlib $@
