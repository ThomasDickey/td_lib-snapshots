# $Id: Makefile,v 7.0 1989/10/04 11:19:02 ste_cm Rel $
# make-file for CM_TOOLS common interface directory
#
# $Log: Makefile,v $
# Revision 7.0  1989/10/04 11:19:02  ste_cm
# BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
#
#	Revision 6.0  89/10/04  11:19:02  ste_cm
#	BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
#	
#	Revision 5.0  89/10/04  11:19:02  ste_cm
#	BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
#	
#	Revision 4.1  89/10/04  11:19:02  dickey
#	added "acl.h" to source-list
#	
#	Revision 4.0  89/08/22  10:31:03  ste_cm
#	BASELINE Thu Aug 24 09:35:35 EDT 1989 -- support:navi_011(rel2)
#	

####### (Development) ##########################################################
GET	= checkout

####### (Command-line Options) #################################################
llib	= td
LLIB	= llib-l$(llib)

####### (Standard Lists) #######################################################
L	= ../lib
H_SRC	=\
	acl.h \
	cmdch.h\
	common.h\
	ptypes.h\
	rcsdefs.h
L_SRC	=\
	llib-ltd
SOURCES	= Makefile $(H_SRC) $(L_SRC)

ALL	= $L/$(LLIB).ln

####### (Standard Productions) #################################################
all:		sources $(ALL)
clean:			; rm -f *.out *.ln core
clobber:	clean	; rm -f $(ALL)
destroy:
	sh -c 'for i in *;do case $$i in RCS);; *) rm -f $$i;;esac;done;exit 0'
sources:	$(SOURCES)
lint.out:
lincnt.out:	$(H_SRC)	; lincnt $(H_SRC) >$@

####### (Details of Productions) ###############################################
#
$(SOURCES):		; $(GET) $@
$L/$(LLIB).ln:	$(H_SRC) $(LLIB) ; - sh -c 'makellib -L$L $(LLIB) $(llib)'
