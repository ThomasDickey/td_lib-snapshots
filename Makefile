# $Id: Makefile,v 11.6 1992/12/04 11:54:58 ste_cm Exp $
# Top-level makefile for CM_TOOLS common library

####### (Development) ##########################################################
INSTALL_INC = ../install_inc
INSTALL_LIB = ../install_lib
INSTALL_MAN = ../install_man

PUT	= rm -f $@; cp -p $? $@
TOP	= ..
GET	= checkout
THIS	= cm_library

I	= $(TOP)/interface
L	= $(TOP)/lib

CLEAN	= *.[oai] *.bak *.log *.out *.tst .nfs* core
DESTROY	=sh -c 'for i in *;do case $$i in RCS);; *) rm -f $$i;;esac;done;exit 0'

####### (Standard Lists) #######################################################
SOURCES	= Makefile descrip.mms README

MFILES	=\
	certificate/Makefile\
	interface/Makefile\
	src/Makefile\
	support/Makefile\
	test/Makefile

IT	=\
	$I/acl.h\
	$I/cmdch.h\
	$I/common.h\
	$I/cm_curses.h\
	$I/cm_qsort.h\
	$I/cm_scomp.h\
	$I/deltree.h\
	$I/dyn_string.h\
	$I/ptypes.h\
	$I/rcsdefs.h\
	$I/sccsdefs.h\
	$I/spreadsheet.h\
	$L/$(THIS).a

####### (Standard Productions) #################################################
all\
lintlib\
install::	lib

all\
clean\
clobber\
destroy\
run_tests\
sources\
lincnt.out\
lint.out::	$(MFILES)
	cd support;	$(MAKE) $@
	cd certificate;	$(MAKE) $@
	cd interface;	$(MAKE) $@
	cd src;		$(MAKE) $@
	cd test;	$(MAKE) $@

lintlib::	$(MFILES)
	cd interface;	$(MAKE) $@

all\
sources::	$(SOURCES)

clean\
clobber::			; rm -f $(CLEAN)
clobber\
destroy::			; rm -rf lib
destroy::			; cd support; $(DESTROY)
destroy::			; $(DESTROY)

install::	all $(IT)
deinstall::			; rm -f $(IT)

####### (Details of Productions) ###############################################
$(MFILES)\
$(SOURCES):				; $(GET) $@
lib:					; mkdir $@

i=interface
$I/acl.h:		$i/acl.h	; $(PUT)
$I/cmdch.h:		$i/cmdch.h	; $(PUT)
$I/cm_curses.h:		$i/cm_curses.h	; $(PUT)
$I/cm_qsort.h:		$i/cm_qsort.h	; $(PUT)
$I/cm_scomp.h:		$i/cm_scomp.h	; $(PUT)
$I/common.h:		$i/common.h	; $(PUT)
$I/deltree.h:		$i/deltree.h	; $(PUT)
$I/dyn_string.h:	$i/dyn_string.h	; $(PUT)
$I/ptypes.h:		$i/ptypes.h	; $(PUT)
$I/rcsdefs.h:		$i/rcsdefs.h	; $(PUT)
$I/sccsdefs.h:		$i/sccsdefs.h	; $(PUT)
$I/spreadsheet.h:	$i/spreadsheet.h; $(PUT)

$L/$(THIS).a:	lib/$(THIS).a		; $(PUT); ranlib $@
