#ifndef lint
static  char    sccs_id[] = "@(#)name2s.c	1.2 88/08/18 15:57:30";
#endif  lint

/*
 * Title:	name2s.c (convert name to external string)
 * Author:	T.E.Dickey
 * Created:	18 Aug 1988 (from ded2s.c)
 * Modified:
 *
 * Function:	Convert a filename-string to printing form (for display), or
 *		to form useful for shell commands.
 *
 * Arguments:	bfr	= buffer in which to place converted name.
 *		len	= maximum length of 'bfr[]'.
 *		name	= name to convert
 *		opt	= options, by bit:
 *			1=escapes,
 *			2=underlying system (e.g., DOMAIN/IX to AEGIS)
 *
 * Returns:	The length of the converted name.
 *
 * patch:	The apollo conversion does not handle some of the bizarre
 *		code for converting the character ".".
 *
 *		Some apollo cases not handled include those which must be
 *		quoted:
 *			"("	=> "'@\('"
 *			"\\"	=> unknown
 *
 *		Don't know if we will ever get a path with "\\" in it.
 *		This does not escape filenames containing a newline.
 */

#include	"ptypes.h"
#include	<ctype.h>
extern	char	*strchr();

#define	isshell(c)	(strchr("*%?$()[]{}|<>^&;#\\\"`'", c) != 0)
#define	isAEGIS(c)	(strchr("*%?()[]{}\\", c) != 0)

name2s(bfr, len, name, opt)
char	*bfr, *name;
{
	char	*base = bfr;
	register int c;
	int	esc	= opt & 1;
#ifdef	apollo
	int	in_leaf	= 0;
#endif	apollo

	while ((c = *name++) && len-- > 0) {
#ifdef	apollo
		if (c == '/')	in_leaf = 0;
		else		in_leaf++;
		if (opt & 2) {	/* show underlying apollo filenames */
			if (isascii(c) && isgraph(c)) {
				if (isalpha(c) && isupper(c)) {
					*bfr++ = ':';
					c = _tolower(c);
				} else if ((c == ':')
				||	   (c == '.'
					&&  in_leaf == 1
					&&  strchr("./", *name) == 0))
					*bfr++ = ':';
				else if (esc) {
					if (isAEGIS(c))
						*bfr++ = '@';
					if (isshell(c))
						*bfr++ = '\\';
				}
				*bfr++ = c;
			} else if (c == ' ') {
				*bfr++ = ':';
				*bfr++ = '_';
			} else {
				FORMAT(bfr, ":%s#%02x", esc ? "\\" : "", c);
				bfr += strlen(bfr);
			}
		} else
#endif	apollo
		if (esc) {
			if(iscntrl(c)
			|| isspace(c)
			|| isshell(c))
				*bfr++ = '\\';	/* escape the nasty thing */
			*bfr++ = c;
		} else {
			if (isascii(c) && isgraph(c)) {
				*bfr++ = c;
			} else
				*bfr++ = '?';
		}
	}
	*bfr = '\0';
	return (bfr-base);
}

#ifdef	TEST
main(argc, argv)
char	*argv[];
{
	extern	int	optind;

	register int j;
	int	opt	= 0;
	char	bfr[BUFSIZ];

	while ((j = getopt(argc, argv, "eu")) != EOF)
		switch (j) {
		case 'e':	opt |= 1;	break;
		case 'u':	opt |= 2;	break;
		}
	for (j = optind; j < argc; j++) {
		name2s(bfr, sizeof(bfr), argv[j], opt);
		printf("%d:\t\"%s\" => \"%s\"\n", j - optind + 1, argv[j], bfr);
	}
}
#endif	TEST
