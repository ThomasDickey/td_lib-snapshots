#ifndef	lint
static	char	Id[] = "$Id: path2dir.c,v 8.1 1993/09/22 17:53:36 dickey Exp $";
#endif

/*
 * Title:	path2dir.c
 * Author:	T.E.Dickey
 * Created:	30 Sep 1988
 * Modified:
 *		22 Sep 1993, gcc warnings
 *		20 Nov 1992, use prototypes
 *
 * Function:	Convert a VMS pathname into the name of the corresponding
 *		directory-file.
 *
 *		Note that this returns a pointer to a static buffer which is
 *		overwritten by each call.
 */

#define		STR_PTYPES
#include	"portunix.h"

char *	path2dir(
	_AR1(char *,	src))
	_DCL(char *,	src)
{
	static	char	buffer[MAXPATHLEN];
	register char	*s	= buffer + strlen(strcpy(buffer, src));

	if (s != buffer && *(--s) == ']') {
		(void)strcpy(s, ".DIR");
		while (--s >= buffer) {
			if (*s == '.') {
				*s = ']';
				if (s == buffer+1) {	/* absorb "[]" */
					register char *t = s + 1;
					s = buffer;
					while ((*s++ = *t++) != EOS)
						;
				}
				break;
			}
			if (*s == '[') {		/* absorb "[" */
				register char *t = s + 1;
				while ((*s++ = *t++) != EOS)
					;
				break;
			}
		}
	}
	return (buffer);
}

#ifdef	TEST
static
void	do_test(
	_AR1(char *,	path))
	_DCL(char *,	path)
{
	PRINTF("%s => %s\n", path, path2dir(path));
}

_MAIN
{
	register int	j;
	if (argc > 1)
		for (j = 1; j < argc; j++)
			do_test(argv[j]);
	else {
		static	char	*tbl[] = {
			"foo",
			"dev:foo",
			"dev:[bar]foo",
			"[bar]foo",
			"[bar]",
			"[foo.bar]",
			"dev:[bar]",
			"dev:[bar];"
			};
		for (j = 0; j < SIZEOF(tbl); j++)
			do_test(tbl[j]);
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
