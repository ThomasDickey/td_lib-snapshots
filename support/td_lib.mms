# $Id: td_lib.mms,v 3.0 1989/08/28 09:22:59 ste_cm Rel $
# MMS reusable rules for PORTUNIX, and programs built with it.
#
# $Log: td_lib.mms,v $
# Revision 3.0  1989/08/28 09:22:59  ste_cm
# BASELINE Fri Oct  6 15:23:19 EDT 1989 -- support:ada_pits_000(rel3)
#
#	Revision 2.1  89/08/28  09:22:59  dickey
#	changed LIB_ARGS/LIB_DEPS around so I can link with shareable C libs
#	
#	Revision 1.2  89/04/25  09:10:03  dickey
#	integration with CM_TOOLS's include-files
#	
#	Revision 1.1  89/04/13  10:52:07  dickey
#	Initial revision
#	
#
# Define standard locations in the tree:
I = PORTUNIX_ROOT:[INTERFACE]	! include-files from PORTUNIX
J = CM_TOOLS_ROOT:[INTERFACE]	! include-files from CM_TOOLS
B = COM:			! program-binaries for installation
#
# Override default for CFLAGS and LINKFLAGS to generate useful information
# for debugging:
INCLUDES=	$(I),$(J)
CFLAGS =	/Diagnostics /Listing /Debug /OBJECT=$@ /Include=($(INCLUDES))
LINKFLAGS =	/MAP/CROSS_REFERENCE/EXEC=$(MMS$TARGET_NAME).EXE
#
# Define a macro useful for generating test-programs for library modules:
TEST_CC =	$(CC)/OBJECT=$(MMS$TARGET_NAME)/DEFINE="TEST" $(MMS$SOURCE)
#
# Define macros useful for specifying dependencies and arguments for the
# libraries used here and by simple applications above:
PORT_LIB=	PORTUNIX_ROOT:[LIB]PORT
TOOL_LIB=	CM_TOOLS_ROOT:[LIB]COMMON
#
OPT_ARGS=	PORTUNIX_ROOT:[SUPPORT]LINK_OPTIONS.OPT
OPT_DEPS=	$(OPT_ARGS),-
		SYS$LIBRARY:VAXCRTLG.EXE
#
LIB_ARGS=	$(PORT_LIB)/LIB,$(TOOL_LIB)/LIB,$(OPT_ARGS)/OPT
LIB_DEPS=	$(PORT_LIB).OLB,$(TOOL_LIB).OLB,$(OPT_DEPS)
#
# If the build succeeds, clean up the diagnostic files, purge stuff we don't
# really need:
.LAST :
	- remove -f *.dia;*
	@- if F$SEARCH("*.lis") .nes. "" then purge *.lis
	@- if F$SEARCH("*.map") .nes. "" then purge *.map
	@ WRITE SYS$OUTPUT "** done"
#
# Override rule for link so we can include libraries:
.OBJ.EXE :
	$(LINK) $(LINKFLAGS) $(MMS$TARGET_NAME)$(OBJ_ARGS),$(LIB_ARGS)
