# $Id: Makefile,v 8.0 1990/05/08 13:53:34 ste_cm Rel $
# Top-level makefile for CM_TOOLS common library
#
# $Log: Makefile,v $
# Revision 8.0  1990/05/08 13:53:34  ste_cm
# BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
#
#	Revision 7.1  90/05/08  13:53:34  dickey
#	added 'sccsdefs.h' to install-rule; simplified other rules
#	
#	Revision 7.0  89/08/22  09:29:58  ste_cm
#	BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
#	
####### (Development) ##########################################################
I	= ../../interface
L	= ../../lib
GET	= checkout
PUT	= copy -v -d ../$@
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
$L/lib.a:	lib/lib.a		; cd lib;	$(PUT)
