# $Header: /users/source/archives/td_lib.vcs/support/RCS/td_lib.mms,v 1.1 1989/04/13 10:52:07 dickey Exp $
# MMS reusable rules for PORTUNIX, and programs built with it.
#
# $Log: td_lib.mms,v $
# Revision 1.1  1989/04/13 10:52:07  dickey
# Initial revision
#
#
# Define standard locations in the tree:
I = PORTUNIX_ROOT:[INTERFACE]	! include-files from PORTUNIX
B = COM:			! program-binaries for installation
#
# Override default for CFLAGS and LINKFLAGS to generate useful information
# for debugging:
INCLUDES=	$(I)
CFLAGS =	/Diagnostics /Listing /Debug /OBJECT=$@ /Include=($(INCLUDES))
LINKFLAGS =	/MAP/CROSS_REFERENCE/EXEC=$(MMS$TARGET_NAME).EXE
#
# Define a macro useful for generating test-programs for library modules:
TEST_CC =	$(CC)/OBJECT=$(MMS$TARGET_NAME)/DEFINE="TEST" $(MMS$SOURCE)
#
# Define macros useful for specifying dependencies and arguments for the
# libraries used here and by simple applications above:
PORT_LIB=	PORTUNIX_ROOT:[LIB]PORT
SYS_LIB	=	SYS$LIBRARY:VAXCRTL		! C runtime library
#
LIB_ARGS=	$(PORT_LIB)/LIB,$(SYS_LIB)/LIB
LIB_DEPS=	$(PORT_LIB).OLB,$(SYS_LIB).OLB
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
