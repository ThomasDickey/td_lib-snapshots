#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: shoarg.c,v 12.2 1993/10/29 17:35:24 dickey Exp $";
#endif

/*
 * Title:	shoarg.c (display argv-array)
 * Created:	20 Jun 1991
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
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
char *	bldcmd(
	_ARX(char *,	dst)
	_ARX(char *,	src)
	_AR1(size_t,	len)
		)
	_DCL(char *,	dst)
	_DCL(char *,	src)
	_DCL(size_t,	len)
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
char *	bldcmd2(
	_ARX(char *,	dst)
	_ARX(char *,	src1)
	_ARX(char *,	src2)
	_AR1(size_t,	len)
		)
	_DCL(char *,	dst)
	_DCL(char *,	src1)
	_DCL(char *,	src2)
	_DCL(size_t,	len)
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
void	shoarg (
	_ARX(FILE *,	fp)
	_ARX(char *,	command)
	_AR1(char *,	string)
		)
	_DCL(FILE *,	fp)
	_DCL(char *,	command)
	_DCL(char *,	string)
{
	char	temp[BUFSIZ];

	FPRINTF(fp, "%% %s\n", bldcmd2(temp, command, string, sizeof(temp)));
}
