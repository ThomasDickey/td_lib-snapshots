# $Id: prog_top.mms,v 7.0 1991/09/12 08:16:06 ste_cm Rel $
# MMS top-level reusable rules for PORTUNIX, and programs built with it.
#
# $Log: prog_top.mms,v $
# Revision 7.0  1991/09/12 08:16:06  ste_cm
# BASELINE Thu Jul 16 16:14:02 1992 -- CM_TOOLS #11
#
#	Revision 6.0  91/09/12  08:16:06  ste_cm
#	BASELINE Mon Oct 21 13:09:39 1991 -- adapt to CM_TOOLS #10
#	
#	Revision 5.1  91/09/12  08:16:06  dickey
#	mods to 'clobber' and 'destroy' rules
#	
#	Revision 3.0  89/04/12  15:07:14  ste_cm
#	BASELINE Fri Oct  6 15:23:19 EDT 1989 -- support:ada_pits_000(rel3)
#	
.INCLUDE PORTUNIX_ROOT:[SUPPORT]LIBRARY_RULES

MFILES	=-
	[.src]descrip.mms
#
ALL -
INSTALL ::	bin.dir
	@ write sys$output "making $@"

ALL -
CLEAN -
CLOBBER -
DESTROY -
INSTALL ::
	set default [.src]
	mms $(MMSQUALIFIERS) $@ /Macro=(B="$(B)")
	set default [-]

CLOBBER -
DESTROY ::
	- remove -rf *.out;* *.log;* bin.dir

DESTROY ::
	- remove -f *.*;*

DEINSTALL :
	set default [.src]
	mms $(MMSQUALIFIERS) $@ /Macro=(B="$(B)")
	set default [-]

RUN_TESTS :	ALL
	set default [.test]
	mms $(MMSQUALIFIERS) $@ /Macro=(B="$(B)")
	set default [-]

bin.dir :
	create/directory [.bin]
