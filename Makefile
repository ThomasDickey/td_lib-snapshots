# $Id: Makefile,v 12.0 1993/04/29 09:07:55 ste_cm Rel $
# Top-level makefile for TD_LIB common library

####### (Development) ##########################################################
INSTALL_INC = ../install_inc
INSTALL_LIB = ../install_lib
INSTALL_MAN = ../install_man

PUT	= rm -f $@; cp -p $? $@
TOP	= ..
GET	= checkout
THIS	= td_lib

I	= $(TOP)/include
L	= $(TOP)/lib

CLEAN	= *.[oai] *.bak *.log *.out *.tst .nfs* core
DESTROY	=sh -c 'for i in *;do case $$i in RCS);; *) rm -f $$i;;esac;done;exit 0'

####### (Standard Lists) #######################################################
SOURCES	= Makefile descrip.mms README COPYING

MFILES	=\
	certify/Makefile\
	include/Makefile\
	src/Makefile\
	support/Makefile\
	test/Makefile

IT	=\
	$I/acl.h\
	$I/cmdch.h\
	$I/$(THIS).h\
	$I/td_curse.h\
	$I/td_qsort.h\
	$I/td_scomp.h\
	$I/td_sheet.h\
	$I/deltree.h\
	$I/dyn_str.h\
	$I/ptypes.h\
	$I/rcsdefs.h\
	$I/sccsdefs.h\
	$L/$(THIS).a

####### (Standard Productions) #################################################
all\
lintlib\
install::	lib

all\
clean\
clobber\
destroy\
run_test\
sources\
lincnt.out\
lint.out::	$(MFILES)
	cd support;	$(MAKE) $@
	cd certify;	$(MAKE) $@
	cd include;	$(MAKE) $@
	cd src;		$(MAKE) $@
	cd test;	$(MAKE) $@

lintlib::	$(MFILES)
	cd include;	$(MAKE) $@

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

i=include
$I/acl.h:		$i/acl.h	; $(PUT)
$I/cmdch.h:		$i/cmdch.h	; $(PUT)
$I/td_curse.h:		$i/td_curse.h	; $(PUT)
$I/td_qsort.h:		$i/td_qsort.h	; $(PUT)
$I/td_scomp.h:		$i/td_scomp.h	; $(PUT)
$I/td_sheet.h:		$i/td_sheet.h	; $(PUT)
$I/$(THIS).h:		$i/$(THIS).h	; $(PUT)
$I/deltree.h:		$i/deltree.h	; $(PUT)
$I/dyn_str.h:		$i/dyn_str.h	; $(PUT)
$I/ptypes.h:		$i/ptypes.h	; $(PUT)
$I/rcsdefs.h:		$i/rcsdefs.h	; $(PUT)
$I/sccsdefs.h:		$i/sccsdefs.h	; $(PUT)

$L/$(THIS).a:	lib/$(THIS).a		; $(PUT); ranlib $@
