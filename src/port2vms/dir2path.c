/*
 * Title:	dir2path.c
 * Author:	T.E.Dickey
 * Created:	06 Oct 1988
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		01 Dec 1993, ifdefs.
 *		22 Sep 1993, gcc warnings
 *		20 Nov 1992, use prototypes.  Added default test-case.
 *
 * Function:	Convert a VMS directory-name into the name of the corresponding
 *		path.
 *
 *		Note that this returns a pointer to a static buffer which is
 *		overwritten by each call.
 */

#define	CHR_PTYPES
#define	STR_PTYPES
#include	"port2vms.h"

MODULE_ID("$Id: dir2path.c,v 12.3 2004/03/07 22:03:45 tom Exp $")

static void
insert(char *dst, char *src)
{
    int c, d;

    for (d = strlen(src), c = strlen(dst); c >= 0; c--)
	dst[c + d] = dst[c];

    while ((c = *src++) != EOS)
	*dst++ = c;
}

char *
dir2path(char *src)
{
    static char buffer[MAXPATHLEN];
    char *s, *t;

    (void) strucpy(buffer, src);
    if ((t = strrchr(buffer, '.')) != NULL) {
	while ((s = strrchr(buffer, ';')) == 0)
	    (void) strcat(buffer, ";1");
	if (!s[1])
	    (void) strcat(buffer, "1");
	if (!strncmp(t, ".DIR;1", 6)) {
	    *t = '\0';
	    if ((s = strrchr(buffer, ']')) != NULL) {
		*s = '.';
	    } else if ((s = strrchr(buffer, ':')) != NULL) {
		insert(s + 1, "[");
	    } else
		insert(buffer, "[.");
	    (void) strcat(buffer, "]");
	}
    }
    return (buffer);
}

#ifdef	TEST
static void
do_test(char *path)
{
    PRINTF("%s => %s\n", path, dir2path(path));
}

/*ARGSUSED*/
_MAIN
{
    int j;
    if (argc > 1) {
	for (j = 1; j < argc; j++)
	    do_test(argv[j]);
    } else {
	static char *tbl[] =
	{
	/* non-directory file? */
	    "foo",
	    "dev:foo",
	    "dev:[bar]foo",
	    "[bar]foo",
	/* has ".dir" */
	    "foo.dir",
	    "dev:foo.dir",
	    "dev:[bar]foo.dir",
	    "[bar]foo.dir",
	/* version is */
	    "foo.dir;1",
	    "dev:foo.dir;1",
	    "dev:[bar]foo.dir;1",
	    "[bar]foo.dir;1",
	/* version other-than-1 */
	    "foo.dir;2",
	    "dev:foo.dir;2",
	    "dev:[bar]foo.dir;2",
	    "[bar]foo.dir;2"
	};
	for (j = 0; j < SIZEOF(tbl); j++)
	    do_test(tbl[j]);
    }
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif
