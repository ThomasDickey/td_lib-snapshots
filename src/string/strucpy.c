#ifndef	lint
static	char	Id[] = "$Id: strucpy.c,v 9.0 1991/05/15 09:55:56 ste_cm Rel $";
#endif

/*
 * Title:	strucpy.c (string-copy, uppercase)
 * Author:	T.E.Dickey
 * Created:	06 Oct 1988
 * $Log: strucpy.c,v $
 * Revision 9.0  1991/05/15 09:55:56  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  09:55:56  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  90/06/27  08:57:20  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.1  90/06/27  08:57:20  dickey
 *		added procedure 'strlcpy()' to this module.
 *		altered 'strucpy()' to return its destination-arg a la 'strcpy()'
 *		
 * Function:	Copy a string, converting its case (upper/lower). The source
 *		and destination arguments may be the same.
 */

#include	"ptypes.h"
#include	<ctype.h>

/*
 * Copy string, converting to uppercase
 */
char	*
strucpy(dst, src)
char	*dst, *src;
{
	auto	char	*base = dst;
	register int	c;
	while (c = *src++) {
		if (isascii(c) && isalpha(c) && islower(c))
			c = toupper(c);
		*dst++ = c;
	}
	*dst = '\0';
	return (base);
}

/*
 * Copy string, converting to lowercase
 */
char	*
strlcpy(dst, src)
char	*dst, *src;
{
	auto	char	*base = dst;
	register int	c;
	while (c = *src++) {
		if (isascii(c) && isalpha(c) && isupper(c))
			c = tolower(c);
		*dst++ = c;
	}
	*dst = '\0';
	return (base);
}
