# $Id: prog_top.mms,v 7.1 1992/11/04 07:19:04 dickey Exp $
# MMS top-level reusable rules for PORTUNIX, and programs built with it.
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
