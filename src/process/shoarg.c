/*
 * Title:	shoarg.c (display argv-array)
 * Created:	20 Jun 1991
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		24 Dec 2000, ctype.h fix for QNX
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		04 Oct 1991, conversion to ANSI
 *		22 Jul 1991, don't assume that 'command' contains no spaces.
 *			     Added entrypoint 'bldcmd()'.
 */

#define	CHR_PTYPES
#define	STR_PTYPES
#include	"ptypes.h"

MODULE_ID("$Id: shoarg.c,v 12.6 2004/03/07 22:03:45 tom Exp $")

/*
 * Function:	Writes a new string with the non-ascii characters escaped.
 */
char *
bldcmd(char *dst,
       char *src,
       size_t len)
{
    char *base = dst;
    int c;
    *dst = EOS;
    while (len-- > 0 && (c = *src++) != EOS) {
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
char *
bldcmd2(char *dst,
	char *src1,
	char *src2,
	size_t len)
{
    char *base = dst;
    (void) bldcmd(dst, src1, len);
    dst += strlen(dst);
    (void) bldcmd(dst, " ", len - (dst - base));
    dst += strlen(dst);
    (void) bldcmd(dst, src2, len - (dst - base));
    return base;
}

/*
 * Function:	Displays a command + argument-string constructed by 'catarg()'
 *
 * Arguments:	fp	- file-pointer to which to write result
 *		command	- command-verb
 *		string	- the command-string to display
 */
void
shoarg(FILE *fp,
       char *command,
       char *string)
{
    char temp[BUFSIZ];

    FPRINTF(fp, "%% %s\n", bldcmd2(temp, command, string, sizeof(temp)));
}
