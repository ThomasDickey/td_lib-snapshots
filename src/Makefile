# $Id: Makefile,v 12.9 1994/07/02 13:24:52 tom Exp $
# MAKE-file for miscellaneous library routines

####### (Environment) ##########################################################
TOP	= ../..
include ../support/td_lib.mk

####### (Standard Lists) #######################################################
MFILES	= \
	cm_funcs/Makefile \
	compare/Makefile \
	curses/Makefile \
	memory/Makefile \
	pathname/Makefile \
	port2dos/Makefile \
	port2vms/Makefile \
	process/Makefile \
	sheet/Makefile \
	string/Makefile \
	unix/Makefile

SOURCES	= \
	td_defns.mk	td_defns.mms \
	td_rules.mk	td_rules.mms

####### (Standard Productions) #################################################
all \
clean \
clobber \
destroy \
run_test\
sources \
install ::	$(SOURCES)
	cd cm_funcs;	$(MAKE) $@
	cd compare;	$(MAKE) $@
	cd curses;	$(MAKE) $@
	cd memory;	$(MAKE) $@
	cd pathname;	$(MAKE) $@
	cd port2dos;	$(MAKE) $@
	cd port2vms;	$(MAKE) $@
	cd process;	$(MAKE) $@
	cd sheet;	$(MAKE) $@
	cd string;	$(MAKE) $@
	cd unix;	$(MAKE) $@

clean ::			; $(RM) $(CLEAN)
clobber ::	clean		; $(RM) $(ALL)
destroy ::			; $(DESTROY)
