# $Id: Makefile,v 10.0 1991/10/03 10:34:16 ste_cm Rel $
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
	deltree.h\
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
$L/$(LLIB).ln:	$(H_SRC) $(LLIB) ; - sh -c 'makellib -I. -L$L $(LLIB) $(llib)'
$(LLIB).i:	$(H_SRC) $(LLIB) ; /usr/lib/cpp -Dlint -I. $(LLIB) $@
