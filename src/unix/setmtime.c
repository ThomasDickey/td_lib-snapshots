#ifndef	lint
static	char	Id[] = "$Id: setmtime.c,v 12.0 1991/10/03 08:43:11 ste_cm Rel $";
#endif

/*
 * Title:	setmtime.c (set file's mtime)
 * Author:	T.E.Dickey
 * Created:	20 May 1988
 * Modified:
 *		03 Oct 1991, convert to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		
 * Function:	Set a file's mtime from a given reference value.  This is used
 *		in applications such as archival-extraction where the underlying
 *		tool/system defaults to the current date.
 */

#include	"ptypes.h"

setmtime(
_ARX(char *,	name)			/* name of file to touch */
_AR1(time_t,	mtime)			/* time we want to leave */
	)
_DCL(char *,	name)
_DCL(time_t,	mtime)
{
#ifdef	SYSTEM5
struct { time_t x, y; } tp;
	tp.x = time((time_t *)0);
	tp.y = mtime;
	return (utime(name, &tp));
#else
time_t	tv[2];
	tv[0] = time((time_t *)0);
	tv[1] = mtime;
	return (utime(name, tv));
#endif
}
