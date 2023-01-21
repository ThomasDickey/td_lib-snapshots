/*
 * Title:	editfile.c
 * Author:	T.E.Dickey
 * Created:	03 Oct 1988
 * Modified:
 *		20 Jan 2023, use mkstemp if available.
 *		12 Dec 2014, fix memory leak (coverity).
 *		07 Mar 2004, remove K&R support, indent'd.
 *		21 Aug 1994, argument of mktemp is volatile on Linux.
 *		01 Dec 1993, ifdefs, TurboC warnings.
 *		22 Sep 1993, gcc warnings
 *		20 Nov 1992, added 3rd arg to _FNX macros.
 *		13 Dec 1991, pass 'sb' arg down to 'func', from initial stat on
 *			     the input-file.
 *		24 Apr 1989, port to unix using 'mktemp()'
 *
 * Function:	Invokes a single-line editing function on a file.  If no changes
 *		are made to the file, we discard the new file and keep the old.
 *
 *		On VMS we can directly create a new version of a file without
 *		having to delete the old one first.  On unix we must copy to
 *		another name first.
 */

#define		STR_PTYPES
#include	"ptypes.h"

MODULE_ID("$Id: editfile.c,v 12.9 2023/01/21 00:57:06 tom Exp $")

#ifdef	vms
#define	MyTemp(name,mode)	fopen(name, mode)
#elif defined(HAVE_MKSTEMP)
#define	MyTemp(name,mode)	fdopen(mkstemp(name), mode)
#else /* SYS_UNIX */
#define	MyTemp(name,mode)	fopen(mktemp(name), mode)
#endif /* vms/SYS_UNIX */

int
editfile(const char *oldname,
	 int (*func) (FILE *o, FILE *i, Stat_t * s),
	 Stat_t * sb)
{
    FILE *ifp = fopen(oldname, "r");
    FILE *ofp;
    int changes = 0;
#ifdef	vms
    char newname[MAXPATHLEN];
    char *s;
    (void) strcpy(newname, oldname);
    if (s = strrchr(newname, ';'))
	s[1] = '\0';
    else
	(void) strcat(newname, ";");
#else /* SYS_UNIX */
    char newname[MAXPATHLEN];
    (void) strcpy(newname, "fileXXXXXX");
#endif /* vms/SYS_UNIX */

    if (ifp != 0) {
	if ((ofp = MyTemp(newname, "w")) != 0) {
	    FPRINTF(stderr, "** edit \"%s\" => \"%s\"\n", oldname, newname);
	    changes += (*func) (ofp, ifp, sb);
	    (void) fclose(ofp);
	    if (changes == 0) {
		FPRINTF(stderr, "** no change made\n");
		(void) unlink(newname);
	    }
#ifndef	vms
	    else {
		if (rename(newname, oldname) >= 0) {
		    (void) unlink(newname);
		} else {
		    perror(newname);
		    return (0);
		}
	    }
#endif
	    if (changes > 0) {
		FPRINTF(stderr, "** %d change(s) made\n", changes);
	    }
	} else {
	    perror(newname);
	}
	(void) fclose(ifp);
    } else {
	perror(oldname);
    }
    return (changes);
}

#ifdef	TEST
static int
do_copy(FILE *ofp, FILE *ifp, Stat_t * sb)
{
    char buffer[BUFSIZ];

    (void) sb;

    while (fgets(buffer, sizeof(buffer), ifp))
	if (fputs(buffer, ofp) < 0) {
	    perror("do_copy");
	    return (0);
	}
    fputs("LAST\n", ofp);
    return (1);
}

_MAIN
{
    int j;

    for (j = 1; j < argc; j++)
	editfile(argv[j], do_copy, (Stat_t *) 0);
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif /* TEST */
