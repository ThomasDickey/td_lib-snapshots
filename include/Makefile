# $Id: Makefile,v 12.7 1994/08/21 18:03:03 tom Exp $
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
	cmv_defs.h \
	deltree.h \
	dyn_str.h \
	ptypes.h\
	rcsdefs.h \
	sccsdefs.h \
	port2vms.h \
	td_ext.h \
	td_curse.h \
	td_local.h \
	td_qsort.h \
	td_scomp.h \
	td_sheet.h \
	vms_dirs.h

L_SRC	=\
	llib-l$(THIS)

SOURCES	= Makefile $(H_SRC) $(L_SRC)

IT	= $L/$(LLIB).ln
ALL	=

####### (Standard Productions) #################################################
all\
run_test\
install:	$(SOURCES) $(ALL)
clean:				; $(RM) $(CLEAN) *.ln
clobber:	clean		; $(RM) $(ALL) $(IT)
destroy:			; $(DESTROY)
sources:	$(SOURCES)
lintlib:	$(IT)
lint.out:	$(IT)

####### (Details of Productions) ###############################################
$(SOURCES):			; $(GET) $@
$L/$(LLIB).ln:	$(SOURCES)	; - sh -c 'makellib $(CPP_OPTS) -L$L $(LLIB) $(llib)'
$(LLIB).i:	$(SOURCES)	; /usr/lib/cpp -Dlint $(CPP_OPTS) $(LLIB) $@
