# $Id: Makefile,v 12.4 1994/07/12 01:00:30 tom Exp $
# make-file for CM_TOOLS common interface directory

THIS	= td
TOP	= ../..
include $(TOP)/td_lib/support/td_lib.mk

####### (Command-line Options) #################################################
llib	= td
LLIB	= llib-l$(llib)

####### (Standard Lists) #######################################################
H_SRC	=\
	$(THIS)_lib.h\
	acl.h \
	deltree.h\
	dyn_str.h\
	ptypes.h\
	rcsdefs.h\
	sccsdefs.h\
	td_ext.h\
	td_curse.h\
	td_local.h\
	td_qsort.h\
	td_scomp.h\
	td_sheet.h

L_SRC	=\
	llib-l$(THIS)

SOURCES	= Makefile $(H_SRC) $(L_SRC)

IT	= $L/$(LLIB).ln
ALL	=

####### (Standard Productions) #################################################
all\
run_test\
install:	$(SOURCES) $(ALL)
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
$L/$(LLIB).ln:	$(SOURCES)	; - sh -c 'makellib -I. $(CPP_OPTS) -L$L $(LLIB) $(llib)'
$(LLIB).i:	$(SOURCES)	; /usr/lib/cpp -Dlint -I. $(LLIB) $@
