# $Header: /users/source/archives/td_lib.vcs/RCS/Makefile,v 1.2 1989/03/24 13:13:30 dickey Exp $
# Top-level makefile for CM_TOOLS common library
#
# $Log: Makefile,v $
# Revision 1.2  1989/03/24 13:13:30  dickey
# added install/deinstall rules
#
#	Revision 1.1  89/03/21  16:25:41  dickey
#	Initial revision
#	
#
####### (Development) ##########################################################
I	= ../../interface
L	= ../../lib
GET	= checkout
PUT	= copy -d -v ../$@

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
destroy\
sources\
lint.out:	$(FIRST)
	cd interface;	make $(MFLAGS) $@
	cd src;		make $(MFLAGS) $@

run_tests:
	@echo '** no test suite available for this module'

install:	all $(ALL)
deinstall:		; rm -f $(ALL)

####### (Details of Productions) ###############################################
lib:			; mkdir $@
interface/Makefile:	; cd interface;	$(GET) Makefile
src/Makefile:		; cd src;	$(GET) Makefile
$I/cmdch.h:		; cd interface;	$(PUT)
$I/ptypes.h:		; cd interface;	$(PUT)
$I/rcsdefs.h:		; cd interface;	$(PUT)
$L/lib.a:		; cd lib;	$(PUT)
