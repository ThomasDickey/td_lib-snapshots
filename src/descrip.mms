# $Id: descrip.mms,v 12.5 1994/08/21 23:41:10 tom Exp $
# MMS rules for [.TD_LIB.SRC]

.INCLUDE [-.SUPPORT]TD_LIB

ALL -
CLEAN -
CLOBBER -
DESTROY -
INSTALL -
PURGE -
DEINSTALL :
	set default [.cm_funcs]
	mms $(MMSQUALIFIERS) $@
	set default [-.compare]
	mms $(MMSQUALIFIERS) $@
	set default [-.curses]
	mms $(MMSQUALIFIERS) $@
	set default [-.memory]
	mms $(MMSQUALIFIERS) $@
	set default [-.pathname]
	mms $(MMSQUALIFIERS) $@
	set default [-.port2vms]
	mms $(MMSQUALIFIERS) $@
	set default [-.process]
	mms $(MMSQUALIFIERS) $@
	set default [-.sheet]
	mms $(MMSQUALIFIERS) $@
	set default [-.string]
	mms $(MMSQUALIFIERS) $@
	set default [-]
