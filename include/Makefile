# $Header: /users/source/archives/td_lib.vcs/include/RCS/Makefile,v 2.0 1989/03/21 16:17:11 ste_cm Exp $
# make-file for CM_TOOLS common interface directory
#
# $Log: Makefile,v $
# Revision 2.0  1989/03/21 16:17:11  ste_cm
# BASELINE Thu Apr  6 09:38:25 EDT 1989
#
#	Revision 1.1  89/03/21  16:17:11  dickey
#	Initial revision
#	
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
	ptypes.h\
	rcsdefs.h
L_SRC	=\
	llib-ltd
SOURCES	= Makefile $(H_SRC) $(L_SRC)

ALL	= $L/$(LLIB).ln

####### (Standard Productions) #################################################
all:	$(ALL)
clean:			; rm -f *.out
clobber:	clean	; rm -f $(ALL)
destroy:		; rm -f *
sources:	$(SOURCES)
lint.out:

####### (Details of Productions) ###############################################
#
$(SOURCES):		; $(GET) $@
$L/$(LLIB).ln:	ptypes.h rcsdefs.h $(LLIB) ; -makellib -L$L $(LLIB) $(llib)
