#ifndef	NO_IDENT
static	char	Id[] = "$Id: memmove.c,v 12.1 1994/07/01 23:57:13 tom Exp $";
#endif

/*
 * Title:	memmove.c
 * Author:	T.E.Dickey
 * Created:	17 Jul 1992, for 'field_of.c'
 * Modified:
 *		01 Jul 1994, split out from 'field_of.c'
 *
 * Function:	substitute for the commonly available function 'memmove'
 */
#include "ptypes.h"

#if	!HAVE_MEMMOVE
char *	memmove(
	_ARX(char *,	s1)
	_ARX(char *,	s2)
	_AR1(size_t,	n)
		)
	_DCL(char *,	s1)
	_DCL(char *,	s2)
	_DCL(size_t,	n)
{
	if (n != 0) {
		if ((s1+n > s2) && (s2+n > s1)) {
			static	char	*buffer;
			static	unsigned length;
			register int	j;
			if (length < n)
				buffer = doalloc(buffer, length = n);
			for (j = 0; j < n; j++)
				buffer[j] = s2[j];
			s2 = buffer;
		}
		while (n-- != 0)
			s1[n] = s2[n];
	}
	return s1;
}
#endif	/* HAVE_MEMMOVE */
