#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: pathhead.c,v 12.3 1993/11/27 16:44:38 dickey Exp $";
#endif

/*
 * Title:	pathhead.c (obtain head from path)
 * Author:	T.E.Dickey
 * Created:	25 Aug 1988
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		06 Feb 1992, use 'stat_dir()'
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		01 Dec 1989, corrected handling of paths such as "name1/name2"
 *			     which should evaluate to "name1".
 *		04 Oct 1989, lint (apollo SR10.1)
 *		
 * Function:	Returns the directory (relative is ok) from the given path.
 *
 *		This always returns a pointer to a static buffer which is
 *		overwritten by successive calls.  Otherwise, it would have to
 *		modify its argument.
 *
 *		If the name given is a root-level (i.e., "/"), then this is
 *		returned -- the returned string should never be empty.
 *
 *		Unlike 'pathleaf()', this must do a 'stat()' to determine if
 *		we have a real directory.  Therefore, an optional stat-block
 *		is permitted as an argument.
 */

#define	STR_PTYPES
#include	"ptypes.h"

char *
pathhead (
_ARX(char *,	path)
_AR1(STAT *,	sb_)
	)
_DCL(char *,	path)
_DCL(STAT *,	sb_)
{
	auto	int	trimmed	= 0;
	auto	STAT	sb;
	register char  *s;
	static	char	buffer[BUFSIZ];

	if (sb_ == 0)	sb_ = &sb;
	path = strcpy(buffer, path);
	while ((s = fleaf_delim(path)) != NULL) {
#ifdef	apollo
		if (!strcmp(path, "//"))		break;
#endif
		if (s[1] == EOS) {	/* trailing delimiter ? */
			if (path == s)			break;
			*s = EOS;	/* trim it */
			trimmed++;
		} else {
			if (stat_dir(path, sb_) < 0) {
				*s = EOS;
				trimmed++;
			} else
				break;
		}
	}
	if (stat_dir(path, sb_) < 0
	||  (*path == EOS)
	||  (s == 0 && !trimmed)) {
		(void)stat(strcpy(path, "."), sb_);
	}
	return (path);
}

#ifdef	TEST
_MAIN
{
	register int j;
	for (j = 1; j < argc; j++)
		printf("%d:\t\"%s\" => \"%s\"\n",
			j,
			argv[j],
			pathhead(argv[j], (STAT *)0));
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif	/* TEST */
