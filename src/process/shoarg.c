#ifndef	lint
static	char	Id[] = "$Id: shoarg.c,v 12.0 1991/10/04 13:56:30 ste_cm Rel $";
#endif

/*
 * Title:	shoarg.c (display argv-array)
 * Created:	20 Jun 1991
 * Modified:
 *		04 Oct 1991, conversion to ANSI
 *		22 Jul 1991, don't assume that 'command' contains no spaces.
 *			     Added entrypoint 'bldcmd()'.
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	<ctype.h>

/*
 * Function:	Writes a new string with the non-ascii characters escaped.
 */
char	*
bldcmd(dst, src, len)
char	*dst, *src;
size_t	len;
{
	char	*base = dst;
	register int	c;
	*dst = EOS;
	while (len-- > 0 && (c = *src++)) {
		if (!isascii(c)) {
			if (--len <= 0)
				break;
			*dst++ = '\\';
			*dst++ = toascii(c);
		} else
			*dst++ = c;
	}
	*dst = EOS;
	return (base);
}

/*
 * Function:	Combines two strings into one destination, with non-ascii stuff
 *		escaped.
 */
char	*
bldcmd2(dst, src1, src2, len)
char	*dst,*src1,*src2;
size_t	len;
{
	char	*base = dst;
	(void)bldcmd(dst, src1, len);			dst += strlen(dst);
	(void)bldcmd(dst, " ",	len - (dst - base));	dst += strlen(dst);
	(void)bldcmd(dst, src2, len - (dst - base));
	return base;
}

/*
 * Function:	Displays a command + argument-string constructed by 'catarg()'
 *
 * Arguments:	fp	- file-pointer to which to write result
 *		command	- command-verb
 *		string	- the command-string to display
 */
shoarg (fp, command, string)
FILE	*fp;
char	*command, *string;
{
	char	temp[BUFSIZ];

	FPRINTF(fp, "%% %s\n", bldcmd2(temp, command, string, sizeof(temp)));
}
