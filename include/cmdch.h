/* $Header: /users/source/archives/td_lib.vcs/include/RCS/cmdch.h,v 8.0 1988/04/27 08:42:10 ste_cm Rel $ */

#define	CTL(c)		('c'&037)
#define	ARO(c)		(CTL(c)|0200)

#define	ARO_UP		ARO(U)
#define	ARO_DOWN	ARO(D)
#define	ARO_LEFT	ARO(L)
#define	ARO_RIGHT	ARO(R)
