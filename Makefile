# $Id: Makefile,v 10.1 1992/02/03 12:34:03 dickey Exp $
# Top-level makefile for CM_TOOLS common library

####### (Development) ##########################################################
TOP	= ../..
B	= $(TOP)/bin
I	= $(TOP)/interface
L	= $(TOP)/lib

COPY	= rm -f $@; cp -p
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
	$I/spreadsheet.h\
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

i=interface
$I/cmdch.h:		$i/cmdch.h	; $(COPY) $? $@
$I/common.h:		$i/common.h	; $(COPY) $? $@
$I/deltree.h:		$i/deltree.h	; $(COPY) $? $@
$I/ptypes.h:		$i/ptypes.h	; $(COPY) $? $@
$I/rcsdefs.h:		$i/rcsdefs.h	; $(COPY) $? $@
$I/sccsdefs.h:		$i/sccsdefs.h	; $(COPY) $? $@
$I/spreadsheet.h:	$i/spreadsheet.h; $(COPY) $? $@
$L/lib.a:	lib/lib.a		; $(COPY) $? $@; ranlib $@
