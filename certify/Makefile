# $Id: Makefile,v 11.2 1992/10/16 12:23:06 dickey Exp $

TOP	= ../..
include $(TOP)/cm_library/support/cm_library.mk

####### (Standard Lists) #######################################################
SOURCES	=\
	README\
	config_sheet.txt\
	release_notes.txt\
	reusable_parts.txt

####### (Standard Productions) #################################################
all:		$(SOURCES) $(ALL)
clean:				; rm -f $(CLEAN)
clobber:	clean		; rm -f $(ALL)
destroy:			; $(DESTROY)
sources:	$(SOURCES)
lint.out\
lintlib\
run_tests\
install\
deinstall:

####### (Details of Productions) ###############################################
$(SOURCES):			; $(GET) $@
