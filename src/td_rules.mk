# $Id: td_rules.mk,v 12.8 2010/07/05 10:04:22 tom Exp $
# Common make-rules for TD_LIB

####### (Standard Productions) #################################################
all\
check\
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
