# $Id: Makefile,v 12.12 1995/02/03 11:48:37 tom Exp $
# Top-level makefile for TD_LIB common library

####### (Development) ##########################################################
INSTALL_INC = ../install_inc
INSTALL_LIB = ../install_lib
INSTALL_MAN = ../install_man

SHELL	= /bin/sh
COPY	= cp
RM	= -rm -f
PUT	= $(RM) $@; $(COPY) $? $@
TOP	= ..
GET	= checkout
THIS	= td_lib

i	= include
I	= $(TOP)/include
L	= $(TOP)/lib

I_CFG	= $i/config.h
MDEFN	= support/td_lib.mk

CLEAN	= *.bak *.log *.out *.tst .nfs* core
PURE	= stamp-* config_h *.cache *.status $(I_CFG) $(MDEFN)
DESTROY	=$(SHELL) -c 'for i in *;do case $$i in RCS);; *) $(RM) $$i;;esac;done;exit 0'

####### (Standard Lists) #######################################################
CONFIG_H=\
	aclocal.m4\
	configure.in

SOURCES	=\
	Makefile\
	td_make.in\
	descrip.mms\
	README\
	COPYING\
	$(CONFIG_H)

MFILES	=\
	certify/Makefile\
	include/Makefile\
	src/Makefile\
	support/Makefile\
	test/Makefile

IT	=\
	$I/acl.h\
	$I/cmdch.h\
	$I/$(THIS).h\
	$I/td_curse.h\
	$I/td_qsort.h\
	$I/td_regex.h\
	$I/td_scomp.h\
	$I/td_sheet.h\
	$I/deltree.h\
	$I/dyn_str.h\
	$I/ptypes.h\
	$I/rcsdefs.h\
	$I/sccsdefs.h\
	$L/$(THIS).a

####### (Standard Productions) #################################################
all\
lintlib\
install::	lib $(I_CFG) $(MDEFN)

clean\
clobber \
destroy ::	td_make.in
	$(SHELL) -c "if test ! -f $(MDEFN);\
		then sed -es/@MAKE@/make/ td_make.in >$(MDEFN);\
		fi"
all\
clean\
clobber\
destroy\
run_test\
sources\
lint.out ::	$(MFILES)
	cd support;	$(MAKE) $@
	cd certify;	$(MAKE) $@
	cd include;	$(MAKE) $@
	cd src;		$(MAKE) $@
	cd test;	$(MAKE) $@

lintlib ::	$(MFILES)
	cd include;	$(MAKE) $@

all\
sources ::	$(SOURCES)

clean \
clobber ::			; $(RM) $(CLEAN)
clobber \
destroy ::			; $(RM) -r lib
destroy ::			; cd support; $(DESTROY)
destroy ::			; $(DESTROY)

distclean:	clobber		; $(RM) $(PURE)

install::	all $(IT)
deinstall::			; $(RM) $(IT)

####### (Details of Productions) ###############################################
$(MFILES)\
$(SOURCES):				; $(GET) $@
lib:					; mkdir $@

$I/acl.h:		$i/acl.h	; $(PUT)
$I/cmdch.h:		$i/cmdch.h	; $(PUT)
$I/td_curse.h:		$i/td_curse.h	; $(PUT)
$I/td_regex.h:		$i/td_regex.h	; $(PUT)
$I/td_qsort.h:		$i/td_qsort.h	; $(PUT)
$I/td_scomp.h:		$i/td_scomp.h	; $(PUT)
$I/td_sheet.h:		$i/td_sheet.h	; $(PUT)
$I/$(THIS).h:		$i/$(THIS).h	; $(PUT)
$I/deltree.h:		$i/deltree.h	; $(PUT)
$I/dyn_str.h:		$i/dyn_str.h	; $(PUT)
$I/ptypes.h:		$i/ptypes.h	; $(PUT)
$I/rcsdefs.h:		$i/rcsdefs.h	; $(PUT)
$I/sccsdefs.h:		$i/sccsdefs.h	; $(PUT)

$L/$(THIS).a:	lib/$(THIS).a		; $(PUT); ranlib $@

configure:	$(CONFIG_H)		; autoconf

# config.status might not change config.h
$(I_CFG): stamp-h
stamp-h: config.status
	$(SHELL) -c ./config.status

#? Makefile: Makefile.in config.status
#? 	./config.status

config.status: configure
	$(SHELL) -c "./config.status --recheck"
