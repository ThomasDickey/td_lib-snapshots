/*
 * Title:	sccsname.c (derive name of SCCS file)
 * Author:	T.E.Dickey
 * Created:	08 May 1990
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		13 Jul 1994, modified interface with 'sccs_dir()'
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, convert to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		08 May 1990, use 'sccsdefs.h'
 *		
 * Function:	Performs translation between SCCS archive and working filenames,
 *		according to the following rules:
 *
 *		to SCCS-file:
 *			{path/}name	=> SCCS/s.name
 *		to working-file:
 *			{path/}s.name	=> name
 *
 *		If a name begins with SCCS_PREFIX, always assume that it is an
 *		SCCS-file, even if no SCCS-directory is specified.  Similarly,
 *		if it does not, it must be a working-file.
 *
 *		These rules are compatible with the assumptions built into
 *		'admin' and 'delta'
 *
 *		These functions return a pointer to static storage which is
 *		overwritten by subsequent calls.  The arguments are unmodified.
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	"sccsdefs.h"

#include	<ctype.h>

MODULE_ID("$Id: sccsname.c,v 12.10 2010/07/05 11:15:26 tom Exp $")

#define	LEN_PREFIX	(sizeof(prefix)-1)

static char prefix[] = SCCS_PREFIX;

/************************************************************************
 *	local procedures						*
 ************************************************************************/

static char *
leaf(char *name)
{
    char *s = fleaf(name);
    return ((s != 0) ? s : name);
}

static const char *
cleaf(const char *name)
{
    const char *s = fleaf(name);
    return ((s != 0) ? s : name);
}

/*
 * Returns TRUE if the name begins with SCCS_PREFIX.
 */
static int
sccs_prefix(const char *name)
{
    const char *s = cleaf(name);
    return (strlen(s) > LEN_PREFIX && !strncmp(s, prefix, LEN_PREFIX));
}

static void
trim_leaf(char *name)
{
    char *s = fleaf_delim(name);
    if (s != 0)
	name = s;
    *name = EOS;
}

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/

/*
 * Given the name of either the working file, or the SCCS-file, obtain the name
 * of the working file.
 */
char *
sccs2name(const char *name, int full)
{
    char *s, *t;
    static char fname[BUFSIZ];

    if (sccs_prefix(name)) {

	s = leaf(strcpy(fname, name));
	strcpy(s, cleaf(name) + LEN_PREFIX);

	if ((s = leaf(fname)) > fname) {
	    char *d = fname;
	    if (full) {
		s[-1] = EOS;
		if (((t = leaf(d)) > d)
		    && sameleaf(t, sccs_dir((char *) 0, (char *) 0)))
		    d = t;
	    }
	    while ((*d++ = *s++) != EOS) ;
	}
    } else {
	(void) strcpy(fname, name);
    }
    return (fname);
}

/*
 * Given the name of either the working file, or the SCCS-file, obtain the name
 * of the SCCS-file.
 */
char *
name2sccs(const char *name, int full)
{
    static char fname[MAXPATHLEN];

    if (sccs_prefix(name)) {
	(void) strcpy(fname, name);
    } else {
	char *dname = sccs_dir(".", name);

	if (full) {
	    trim_leaf(strcpy(fname, name));
	    if (sameleaf(fname, dname))
		trim_leaf(fname);
	} else {
	    *fname = EOS;
	}

	if (isSlash(*dname) || *dname == '~')
	    (void) strcpy(fname, dname);
	else
	    (void) pathcat(fname, fname, dname);

	(void) strcat(
			 pathcat(fname, fname, prefix),
			 cleaf(name));
    }
    return (fname);
}

#ifdef	TEST
static void
do_test(int argc, const char **argv, int full)
{
    int j;
    char old[BUFSIZ], *new;

    if (argc > 1) {
	printf("** %s-path\n", full ? "full" : "local");
	printf("name2sccs:\n");
	for (j = 1; j < argc; j++) {
	    (void) strcpy(old, name2sccs(argv[j], !full));
	    new = name2sccs(argv[j], full);
	    printf("  %-20s => %s%s\n",
		   argv[j], new,
		   strcmp(old, new) ? " (*)" : "");
	}
	printf("sccs2name:\n");
	for (j = 1; j < argc; j++) {
	    (void) strcpy(old, sccs2name(argv[j], !full));
	    new = sccs2name(argv[j], full);
	    printf("  %-20s => %s%s\n",
		   argv[j], new,
		   strcmp(old, new) ? " (*)" : "");
	}
    } else {
	static char my_putenv[80];
	static const char *test[] =
	{
	    "?",
	    "name", "s.name",
	    "path/name", "path/s.name",
	    "path/SCCS/name", "path/SCCS/s.name",
	    "SCCS/name", "SCCS/s.name"
	};
	(void) putenv(strcpy(my_putenv, "SCCS_DIR=SCCS"));
	do_test(SIZEOF(test), test, full);
    }
}

_MAIN
{
    const char **params = (const char **) argv;

    do_test(argc, params, FALSE);
    do_test(argc, params, TRUE);
    exit(SUCCESS);
}
#endif /* TEST */
