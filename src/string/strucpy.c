#ifndef	lint
static	char	Id[] = "$Id: strucpy.c,v 12.1 1993/09/21 18:54:03 dickey Exp $";
#endif

/*
 * Title:	strucpy.c (string-copy, uppercase)
 * Author:	T.E.Dickey
 * Created:	06 Oct 1988
 * Modified:
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		27 Jun 1990, added procedure 'strlcpy()' to this module. 
 *			     Altered 'strucpy()' to return its destination-arg
 *			     a la 'strcpy()'
 *		
 * Function:	Copy a string, converting its case (upper/lower). The source
 *		and destination arguments may be the same.
 */

#define CHR_PTYPES
#include "ptypes.h"

/*
 * Copy string, converting to uppercase
 */
char *	strucpy(
	_ARX(char *,	dst)
	_AR1(char *,	src)
		)
	_DCL(char *,	dst)
	_DCL(char *,	src)
{
	auto	char	*base = dst;
	register int	c;
	while ((c = *src++) != EOS) {
		UpperCase(c);
		*dst++ = c;
	}
	*dst = EOS;
	return (base);
}

/*
 * Copy string, converting to lowercase
 */
char *	strlcpy(
	_ARX(char *,	dst)
	_AR1(char *,	src)
		)
	_DCL(char *,	dst)
	_DCL(char *,	src)
{
	auto	char	*base = dst;
	register int	c;
	while ((c = *src++) != EOS) {
		LowerCase(c);
		*dst++ = c;
	}
	*dst = EOS;
	return (base);
}
