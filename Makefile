# $Id: Makefile,v 7.0 1989/08/22 09:29:58 ste_cm Rel $
# Top-level makefile for CM_TOOLS common library
#
# $Log: Makefile,v $
# Revision 7.0  1989/08/22 09:29:58  ste_cm
# BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
#
#	Revision 6.0  89/08/22  09:29:58  ste_cm
#	BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
#	
#	Revision 5.0  89/08/22  09:29:58  ste_cm
#	BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
#	
#	Revision 4.0  89/08/22  09:29:58  ste_cm
#	BASELINE Thu Aug 24 09:34:01 EDT 1989 -- support:navi_011(rel2)
#	
#	Revision 3.1  89/08/22  09:29:58  dickey
#	corrected 'destroy' rule
#	
#	Revision 3.0  89/06/08  12:28:54  ste_cm
#	BASELINE Mon Jun 19 13:20:43 EDT 1989
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
destroy:	$(FIRST)
	cd interface;	$(MAKE) destroy
	cd src;		$(MAKE) destroy
	rm -rf lib
	sh -c 'for i in *;do case $$i in RCS);; *) rm -f $$i;;esac;done;exit 0'

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
