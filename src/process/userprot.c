#ifndef	lint
static	char	Id[] = "$Id: userprot.c,v 9.0 1991/05/15 09:57:23 ste_cm Rel $";
#endif

/*
 * Title:	userprot.c (set protection for setuid program's caller)
 * Author:	T.E.Dickey
 * Created:	30 Aug 1988
 * $Log: userprot.c,v $
 * Revision 9.0  1991/05/15 09:57:23  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  09:57:23  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  88/09/13  09:54:18  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  88/09/13  09:54:18  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  88/09/13  09:54:18  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  88/09/13  09:54:18  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  88/09/13  09:54:18  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/09/13  09:54:18  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/09/13  09:54:18  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.3  88/09/13  09:54:18  dickey
 *		sccs2rcs keywords
 *		
 *		13 Sep 1988, rewrote using 'for_user()'
 *
 * Function:	For a set-uid program, sets protection (and modification time)
 *		for the user.  By forking, we can reset our uid; this is
 *		faster than an exec.
 *
 * Returns:	-1 if an error was found; sets 'errno'.
 */

#include	"ptypes.h"

static	char	*upr_name;
static	int	upr_mode;
static	time_t	upr_time;

static	
upr_func() { 
	if (chmod(upr_name, upr_mode) >= 0)
		(void)setmtime(upr_name, upr_time);
}

userprot(name, mode, mtime)
char	*name;
int	mode;
time_t	mtime;
{
	upr_name = name;
	upr_mode = mode;
	upr_time = mtime;
	return (for_user(upr_func));
}
