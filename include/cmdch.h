/* $Id: cmdch.h,v 12.0 1993/04/26 15:27:49 ste_cm Rel $ */

/*
 * definitions for 'cmdch()'
 */

#define	CTL(c)		(c&037)
#define	ARO(c)		(CTL(c)|0200)

#define	ARO_UP		ARO('U')
#define	ARO_DOWN	ARO('D')
#define	ARO_LEFT	ARO('L')
#define	ARO_RIGHT	ARO('R')
