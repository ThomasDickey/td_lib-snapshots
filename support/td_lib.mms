# $Id: td_lib.mms,v 12.1 1994/08/21 23:02:07 tom Exp $
# MMS reusable rules for TD_LIB, and programs built with it.
#
# (originally named LIBRARY_RULES.MMS)

# Define standard locations in the tree:
I = TD_LIB_ROOT:[INCLUDE]	! include-files from TD_LIB
B = COM:			! program-binaries for installation
#
# Override default for CFLAGS and LINKFLAGS to generate useful information
# for debugging:
INCLUDES=	$(I)
CFLAGS =	/Diagnostics /Listing /Debug-
		/OBJECT=$@ /Include=($(INCLUDES)) /G_FLOAT
LINKFLAGS =	/MAP/CROSS_REFERENCE/EXEC=$(MMS$TARGET_NAME).EXE
#
# Define a macro useful for generating test-programs for library modules:
TEST_CC =	$(CC)/OBJECT=$(MMS$TARGET_NAME)/DEFINE="TEST" $(MMS$SOURCE)
#
# Define macros useful for specifying dependencies and arguments for the
# libraries used here and by simple applications above:
TOOL_LIB=	TD_LIB_ROOT:[LIB]TD_LIB
#
OPT_ARGS=	TD_LIB_ROOT:[SUPPORT]LINK_OPTIONS.OPT
OPT_DEPS=	$(OPT_ARGS),-
		SYS$LIBRARY:VAXCRTLG.EXE
#
LIB_ARGS=	$(TOOL_LIB)/LIB,$(OPT_ARGS)/OPT
LIB_DEPS=	$(TOOL_LIB).OLB,$(OPT_DEPS)
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
