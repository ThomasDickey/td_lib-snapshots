# $Id: Makefile,v 11.3 1992/10/09 12:14:39 dickey Exp $
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

####### (Standard Lists) #######################################################
SOURCES	= Makefile descrip.mms README

MFILES	=\
	certificate/Makefile\
	interface/Makefile\
	src/Makefile\
	support/Makefile\
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
	$L/$(THIS).a

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
	cd support;	$(MAKE) $@
	cd certificate;	$(MAKE) $@
	cd interface;	$(MAKE) $@
	cd src;		$(MAKE) $@
	cd test;	$(MAKE) $@

lintlib::	$(MFILES)
	cd interface;	$(MAKE) $@

sources::	$(SOURCES)

clean\
clobber::			; rm -f $(CLEAN)
clobber\
destroy::			; rm -rf lib
destroy::			; $(DESTROY)

run_tests::
	@echo '** no test suite available for this module'

install::	all $(ALL)
deinstall::			; rm -f $(ALL)

####### (Details of Productions) ###############################################
$(MFILES)\
$(SOURCES):				; $(GET) $@
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

$L/$(THIS).a:	lib/$(THIS).a		; $(PUT); ranlib $@
