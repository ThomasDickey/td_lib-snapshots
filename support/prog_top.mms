# $Header: /users/source/archives/td_lib.vcs/support/RCS/prog_top.mms,v 5.0 1989/04/12 15:07:14 ste_cm Rel $
# MMS top-level reusable rules for PORTUNIX, and programs built with it.
#
# $Log: prog_top.mms,v $
# Revision 5.0  1989/04/12 15:07:14  ste_cm
# BASELINE Tue Jun 11 16:44:53 1991 -- apollo sr10.3
#
#	Revision 4.0  89/04/12  15:07:14  ste_cm
#	BASELINE Tue Aug 14 16:27:44 1990
#	
#	Revision 3.0  89/04/12  15:07:14  ste_cm
#	BASELINE Fri Oct  6 15:23:19 EDT 1989 -- support:ada_pits_000(rel3)
#	
#	Revision 2.0  89/04/12  15:07:14  ste_cm
#	BASELINE Mon Jun 12 09:07:53 EDT 1989
#	
#	Revision 1.1  89/04/12  15:07:14  dickey
#	Initial revision
#	
#
.INCLUDE PORTUNIX_ROOT:[SUPPORT]LIBRARY_RULES

MFILES	=-
	bin.dir
#
ALL -
CLEAN -
CLOBBER -
DESTROY -
INSTALL :	$(MFILES)
	set default [.src]
	mms $(MMSQUALIFIERS) $@ /Macro=(B="$(B)")
	set default [-]

PURGE	-
DEINSTALL :
	set default [.src]
	mms $(MMSQUALIFIERS) $@ /Macro=(B="$(B)")
	set default [-]

RUN_TESTS :	ALL
	set default [.test]
	mms $(MMSQUALIFIERS) $@ /Macro=(B="$(B)")
	set default [-]

#
bin.dir :	; create/directory [.bin]
