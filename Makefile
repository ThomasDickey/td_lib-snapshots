# $Id: Makefile,v 10.4 1992/02/06 15:50:21 dickey Exp $
# Top-level makefile for CM_TOOLS common library

####### (Development) ##########################################################
TOP	= ../..
B	= $(TOP)/bin
I	= $(TOP)/interface
L	= $(TOP)/lib

GET	= checkout
COPY	= cp -p
PUT	= rm -f $@; $(COPY) $? $@
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
	$I/dyn_string.h\
	$I/ptypes.h\
	$I/rcsdefs.h\
	$I/sccsdefs.h\
	$I/spreadsheet.h\
	$L/lib.a

####### (Standard Productions) #################################################
all\
lintlib\
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

lintlib::	$(MFILES)
	cd interface;	$(MAKE) $@

sources::	$(SOURCES)

clean\
clobber::
	rm -f *.bak *.log *.out core
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
$(SOURCES):				; $(GET) -x $@
lib:					; mkdir $@

i=interface
$I/cmdch.h:		$i/cmdch.h	; $(PUT)
$I/common.h:		$i/common.h	; $(PUT)
$I/deltree.h:		$i/deltree.h	; $(PUT)
$I/dyn_string.h:	$i/dyn_string.h	; $(PUT)
$I/ptypes.h:		$i/ptypes.h	; $(PUT)
$I/rcsdefs.h:		$i/rcsdefs.h	; $(PUT)
$I/sccsdefs.h:		$i/sccsdefs.h	; $(PUT)
$I/spreadsheet.h:	$i/spreadsheet.h; $(PUT)

$L/lib.a:	lib/lib.a		; $(PUT); ranlib $@
