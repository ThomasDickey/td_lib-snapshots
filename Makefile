# $Id: Makefile,v 9.1 1991/07/23 10:01:32 dickey Exp $
# Top-level makefile for CM_TOOLS common library
#
# $Log: Makefile,v $
# Revision 9.1  1991/07/23 10:01:32  dickey
# on SunOs, must ranlib archive after copying it
#
#	Revision 9.0  91/06/05  14:36:31  ste_cm
#	BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
#	
####### (Development) ##########################################################
TOP	= ../..
B	= $(TOP)/bin
I	= $(TOP)/interface
L	= $(TOP)/lib
GET	= checkout
PUT	= ../$B/copy -d ../$@
CFLAGS	=
MAKE	= make $(MFLAGS) -k$(MAKEFLAGS)	GET=$(GET) CFLAGS="$(CFLAGS)"

####### (Standard Lists) #######################################################
SOURCES	= Makefile descrip.mms README
FIRST	=\
	$(SOURCES)\
	lib\
	interface/Makefile\
	src/Makefile

ALL	=\
	$I/cmdch.h\
	$I/common.h\
	$I/ptypes.h\
	$I/rcsdefs.h\
	$I/sccsdefs.h\
	$L/lib.a

####### (Standard Productions) #################################################
all\
clean\
clobber\
destroy rdestroy\
sources\
lincnt.out\
lint.out::	$(FIRST)
	cd interface;	$(MAKE) $@
	cd src;		$(MAKE) $@

rdestroy\
destroy::
	rm -rf lib
	sh -c 'for i in *;do case $$i in RCS);; *) rm -f $$i;;esac;done;exit 0'

run_tests:
	@echo '** no test suite available for this module'

install:	all $(ALL)
deinstall:		; rm -f $(ALL)

####### (Details of Productions) ###############################################
interface/Makefile\
src/Makefile\
$(SOURCES):				; $(GET) -x $@
lib:					; mkdir $@

$I/cmdch.h:	interface/cmdch.h	; cd interface;	$(PUT)
$I/common.h:	interface/common.h	; cd interface;	$(PUT)
$I/ptypes.h:	interface/ptypes.h	; cd interface;	$(PUT)
$I/rcsdefs.h:	interface/rcsdefs.h	; cd interface;	$(PUT)
$I/sccsdefs.h:	interface/sccsdefs.h	; cd interface;	$(PUT)
$L/lib.a:	lib/lib.a		; cd lib;	$(PUT); ranlib $@
