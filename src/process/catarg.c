#ifndef	lint
static	char	sccs_id[] = "@(#)catarg.c	1.1 88/05/26 07:58:57";
#endif	lint

/*
 * Title:	catarg.c (concatenate argument)
 * Author:	T.E.Dickey
 * Created:	26 May 1988
 * Modified:
 *
 * Function:	Concatenate strings from 'argv[]' onto a buffer which will
 *		later be split in 'bldarg[]'.  Unlike 'strcat()', however,
 *		we translate embedded blanks to another character so that
 *		they can be passed through via 'blarg()'.
 */

extern	char	*strcpy();

catarg(dst, src)
char	*dst, *src;
{
	dst += strlen(dst);
	if (*src) {
		(void)strcpy(dst, src);
		while (*dst) {
			if (*dst == ' ')
				*dst |= 0200;
			dst++;
		}
		*dst++ = ' ';
		*dst   = '\0';
	}
}
