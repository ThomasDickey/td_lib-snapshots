#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: setmtime.c,v 12.3 1993/11/27 14:56:30 dickey Exp $";
#endif

/*
 * Title:	setmtime.c (set file's mtime)
 * Author:	T.E.Dickey
 * Created:	20 May 1988
 * Modified:
 *		29 Oct 1993, ifdef-ident, port to HP/UX
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, convert to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		
 * Function:	Set a file's mtime from a given reference value.  This is used
 *		in applications such as archival-extraction where the underlying
 *		tool/system defaults to the current date.
 */

#include	"ptypes.h"
#include	<time.h>

int	setmtime(
	_ARX(char *,	name)			/* name of file to touch */
	_AR1(time_t,	mtime)			/* time we want to leave */
		)
	_DCL(char *,	name)
	_DCL(time_t,	mtime)
{
#ifdef	SYSTEM5
#if	defined(__hpux)
	struct utimbuf tp;
#else
	struct { time_t actime, modtime; } tp;
#endif
	tp.actime  = time((time_t *)0);
	tp.modtime = mtime;
	return (utime(name, &tp));
#else
	extern	int	utime(_arx(char *,s) _ar1(time_t *,p));
	time_t	tv[2];
	tv[0] = time((time_t *)0);
	tv[1] = mtime;
	return (utime(name, tv));
#endif
}
