# $Id: Makefile,v 12.3 1994/07/02 19:20:55 tom Exp $
# make-file for TD_LIB common/test library

####### (Development) ##########################################################
TOP	= ../..
include ../support/td_lib.mk

####### (Standard Lists) #######################################################
DATA_FILES = \
	dummy.txt \
	dummy2.txt \
	dummy3.txt \
	dummy4.txt

MFILES = \
	td_defns.mk \
	td_rules.mk

SOURCES	= Makefile README $(DATA_FILES) $(MFILES)

####### (Standard Productions) #################################################
all  \
clean \
clobber \
destroy \
sources \
programs \
run_test ::	$(SOURCES)
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

clean ::			; $(RM) $(CLEAN) *.tmp *.pure*
destroy ::			; $(DESTROY)
lint.out:
lincnt.out:

####### (Details of Productions) ###############################################

$(SOURCES):		; $(GET) $@
