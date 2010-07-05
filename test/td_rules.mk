# $Id: td_rules.mk,v 12.7 2010/07/05 10:04:22 tom Exp $
# common rules for TD_LIB common/test library

ALL	= $(PROGS)

####### (Standard Productions) #################################################
all:		$(SOURCES)
clean:				; -$(RM) $(CLEAN) $(PROGS) *.tmp *.pure*
clobber:	clean		; -$(RM) $(ALL)
distclean:	clean
sources:	$(SOURCES)
programs:	$(PROGS)
check:		$(SOURCES) $(PROGS) ; $(RUN_TESTS)
lint.out:
