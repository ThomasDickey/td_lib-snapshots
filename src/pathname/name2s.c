#ifndef lint
static  char    Id[] = "$Id: name2s.c,v 8.0 1990/03/12 09:05:51 ste_cm Rel $";
#endif  lint

/*
 * Title:	name2s.c (convert name to external string)
 * Author:	T.E.Dickey
 * Created:	18 Aug 1988 (from ded2s.c)
 * $Log: name2s.c,v $
 * Revision 8.0  1990/03/12 09:05:51  ste_cm
 * BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *
 *		Revision 7.0  90/03/12  09:05:51  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  90/03/12  09:05:51  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.1  90/03/12  09:05:51  dickey
 *		lint (apollo sr10.1)
 *		
 *		Revision 5.0  89/10/04  11:57:47  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.1  89/10/04  11:57:47  dickey
 *		lint (apollo SR10.1)
 *		
 *		Revision 4.0  89/07/25  09:08:47  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.1  89/07/25  09:08:47  dickey
 *		recompiled with apollo SR10 -- mods for function prototypes
 *		
 *		Revision 3.0  88/08/19  06:52:01  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/08/19  06:52:01  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.4  88/08/19  06:52:01  dickey
 *		sccs2rcs keywords
 *		
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

#define	STR_PTYPES
#include	"ptypes.h"
#include	<ctype.h>

#define	isshell(c)	(strchr("*%?$()[]{}|<>^&;#\\\"`'", (size_t)c) != 0)
#define	isAEGIS(c)	(strchr("*%?()[]{}\\", (size_t)c) != 0)

name2s(bfr, len, name, opt)
char	*bfr, *name;
int	len, opt;
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
					&&  strchr("./", (size_t)*name) == 0))
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
