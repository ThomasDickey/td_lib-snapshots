#ifndef	lint
static	char	Id[] = "$Id: pathhead.c,v 5.0 1989/10/04 12:14:05 ste_cm Rel $";
#endif	lint

/*
 * Title:	pathhead.c (obtain head from path)
 * Author:	T.E.Dickey
 * Created:	25 Aug 1988
 * $Log: pathhead.c,v $
 * Revision 5.0  1989/10/04 12:14:05  ste_cm
 * BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *
 *		Revision 4.1  89/10/04  12:14:05  dickey
 *		lint (apollo SR10.1)
 *		
 *		Revision 4.0  88/08/25  15:07:07  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/08/25  15:07:07  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/08/25  15:07:07  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.2  88/08/25  15:07:07  dickey
 *		sccs2rcs keywords
 *		
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

#define	notDIR(path)	((stat(path, sb_) < 0)\
			||  ((sb_->st_mode & S_IFMT) != S_IFDIR))

char *
pathhead (path, sb_)
char	*path;
struct	stat	*sb_;
{
	struct	stat	sb;
	register char  *s;
	static	char	buffer[BUFSIZ];

	if (sb_ == 0)	sb_ = &sb;
	path = strcpy(buffer, path);
	while (s = strrchr(path, '/')) {
		if (s[1] == EOS) {	/* trailing '/' ? */
			if (!strcmp(path, "/"))		break;
#ifdef	apollo
			if (!strcmp(path, "//"))	break;
#endif	apollo
			*s = EOS;	/* trim it */
		} else {
			if (notDIR(path))
				*s = EOS;
			else
				break;
		}
	}
	if (notDIR(path)
	||  (*path == EOS)
	||  (s == 0)) {
		(void)stat(strcpy(path, "."), sb_);
	}
	return (path);
}

#ifdef	TEST
main(argc, argv)
char	*argv[];
{
	register int j;
	for (j = 1; j < argc; j++)
		printf("%d:\t\"%s\" => \"%s\"\n",
			j,
			argv[j],
			pathhead(argv[j], (struct stat *)0));
}
#endif	TEST
