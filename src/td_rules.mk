# $Id: td_rules.mk,v 12.7 1997/09/11 01:11:52 tom Exp $
# Common make-rules for TD_LIB

####### (Standard Productions) #################################################
all\
run_test\
install:	$(SOURCES) $(ALL)
	@echo '** made '$@

clean:				; -$(RM) $(CLEAN)
clobber:	clean		; -$(RM) $(ALL)
distclean:	clean
sources:	$(SOURCES)

####### (Details of Productions) ###############################################

.PRECIOUS: $Z

$(OBJS):	$(PTYPES_H)

####### (Development) ##########################################################
lint.out:	$(CSRC)		; $(DATE); $(LINT_EACH)
tags:		$(CSRC)		; dotags $(CSRC)
