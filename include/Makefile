# $Id: Makefile,v 10.2 1992/02/03 10:23:28 dickey Exp $
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
	sccsdefs.h\
	spreadsheet.h

L_SRC	=\
	llib-ltd

SOURCES	= Makefile $(H_SRC) $(L_SRC)

IT	= $L/$(LLIB).ln
ALL	=

####### (Standard Productions) #################################################
all:		$(SOURCES) $(ALL)
clean:			; rm -f *.bak *.log *.out *.ln core
clobber:	clean	; rm -f $(ALL) $(IT)
destroy:
	sh -c 'for i in *;do case $$i in RCS);; *) rm -f $$i;;esac;done;exit 0'
sources:	$(SOURCES)
lintlib:	$(IT)
lint.out:
lincnt.out:	$(H_SRC)	; lincnt $(H_SRC) >$@

####### (Details of Productions) ###############################################
#
$(SOURCES):		; $(GET) $@
$L/$(LLIB).ln:	$(H_SRC) $(LLIB) ; - sh -c 'makellib -I. -L$L $(LLIB) $(llib)'
$(LLIB).i:	$(H_SRC) $(LLIB) ; /usr/lib/cpp -Dlint -I. $(LLIB) $@
