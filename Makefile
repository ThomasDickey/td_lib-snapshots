# $Header: /users/source/archives/td_lib.vcs/RCS/Makefile,v 3.0 1989/06/08 12:28:54 ste_cm Rel $
# Top-level makefile for CM_TOOLS common library
#
# $Log: Makefile,v $
# Revision 3.0  1989/06/08 12:28:54  ste_cm
# BASELINE Mon Jun 19 13:20:43 EDT 1989
#
#	Revision 2.4  89/06/08  12:28:54  dickey
#	corrected 'destroy' rule
#	
#	Revision 2.3  89/04/25  12:48:12  dickey
#	added common.h to install-list
#	
#	Revision 2.2  89/04/25  08:35:36  dickey
#	ported subset of this library to VMS; added descrip.mms for that purpose
#	
#	Revision 2.1  89/04/24  11:42:17  dickey
#	added README file
#	
#	Revision 2.0  89/03/28  10:20:53  ste_cm
#	BASELINE Thu Apr  6 09:37:24 EDT 1989
#	
####### (Development) ##########################################################
I	= ../../interface
L	= ../../lib
GET	= checkout
PUT	= copy -v -d ../$@
MAKE	= make $(MFLAGS) -k$(MAKEFLAGS)	GET=$(GET)

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
	$L/lib.a

####### (Standard Productions) #################################################
all\
clean\
clobber\
sources\
lincnt.out\
lint.out:	$(FIRST)
	cd interface;	$(MAKE) $@
	cd src;		$(MAKE) $@

rdestroy\
destroy:	$(FIRST) clobber
	cd interface;	$(MAKE) destroy
	cd src;		$(MAKE) destroy
	rmdir lib
	sh -c 'for i in *;do case $$i in RCS);; *) rm -f $$i;;esac;done'

run_tests:
	@echo '** no test suite available for this module'

install:	all $(ALL)
deinstall:		; rm -f $(ALL)

####### (Details of Productions) ###############################################
$(SOURCES):				; $(GET) $@
lib:					; mkdir $@
interface/Makefile:			; cd interface;	$(GET) Makefile
src/Makefile:				; cd src;	$(GET) Makefile

$I/cmdch.h:	interface/cmdch.h	; cd interface;	$(PUT)
$I/common.h:	interface/common.h	; cd interface;	$(PUT)
$I/ptypes.h:	interface/ptypes.h	; cd interface;	$(PUT)
$I/rcsdefs.h:	interface/rcsdefs.h	; cd interface;	$(PUT)
$L/lib.a:	lib/lib.a		; cd lib;	$(PUT)
