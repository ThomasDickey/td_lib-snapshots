# $Id: td_lib.mms,v 7.1 1992/11/04 07:18:34 dickey Exp $
# MMS reusable rules for PORTUNIX, and programs built with it.
#
# Define standard locations in the tree:
I = PORTUNIX_ROOT:[INTERFACE]	! include-files from PORTUNIX
J = CM_LIBRARY_ROOT:[INTERFACE]	! include-files from CM_LIBRARY
B = COM:			! program-binaries for installation
#
# Override default for CFLAGS and LINKFLAGS to generate useful information
# for debugging:
INCLUDES=	$(I),$(J)
CFLAGS =	/Diagnostics /Listing /Debug-
		/OBJECT=$@ /Include=($(INCLUDES)) /G_FLOAT
LINKFLAGS =	/MAP/CROSS_REFERENCE/EXEC=$(MMS$TARGET_NAME).EXE
#
# Define a macro useful for generating test-programs for library modules:
TEST_CC =	$(CC)/OBJECT=$(MMS$TARGET_NAME)/DEFINE="TEST" $(MMS$SOURCE)
#
# Define macros useful for specifying dependencies and arguments for the
# libraries used here and by simple applications above:
PORT_LIB=	PORTUNIX_ROOT:[LIB]PORT
TOOL_LIB=	CM_LIBRARY_ROOT:[LIB]COMMON
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
