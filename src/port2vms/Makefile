# $Id: Makefile,v 12.4 1995/10/14 22:03:44 tom Exp $
# MAKE-file for functions supporting VAX/VMS porting

####### (Environment) ##########################################################
include ../td_defns.mk
include ../../support/td_lib.mk

####### (Standard Lists) #######################################################

# modules that can be compiled/tested on UNIX
CSRC	=\
	dir2path.c\
	name2vms.c\
	path2dir.c\
	path2vms.c\
	vms2name.c\
	vms_wild.c\
	vms_pthc.c\
	vms_rpth.c\
	zone2vms.c

OBJS	=\
	$Z(dir2path.o)\
	$Z(name2vms.o)\
	$Z(path2dir.o)\
	$Z(path2vms.o)\
	$Z(vms2name.o)\
	$Z(vms_wild.o)\
	$Z(vms_pthc.o)\
	$Z(vms_rpth.o)\
	$Z(zone2vms.o)

# modules that _only_ compile on vms:
VMS_SRC	=\
	rmsio.c\
	time2vms.c\
	uid2s.c\
	utimes.c\
	vms_dirs.c

SOURCES	= Makefile descrip.mms $(CSRC) $(VMS_SRC)

####### (Standard Productions) #################################################
include ../td_rules.mk

####### (Details of Productions) ###############################################
$(OBJS):	$(PTYPES_H) $I/port2vms.h
