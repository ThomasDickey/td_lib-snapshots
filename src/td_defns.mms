# $Id: td_defns.mms,v 12.1 1994/07/02 13:23:15 tom Exp $
# common definitions MMS-files for TD_LIB routines	

####### (Command-line Options) #################################################
I	=	[--.INCLUDE]
L	=	[--.LIB]
A	=	$(L)TD_LIB.OLB
DEFINES	=
INCLUDES =	/Include=($(I))
CFLAGS =	$(DEFINES) $(INCLUDES) /LIST /OBJECT=$@
LINKFLAGS =	/MAP/CROSS_REFERENCE/EXEC=$@
C_RTL =		SYS$LIBRARY:VAXCRTL.OLB		! C runtime library
TEST_CC =	$(CC) $(CFLAGS) /DEFINE="TEST" $(MMS$SOURCE)

PTYPES_H =	$(I)PTYPES.H $(I)TD_LIB.H
