# $Id: lib_top.mms,v 12.1 1994/08/21 23:14:02 tom Exp $
# MMS top-level reusable rules for TD_LIB, and programs built with it.
#
# (originally named TOP_LIB_RULES.MMS)

.INCLUDE TD_LIB_ROOT:[SUPPORT]TD_LIB

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
