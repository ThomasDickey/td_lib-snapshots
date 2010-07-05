/*
 * Title:	edittree.c
 * Author:	T.E.Dickey
 * Created:	06 Oct 1988
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		01 Dec 1993, ifdefs.
 *		22 Sep 1993, gcc warnings
 *		29 Apr 1993, sort _all_ leaves before doing translation.
 *		20 Nov 1992, added 3rd arg to _FNX macros.
 *		11 Dec 1991, added 'links' argument.  Process entire list of
 *			     files per-directory to avoid possible conflict
 *			     with temporary-files in current-directory.  Also,
 *			     pass-in 'sb' argument to 'func()' with name.
 *
 * Function:	Invokes a single-line editing function on a tree of files.
 *
 *		On VMS, we use this also to hide the fact that DCL does not
 *		do wildcard expansion.  Hence, the "recur" option may be
 *		false simply to do in-directory scanning.
 *
 *		Note that this assumes that we do directory-recursion in a
 *		unix-style, rather than the "..." wildcard that VMS provides.
 */
#define	DIR_PTYPES
#define	ERR_PTYPES
#define	STR_PTYPES
#include	"port2vms.h"
#include	"td_qsort.h"

MODULE_ID("$Id: edittree.c,v 12.7 2010/07/04 19:18:14 tom Exp $")

typedef char *PTR;
#define	CHUNK	127		/* 1 less than a power of 2 */
#define	v_ALLOC(v,n,s)	v = DOALLOC(v, PTR, ((++n)|CHUNK)+1);\
			v[n-1] = txtalloc(s)

#ifdef	TEST
#define	TELL_FILE(name)	FPRINTF(stderr, "%d\t%s => %s\n", changes, nesting, name);
#define	TELL_DIR(name)	FPRINTF(stderr, "%d\t%s directory %s\n", changes, nesting, name);
int
editfile(char *n,
	 int (*func) (FILE *o, FILE *i, Stat_t * s),
	 Stat_t * s)
{
    return 1;
}
#else
#define	TELL_FILE(name)
#define	TELL_DIR(name)
#endif

#ifdef	S_IFLNK
#define	LOOK(name,sb)	(links ? stat(name,sb) : lstat(name,sb))
#else
#define	LOOK(name,sb)	(stat(name,sb))
#endif

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/

int
edittree(const char *oldname,
	 int (*func) (FILE *o, FILE *i, Stat_t * s),
	 int recur,
	 int links)
{
    DIR *dirp;
    DirentT *dp;
    int changes = 0;
    int next = recur ? recur + 1 : 0;
    Stat_t sb;
    unsigned num;
    PTR *vec;
    char newname[MAXPATHLEN];
    char oldpath[MAXPATHLEN];
    const char *newpath;

#ifdef	TEST
    static char stack[] = ". . . . . . . ";
    int level = recur ? recur : 1;
    char *nesting = &stack[sizeof(stack) - (level * 2)];
#endif

    if (LOOK(oldname, &sb) < 0) {
	errno = ENOENT;		/* bypass vms-bug */
	perror(oldname);
	return (0);
    }

    if (_OPENDIR(oldname, sb.st_mode)) {
	TELL_DIR(oldname);

	if (getwd(oldpath) == 0) {
	    perror("(getwd)");
	    return (0);
	}
	newpath = EDITDIR_ARG;
#ifdef	vms
	if (vms_iswild(oldname))
	    newpath = oldname;
	else
#endif
	if (chdir(DIR2PATH(oldname)) < 0) {
	    perror(oldname);
	    return (0);
	}

	if ((dirp = opendir(newpath)) != NULL) {
	    num = 0;
	    vec = 0;
	    while ((dp = readdir(dirp)) != NULL) {
		(void) strcpy(newname, dp->d_name);
#ifndef	vms
		if (dotname(newname))
		    continue;
#endif
		v_ALLOC(vec, num, newname);
	    }
	    closedir(dirp);
	    if (num != 0) {
		qsort((PTR) vec, num, sizeof(PTR), cmp_qsort);
		while (num-- != 0) {
		    if (LOOK(vec[num], &sb) < 0) {
			perror(vec[num]);
			continue;
		    }
		    if (isDIR(sb.st_mode)) {
			if (next)
			    changes += edittree(vec[num], func, next, links);
		    } else if (isFILE(sb.st_mode)) {
			TELL_FILE(vec[num]);
			changes += editfile(vec[num], func, &sb);
		    }
		}
		dofree((PTR) vec);
	    }
	}
	if (chdir(oldpath) != 0)
	    failed(oldpath);
    } else {
	TELL_FILE(oldname);
	changes += editfile(oldname, func, &sb);
    }
    return (changes);
}

#ifdef	TEST
static int
do_copy(FILE *ofp, FILE *ifp, Stat_t * sb)
{
    char buffer[BUFSIZ];
    while (fgets(buffer, sizeof(buffer), ifp))
	fputs(buffer, ofp);
    return (1);
}

static void
do_test(int argc, char **argv)
{
    int j;
    int recur = FALSE;
    int links = FALSE;

    for (j = 1; j < argc; j++) {
	if (!strcmp(argv[j], "-r"))
	    recur = TRUE;
	else if (!strcmp(argv[j], "-L"))
	    links = TRUE;
	else
	    PRINTF("count = %d\n", edittree(argv[j], do_copy, recur, links));
    }
}

/*ARGSUSED*/
_MAIN
{
    if (argc > 1)
	do_test(argc, argv);
    else {
	static char *tbl[] =
	{
	    "?",
#ifdef	vms
	    "[-]", "-r", "[-]"
#else				/* SYS_UNIX */
	    "..", "-r", ".."
#endif				/* vms/SYS_UNIX */
	};
	do_test(SIZEOF(tbl), tbl);
    }
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif /* TEST */
