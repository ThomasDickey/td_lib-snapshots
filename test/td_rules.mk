# $Id: td_rules.mk,v 12.3 1994/07/05 00:04:54 tom Exp $
# common rules for TD_LIB common/test library

ALL	= $(PROGS)

####### (Standard Productions) #################################################
all:		$(SOURCES)
clean:				; -$(RM) $(CLEAN) *.tmp *.pure*
clobber:	clean		; -$(RM) $(ALL)
destroy:			; $(DESTROY)
sources:	$(SOURCES)
programs:	$(PROGS)
run_test:	$(SOURCES) $(PROGS) ; $(RUN_TESTS)
lint.out:
lincnt.out:

####### (Details of Productions) ###############################################
$(SOURCES):		; $(GET) $@