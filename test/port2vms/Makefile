# $Id: Makefile,v 12.3 1995/01/29 00:47:37 tom Exp $
# make-file for TD_LIB common/test library

####### (Development) ##########################################################
THIS	= port2vms
include ../td_defns.mk
include ../../support/td_lib.mk

####### (Standard Lists) #######################################################
REF_FILES =\
	dir2path.ref\
	name2vms.ref\
	path2dir.ref\
	path2vms.ref\
	vms2name.ref\
	vms_wild.ref\
	vms_pthc.ref\
	vms_rpth.ref\
	zone2vms.ref

SCRIPTS =\
	run_test.sh\
	dir2path.sh\
	name2vms.sh\
	path2dir.sh\
	path2vms.sh\
	vms2name.sh\
	vms_wild.sh\
	vms_pthc.sh\
	vms_rpth.sh\
	zone2vms.sh

SOURCES	= Makefile descrip.mms README $(SCRIPTS) $(REF_FILES)

PROGS	=\
	dir2path\
	name2vms\
	path2dir\
	path2vms\
	vms2name\
	vms_wild\
	vms_pthc\
	vms_rpth\
	zone2vms

ALL	= $(PROGS)

####### (Standard Productions) #################################################
include ../td_rules.mk

####### (Details of Productions) ###############################################

# rules to make SunOs 'make' "work" with VPATH

dir2path.c\
name2vms.c\
path2dir.c\
path2vms.c\
vms2name.c\
vms_wild.c\
vms_pthc.c\
vms_rpth.c\
zone2vms.c:
