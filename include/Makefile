# $Header: /users/source/archives/td_lib.vcs/include/RCS/Makefile,v 3.0 1989/06/08 12:25:14 ste_cm Rel $
# make-file for CM_TOOLS common interface directory
#
# $Log: Makefile,v $
# Revision 3.0  1989/06/08 12:25:14  ste_cm
# BASELINE Mon Jun 19 13:22:44 EDT 1989
#
#	Revision 2.2  89/06/08  12:25:14  dickey
#	corrections to 'clean' & 'destroy' rules
#	
#	Revision 2.1  89/04/20  14:07:03  dickey
#	moved guts of llib-ltd to "common.h" to use for function-prototype on
#	VMS.
#	
#	Revision 2.0  89/03/21  16:17:11  ste_cm
#	BASELINE Thu Apr  6 09:38:25 EDT 1989
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
	sh -c 'for i in *;do case $$i in RCS);; *) rm -f $$i;;esac;done'
sources:	$(SOURCES)
lint.out:

####### (Details of Productions) ###############################################
#
$(SOURCES):		; $(GET) $@
$L/$(LLIB).ln:	$(H_SRC) $(LLIB) ; -makellib -L$L $(LLIB) $(llib)
