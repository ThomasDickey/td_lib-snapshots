# $Id: td_rules.mms,v 12.2 1994/07/02 15:12:35 tom Exp $
# common rules MMS-file for TD_LIB functions	

LIBRARIES =	$(A)($(LIBRARYMODULES)) -	! project library
		$(C_RTL)

####### (Standard Productions) #################################################
.FIRST :
	@ WRITE SYS$OUTPUT "** support library make: $@"
.LAST :
	- purge *.obj
	- remove -f *.dia;*
	@ WRITE SYS$OUTPUT "** done"

ALL	= $(A)

ALL :		$(ALL)
		@ WRITE SYS$OUTPUT "** no action for $@"
CLEAN :
		- purge
		- remove -vf *.LIS;* *.LOG;* TEST_*.*;*
		- remove -vf *.OUT;* $(L)*.A;* $(L)*.LN;* $(I)*.LN;*
CLOBBER :	CLEAN
		- remove -vf *.OBJ;* $(ALL)
DESTROY :	
		- remove -vf *.*;*
INSTALL :	
		@ WRITE SYS$OUTPUT "** no action for $@"
