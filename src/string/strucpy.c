#ifndef	lint
static	char	what[] = "$Header: /users/source/archives/td_lib.vcs/src/string/RCS/strucpy.c,v 7.0 1988/10/06 10:49:52 ste_cm Rel $";
#endif	lint

/*
 * Title:	strucpy.c
 * Author:	T.E.Dickey
 * Created:	06 Oct 1988
 *
 * Function:	Copy a string, converting to uppercase.
 */

#include	<ctype.h>

strucpy(dst, src)
char	*dst, *src;
{
	register int	c;
	while (c = *src++) {
		if (isascii(c) && isalpha(c) && islower(c))
			c = toupper(c);
		*dst++ = c;
	}
	*dst = '\0';
}
