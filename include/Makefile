# $Id: Makefile,v 9.1 1991/09/09 07:38:16 dickey Exp $
# make-file for CM_TOOLS common interface directory
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
clean:			; rm -f *.out *.ln *.bak core
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
