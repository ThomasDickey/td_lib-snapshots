# $Id: Makefile,v 9.0 1990/05/08 13:27:45 ste_cm Rel $
# make-file for CM_TOOLS common interface directory
#
# $Log: Makefile,v $
# Revision 9.0  1990/05/08 13:27:45  ste_cm
# BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
#
#	Revision 8.0  90/05/08  13:27:45  ste_cm
#	BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
#	
#	Revision 7.1  90/05/08  13:27:45  dickey
#	broke 'sccsdefs.h' out of 'common.h'
#	
#	Revision 7.0  89/10/04  11:19:02  ste_cm
#	BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
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
	rcsdefs.h\
	sccsdefs.h
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
