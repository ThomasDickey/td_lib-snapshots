# $Id: Makefile,v 4.0 1989/08/22 10:31:03 ste_cm Rel $
# make-file for CM_TOOLS common interface directory
#
# $Log: Makefile,v $
# Revision 4.0  1989/08/22 10:31:03  ste_cm
# BASELINE Thu Aug 24 09:35:35 EDT 1989 -- support:navi_011(rel2)
#
#	Revision 3.2  89/08/22  10:31:03  dickey
#	invoke 'makellib' via 'sh' so this works ok on sparc.
#	
#	Revision 3.1  89/08/22  09:36:10  dickey
#	corrected 'destroy' rule; added 'lincnt.out'
#	
#	Revision 3.0  89/06/08  12:25:14  ste_cm
#	BASELINE Mon Jun 19 13:22:44 EDT 1989
#	

####### (Development) ##########################################################
GET	= checkout

####### (Command-line Options) #################################################
llib	= td
LLIB	= llib-l$(llib)

####### (Standard Lists) #######################################################
L	= ../lib
H_SRC	=\
	cmdch.h\
	common.h\
	ptypes.h\
	rcsdefs.h
L_SRC	=\
	llib-ltd
SOURCES	= Makefile $(H_SRC) $(L_SRC)

ALL	= $L/$(LLIB).ln

####### (Standard Productions) #################################################
all:	$(ALL)
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
