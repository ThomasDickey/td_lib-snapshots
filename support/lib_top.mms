# $Id: lib_top.mms,v 7.1 1992/11/04 07:19:22 dickey Exp $
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

RUN_TESTS :	ALL
	set default [.test]
	mms $(MMSQUALIFIERS) $@ /Macro=(B="$(B)")
	set default [-]

#
lib.dir :	; create/directory [.lib]
