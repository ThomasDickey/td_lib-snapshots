# $Header: /users/source/archives/td_lib.vcs/support/RCS/td_lib.mms,v 2.0 1989/04/27 10:59:19 ste_cm Rel $
# MMS reusable rules for PORTUNIX, and programs built with it.
#
# $Log: td_lib.mms,v $
# Revision 2.0  1989/04/27 10:59:19  ste_cm
# BASELINE Mon Jun 12 09:07:53 EDT 1989
#
#	Revision 1.3  89/04/27  10:59:19  dickey
#	added TOOL_LIB (CM_TOOLS) variable
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
SYS_LIB	=	SYS$LIBRARY:VAXCRTL		! C runtime library
#
LIB_ARGS=	$(PORT_LIB)/LIB,$(TOOL_LIB)/LIB,$(SYS_LIB)/LIB
LIB_DEPS=	$(PORT_LIB).OLB,$(TOOL_LIB).OLB,$(SYS_LIB).OLB
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
