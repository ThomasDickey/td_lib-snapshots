#ifndef lint
static  char    Id[] = "$Id: name2s.c,v 12.2 1993/09/21 18:54:04 dickey Exp $";
#endif 

/*
 * Title:	name2s.c (convert name to external string)
 * Author:	T.E.Dickey
 * Created:	18 Aug 1988 (from ded2s.c)
 * Modified:
 *		21 Sep 1993, gcc-warnings
 *		04 Oct 1991, conversion to ANSI
 *		22 Jul 1991, allow space as a printing char.
 *		19 Aug 1988, added 'opt & 4' mode so we can make 'copy' work on
 *			     things like "/bin/[".
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
 *			4=underlying escapes (no unix escapes)
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

#define	CHR_PTYPES
#define	STR_PTYPES
#include "ptypes.h"

#define	isshell(c)	(strchr("*%?$()[]{}|<>^&;#\\\"`'", c) != 0)
#define	isAEGIS(c)	(strchr("*%?()[]{}\\", c) != 0)

#undef	doAEGIS
#if	defined(apollo) || defined(TEST)
#define	doAEGIS
#endif

int	name2s(
	_ARX(char *,	bfr)
	_ARX(int,	len)
	_ARX(char *,	name)
	_AR1(int,	opt)
		)
	_DCL(char *,	bfr)
	_DCL(int,	len)
	_DCL(char *,	name)
	_DCL(int,	opt)
{
	char	*base = bfr;
	register int c;
	int	esc	= opt & 1;
#ifdef	doAEGIS
	int	in_leaf	= 0;
#endif

	while ((c = *name++) && len-- > 0) {
#ifdef	doAEGIS
		if (c == '/')	in_leaf = 0;
		else		in_leaf++;
		if (opt & 2) {	/* show underlying apollo filenames */
			if (isascii(c) && isgraph(c)) {
				if (isalpha(c) && isupper(c)) {
					*bfr++ = ':';
					c = LowerMacro(c);
				} else if ((c == ':')
				||	   (c == '.'
					&&  in_leaf == 1
					&&  strchr("./", *name) == 0))
					*bfr++ = ':';
				else if (opt & 5) {
					if (isAEGIS(c))
						*bfr++ = '@';
					if ((opt & 1) && isshell(c))
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
#endif
		if (esc) {
			if(!isascii(c)
			 || iscntrl(c)
			 || isspace(c)
			 || isshell(c))
				*bfr++ = '\\';	/* escape the nasty thing */
			*bfr++ = c;
		} else {
			if (isascii(c) && isprint(c)) {
				*bfr++ = c;
			} else
				*bfr++ = '?';
		}
	}
	*bfr = EOS;
	return (bfr-base);
}

#ifdef	TEST
/*ARGSUSED*/
_MAIN
{
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
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
