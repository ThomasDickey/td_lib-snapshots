# $Id: prog_top.mms,v 12.1 1994/08/21 23:13:48 tom Exp $
# MMS top-level reusable rules for PORTUNIX, and programs built with it.
#
# (originally named STANDARD_RULES.MMS)

.INCLUDE TD_LIB_ROOT:[SUPPORT]TD_LIB

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

RUN_TEST :	ALL
	set default [.test]
	mms $(MMSQUALIFIERS) $@ /Macro=(B="$(B)")
	set default [-]

bin.dir :
	create/directory [.bin]
