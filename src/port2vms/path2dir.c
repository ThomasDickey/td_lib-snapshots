/*
 * Title:	path2dir.c
 * Author:	T.E.Dickey
 * Created:	30 Sep 1988
 * Modified:
 *		26 Dec 2014, coverity warnings
 *		07 Mar 2004, remove K&R support, indent'd.
 *		01 Dec 1993, ifdefs.
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
#include	"port2vms.h"

MODULE_ID("$Id: path2dir.c,v 12.5 2014/12/26 14:00:09 tom Exp $")

char *
path2dir(const char *src)
{
    static char buffer[MAXPATHLEN];
    if (strlen(src) < sizeof(buffer)) {
	char *s = buffer + strlen(strcpy(buffer, src));

	if (s != buffer && *(--s) == ']') {
	    (void) strcpy(s, ".DIR");
	    while (--s >= buffer) {
		if (*s == '.') {
		    *s = ']';
		    if (s == buffer + 1) {	/* absorb "[]" */
			char *t = s + 1;
			s = buffer;
			while ((*s++ = *t++) != EOS) ;
		    }
		    break;
		}
		if (*s == '[') {	/* absorb "[" */
		    char *t = s + 1;
		    while ((*s++ = *t++) != EOS) ;
		    break;
		}
	    }
	}
    } else {
	strcpy(buffer, ".");
    }
    return (buffer);
}

#ifdef	TEST
static void
do_test(const char *path)
{
    PRINTF("%s => %s\n", path, path2dir(path));
}

_MAIN
{
    int j;
    if (argc > 1) {
	for (j = 1; j < argc; j++)
	    do_test(argv[j]);
    } else {
	static const char *tbl[] =
	{
	    "foo",
	    "dev:foo",
	    "dev:[bar]foo",
	    "[bar]foo",
	    "[bar]",
	    "[foo.bar]",
	    "dev:[bar]",
	    "dev:[bar];"
	};
	for (j = 0; j < (int) SIZEOF(tbl); j++)
	    do_test(tbl[j]);
    }
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif
