/*
 * Title:	fleaf.c (file/pathname leaf)
 * Author:	T.E.Dickey
 * Created:	27 Nov 1993
 *
 * Function:	Returns a pointer in 'path' to the leaf (i.e., filename)
 *		iff a pathname-delimiter precedes it.  If no delimiter is
 *		found, return null.
 *
 *		This is different from 'pathleaf()', which always returns
 *		a nonnull pointer, and properly handles the cases of trailing
 *		pathname delimiters.
 */
#define STR_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: fleaf.c,v 12.5 2003/04/25 23:21:52 tom Exp $")

char *	fleaf_delim(
	_AR1(char *, path))
	_DCL(char *, path)
{
#ifdef	SYS_UNIX
	return strrchr(path, '/');
#endif
#if	defined(vms) || defined(MSDOS)
	register char *s = path + strlen(path);
	while (s-- != path) {
		if (isSlash(*s))
			return s;
	}
	return 0;
#endif
}

char *	fleaf(
	_AR1(char *, path))
	_DCL(char *, path)
{
	register char *s = fleaf_delim(path);
	if (s != 0)
		s++;
	return s;
}
