# $Header: /users/source/archives/td_lib.vcs/support/RCS/lib_top.mms,v 7.0 1989/04/25 08:27:26 ste_cm Rel $
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
