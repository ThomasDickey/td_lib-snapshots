#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/curses/RCS/beep.c,v 8.0 1988/08/11 09:34:54 ste_cm Rel $";
#endif	lint

/*
 * Title:	beep.c (sound audible alarm)
 * Author:	T.E.Dickey
 * Created:	24 Mar (from 'ded.c')
 * $Log: beep.c,v $
 * Revision 8.0  1988/08/11 09:34:54  ste_cm
 * BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *
 *		Revision 7.0  88/08/11  09:34:54  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  88/08/11  09:34:54  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  88/08/11  09:34:54  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  88/08/11  09:34:54  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/08/11  09:34:54  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/08/11  09:34:54  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.3  88/08/11  09:34:54  dickey
 *		sccs2rcs keywords
 *		
 *		11 Aug 1988, since we use this only for curses applications,
 *			     don't use it in system5!
 */

#include	"ptypes.h"

#ifndef	SYSTEM5
beep()
{
	blip('\007');
}
#endif	SYSTEM5
