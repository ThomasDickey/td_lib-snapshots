/*
 * Title:	path2vms.c
 * Author:	T.E.Dickey
 * Created:	30 Sep 1988
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		01 Dec 1993, ifdefs.
 *		20 Nov 1992, use prototypes.  Added a test-driver
 *		28 Oct 1988, return destination buffer
 *
 * Function:	Convert a unix-style pathname to a VMS-style pathname when we
 *		are certain that it is a directory-name, not a file.
 */

#define		STR_PTYPES
#include	"port2vms.h"

MODULE_ID("$Id: path2vms.c,v 12.3 2004/03/07 22:03:45 tom Exp $")

char *
path2vms(char *dst, char *src)
{
    char tmp[MAXPATHLEN];
    int len = strlen(strcpy(tmp, src));
    if (len == 0 || tmp[len - 1] != '/')
	(void) strcat(tmp, "/");
    return (name2vms(dst, tmp));
}

#ifdef	TEST
static void
do_test(char *path)
{
    char tmp[MAXPATHLEN];
    PRINTF("%s => %s\n", path, path2vms(tmp, path));
}

_MAIN
{
    int j;
    if (argc > 1)
	for (j = 1; j < argc; j++)
	    do_test(argv[j]);
    else {
	static char *tbl[] =
	{
	    "foo",
	    "foo/bar",
	    "foo.bar",
	    "foo/bar.twice"
	};
	for (j = 0; j < SIZEOF(tbl); j++)
	    do_test(tbl[j]);
    }
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif
