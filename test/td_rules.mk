# $Id: td_rules.mk,v 12.6 1997/09/11 21:55:48 tom Exp $
# common rules for TD_LIB common/test library

ALL	= $(PROGS)

####### (Standard Productions) #################################################
all:		$(SOURCES)
clean:				; -$(RM) $(CLEAN) $(PROGS) *.tmp *.pure*
clobber:	clean		; -$(RM) $(ALL)
distclean:	clean
sources:	$(SOURCES)
programs:	$(PROGS)
run_test:	$(SOURCES) $(PROGS) ; $(RUN_TESTS)
lint.out:
