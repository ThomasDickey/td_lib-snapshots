# $Id: Makefile,v 12.2 1994/11/06 21:55:56 tom Exp $

TOP	= ../..
include $(TOP)/td_lib/support/td_lib.mk

####### (Standard Lists) #######################################################
SOURCES	=\
	README\
	config.txt\
	release.txt\
	reusable.txt

ALL =

####### (Standard Productions) #################################################
all:		$(SOURCES) $(ALL)
clean:				; -$(RM) $(CLEAN)
clobber:	clean		; -$(RM) $(ALL)
destroy:			; $(DESTROY)
sources:	$(SOURCES)
lint.out\
lintlib\
run_test\
install\
deinstall:

####### (Details of Productions) ###############################################
$(SOURCES):			; $(GET) $@
