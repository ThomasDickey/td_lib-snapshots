/*
 * Title:	setmtime.c (set file's mtime)
 * Author:	T.E.Dickey
 * Created:	20 May 1988
 * Modified:
 *		26 Mar 2002, if atime is zero, use current time.  Zero does not
 *                           work with cygwin.
 *		22 Apr 1995, added 'atime' parameter.
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

MODULE_ID("$Id: setmtime.c,v 12.8 2002/03/26 18:59:32 tom Exp $")

#if HAVE_UTIME_H
#include	<utime.h>
#else
struct	utimbuf { time_t actime, modtime; };
extern	int	utime(_arx(const char *,s) _ar1(const struct utimbuf *,p));
#endif

int	setmtime(
	_ARX(char *,	name)		/* name of file to touch */
	_ARX(time_t,	mtime)		/* modification time we want to leave */
	_AR1(time_t,	atime)		/* access time we want to leave */
		)
	_DCL(char *,	name)
	_DCL(time_t,	mtime)
	_DCL(time_t,	atime)
{
	struct utimbuf tp;

	tp.modtime = mtime;
	tp.actime  = (atime != 0) ? atime : time((time_t *)0);
	return (utime(name, &tp));
}
