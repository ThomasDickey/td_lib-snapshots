# $Header: /users/source/archives/td_lib.vcs/RCS/Makefile,v 2.0 1989/03/28 10:20:53 ste_cm Exp $
# Top-level makefile for CM_TOOLS common library
#
# $Log: Makefile,v $
# Revision 2.0  1989/03/28 10:20:53  ste_cm
# BASELINE Thu Apr  6 09:37:24 EDT 1989
#
#	Revision 1.4  89/03/28  10:20:53  dickey
#	use MAKE-variable to encapsulate recursive-build info.
#	
#	Revision 1.3  89/03/27  14:02:59  dickey
#	integration/cleanup of recursive make
#	
#	Revision 1.2  89/03/24  13:13:30  dickey
#	added install/deinstall rules
#	
#	Revision 1.1  89/03/21  16:25:41  dickey
#	Initial revision
#	
#
####### (Development) ##########################################################
I	= ../../interface
L	= ../../lib
GET	= checkout
PUT	= copy -v -d ../$@
MAKE	= make $(MFLAGS) -k$(MAKEFLAGS)	GET=$(GET)

####### (Standard Lists) #######################################################
FIRST	=\
	lib\
	interface/Makefile\
	src/Makefile

ALL	=\
	$I/cmdch.h\
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
	rm -f *

run_tests:
	@echo '** no test suite available for this module'

install:	all $(ALL)
deinstall:		; rm -f $(ALL)

####### (Details of Productions) ###############################################
lib:					; mkdir $@
interface/Makefile:			; cd interface;	$(GET) Makefile
src/Makefile:				; cd src;	$(GET) Makefile

$I/cmdch.h:	interface/cmdch.h	; cd interface;	$(PUT)
$I/ptypes.h:	interface/ptypes.h	; cd interface;	$(PUT)
$I/rcsdefs.h:	interface/rcsdefs.h	; cd interface;	$(PUT)
$L/lib.a:	lib/lib.a		; cd lib;	$(PUT)
