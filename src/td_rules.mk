# $Id: td_rules.mk,v 12.5 1997/09/09 22:05:06 tom Exp $
# Common make-rules for TD_LIB

####### (Standard Productions) #################################################
all\
run_test\
install:	$(SOURCES) $(ALL)
	@echo '** made '$@

clean:				; -$(RM) $(CLEAN)
clobber:	clean		; -$(RM) $(ALL)
destroy:			; $(DESTROY)
distclean:	clean		; -$(RM) makefile
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
