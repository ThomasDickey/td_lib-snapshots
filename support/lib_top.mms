# $Id: lib_top.mms,v 8.0 1993/04/27 16:43:01 ste_cm Rel $
# MMS top-level reusable rules for PORTUNIX, and programs built with it.
#
.INCLUDE PORTUNIX_ROOT:[SUPPORT]LIBRARY_RULES

MFILES	=-
	lib.dir
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

RUN_TEST :	ALL
	set default [.test]
	mms $(MMSQUALIFIERS) $@ /Macro=(B="$(B)")
	set default [-]

#
lib.dir :	; create/directory [.lib]
