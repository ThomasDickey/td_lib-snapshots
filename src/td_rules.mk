# $Id: td_rules.mk,v 12.4 1994/07/12 17:06:32 tom Exp $
# Common make-rules for TD_LIB

####### (Standard Productions) #################################################
all\
run_test\
install:	$(SOURCES) $(ALL)
	@echo '** made '$@

clean:				; -$(RM) $(CLEAN)
clobber:	clean		; -$(RM) $(ALL)
destroy:			; $(DESTROY)
sources:	$(SOURCES)

####### (Details of Productions) ###############################################
$Z:	$(OBJS)
	$(RANLIB) $Z

.PRECIOUS: $Z

$(OBJS):	$(PTYPES_H)

####### (Development) ##########################################################
$(SOURCES):			; $(GET) $@
lint.out:	$(CSRC)		; $(DATE); $(LINT_EACH)
lincnt.out:	$(CSRC)		; $(DATE); lincnt $(CSRC) >>$@
tags:		$(CSRC)		; dotags $(CSRC)
