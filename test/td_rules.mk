# $Id: td_rules.mk,v 12.4 1997/09/10 00:28:31 tom Exp $
# common rules for TD_LIB common/test library

ALL	= $(PROGS)

####### (Standard Productions) #################################################
all:		$(SOURCES)
clean:				; -$(RM) $(CLEAN) *.tmp *.pure*
clobber:	clean		; -$(RM) $(ALL)
distclean:	clean
sources:	$(SOURCES)
programs:	$(PROGS)
run_test:	$(SOURCES) $(PROGS) ; $(RUN_TESTS)
lint.out:
lincnt.out:

####### (Details of Productions) ###############################################
$(SOURCES):		; $(GET) $@
