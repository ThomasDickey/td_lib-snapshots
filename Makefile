# $Header: /users/source/archives/td_lib.vcs/RCS/Makefile,v 2.1 1989/04/24 11:42:17 dickey Exp $
# Top-level makefile for CM_TOOLS common library
#
# $Log: Makefile,v $
# Revision 2.1  1989/04/24 11:42:17  dickey
# added README file
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
SOURCES	= README
FIRST	=\
	$(SOURCES)\
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
$(SOURCES):				; $(GET) $@
lib:					; mkdir $@
interface/Makefile:			; cd interface;	$(GET) Makefile
src/Makefile:				; cd src;	$(GET) Makefile

$I/cmdch.h:	interface/cmdch.h	; cd interface;	$(PUT)
$I/ptypes.h:	interface/ptypes.h	; cd interface;	$(PUT)
$I/rcsdefs.h:	interface/rcsdefs.h	; cd interface;	$(PUT)
$L/lib.a:	lib/lib.a		; cd lib;	$(PUT)
