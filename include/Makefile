# $Id: Makefile,v 11.3 1992/11/19 15:03:40 dickey Exp $
# make-file for CM_TOOLS common interface directory

TOP	= ../..
include $(TOP)/cm_library/support/cm_library.mk

####### (Command-line Options) #################################################
llib	= td
LLIB	= llib-l$(llib)

####### (Standard Lists) #######################################################
H_SRC	=\
	acl.h \
	cmdch.h\
	cm_curses.h\
	cm_qsort.h\
	cm_scomp.h\
	common.h\
	deltree.h\
	dyn_string.h\
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
clean:				; rm -f $(CLEAN) *.ln
clobber:	clean		; rm -f $(ALL) $(IT)
destroy:			; $(DESTROY)
sources:	$(SOURCES)
lintlib:	$(IT)
lint.out:	$(IT)
lincnt.out:	$(H_SRC)	; lincnt $(H_SRC) >$@

####### (Details of Productions) ###############################################
#
$(SOURCES):			; $(GET) $@
$L/$(LLIB).ln:	$(SOURCES)	; - sh -c 'makellib -I. -L$L $(LLIB) $(llib)'
$(LLIB).i:	$(SOURCES)	; /usr/lib/cpp -Dlint -I. $(LLIB) $@
