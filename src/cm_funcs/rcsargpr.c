/*
 * Title:	rcsargpair.c
 * Author:	T.E.Dickey
 * Created:	05 Feb 1992
 * Modified:
 *		27 Dec 2014, coverity warnings.
 *		24 May 2010, fix clang --analyze warnings.
 *		05 Sep 2006, allow '$' in filenames
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	Parses argument-lists of working and/or archive names for rcs-
 *		based tools.
 *
 *		This is intended to act like the normal rcs/ci/co argument
 *		parsing, except for the cases where the path of the RCS file is
 *		not specified.  In these cases, we allow the environment
 *		variable RCS_DIR to modify the parsing of names:
 *
 *		a) if RCS_DIR is not set, assume a default value of "RCS".
 *		b) if RCS_DIR is set, use its value.
 *
 *		We assume that the RCS file in all cases has a unique location,
 *		and do not search for it in the alternate location.  This is
 *		because (unlike the rcs utilities), we want to create the RCS
 *		directory if it does not already exist.
 *
 * Returns:	the next index into 'argv[]' at which we can parse arguments.
 *		If an error is found, the last-index is returned after printing
 *		an error message.
 *
 *		Loop on this function, e.g.,
 *
 *			int	n = 1;
 *			while (n < argc) {
 *				n = rcsargpair(n, argc, argv);
 *				process_pair();
 *			}
 *
 * From RCS man-pages:
 * CI:
 *	Pairs of RCS files and working files may be specified in 3 ways (see
 *	also the example section of 'co').
 *
 *	1) Both the RCS file and the working file are given. The RCS file name
 *	   is of the form 'path1/workfile,v' and the working file name is of
 *	   the form 'path2/workfile', where 'path1/' and 'path2/' are (possibly
 *	   different or empty) paths and 'workfile' is a file name.
 *
 *	2) Only the RCS file is given. Then the working file is assumed to be
 *	   in the current directory and its name is derived from the name of
 *	   the RCS file by removing 'path1/' and the suffix ',v'.
 *
 *	3) Only the working file is given. Then 'ci' looks for an RCS file of
 *	   the form 'path2/RCS/workfile,v' or 'path2/workfile,v' (in this
 *	   order).
 *
 *	If the RCS file is specified without a path in 1) and 2), then 'ci'
 *	looks for the RCS file first in the directory ./RCS and then in the
 *	current directory.
 *
 * CO:
 *	Identical with 'ci', except in 2), it creates the working file in the
 *	current directory (rather than assuming it is already there!).
 *
 * RCS:
 *	Files ending in `,v' are RCS files, all others are working files. If a
 *	working file is given, 'rcs' tries to find the corresponding RCS file
 *	first in directory ./RCS and then in the current directory, as explained
 *	in 'co'.
 *
 *	The command "rcs -i" creates and initializes a new RCS file, but does
 *	not deposit any revision.  If the RCS file has no path prefix, 'rcs'
 *	tries to place it first into the subdirectory ./RCS, and then into the
 *	current directory.  If the RCS file already exists, an error message
 *	is printed.
 */

#define	STR_PTYPES
#include "ptypes.h"
#include "rcsdefs.h"
#include <errno.h>

MODULE_ID("$Id: rcsargpr.c,v 12.12 2025/01/07 00:01:26 tom Exp $")

/************************************************************************
 *	local data							*
 ************************************************************************/
#ifdef	TEST
#define	TRACE(s)	PRINTF s
#else
#define	TRACE(s)
#endif

#define	DEBUG(s)	if(RCS_DEBUG) PRINTF s;

#define	NOT_YET		1	/* neither -1 nor 0 */

#define	LEN_SUFFIX	(sizeof(suffix)-1)

static char suffix[] = RCS_SUFFIX;

static Stat_t stat_working, stat_archive, stat_located;

static int errs_working;
static int have_working;
static int errs_archive;
static int have_archive;
static int errs_located;
static int have_located;

static char *name_working;	/* complete path of working file */
static char *name_archive;	/* complete path of RCS file */
static char *name_located;	/* directory in which archive is located */

/************************************************************************
 *	local procedures						*
 ************************************************************************/

/*
 * Returns TRUE if the name ends with RCS_SUFFIX, making the filename that of
 * an RCS-archive.
 */
static int
is_archive(char *name)
{
    size_t len = strlen(name);
    if (len >= LEN_SUFFIX)
	if (!strcmp(suffix, name + len - LEN_SUFFIX))
	    return (TRUE);
    return (FALSE);
}

/*
 * Returns a pointer to the leaf (i.e., filename) of a path.
 */
static char *
leaf_of(char *name)
{
    char *s = fleaf(name);
    return ((s != NULL) ? s : name);
}

/*
 * Strips the RCS-suffix from a path (modifies/returns its argument)
 */
static char *
strip_suffix(char *name)
{
    size_t len = strlen(name);
    if (len >= LEN_SUFFIX)
	if (!strcmp(suffix, name + len - LEN_SUFFIX))
	    name[len - LEN_SUFFIX] = EOS;
    return name;
}

/*
 * Compares two paths to see if they have the same leaf (ignoring suffix)
 */
static int
same_name(char *name1,
	  char *name2)
{
    char leaf1[MAXPATHLEN];
    char leaf2[MAXPATHLEN];
    int rc = -1;

    name1 = leaf_of(name1);
    name2 = leaf_of(name2);

    if (strlen(name1) < sizeof(leaf1)
	&& strlen(name2) < sizeof(leaf2)) {
	rc = !strcmp(
			strip_suffix(strcpy(leaf1, name1)),
			strip_suffix(strcpy(leaf2, name2))
	    );
    }
    return rc;
}

/*
 * Verify that a pathname is not too long, and properly formatted.
 */
static int
ok_name(char *name)
{
    size_t len = strlen(name) + strlen(rcs_dir(NULL, NULL)) +
    LEN_SUFFIX + 2;

    if (len > MAXPATHLEN) {
	FPRINTF(stderr, "? name too long:%s\n", name);
	return FALSE;
    }
    return TRUE;		/* ok I guess */
}

/*
 * Initializes this module
 */
static void
initialize(void)
{
    static int done;

    if (!done) {
	done = TRUE;

	*(name_working = doalloc((char *) 0, (size_t) MAXPATHLEN + 1)) = EOS;
	*(name_archive = doalloc((char *) 0, (size_t) MAXPATHLEN + 1)) = EOS;
	*(name_located = doalloc((char *) 0, (size_t) MAXPATHLEN + 1)) = EOS;
    }
}

/*
 * Re-initializes (for use in 'rcsargpair()')
 */
static void
reinitialize(void)
{
    initialize();
    errs_working = errs_archive = errs_located = 0;
    have_working = have_archive = have_located = NOT_YET;
}

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/

/*
 * Functions that copy the corresponding name into caller's buffer, and return
 * 0 iff the object already exists (-1 otherwise).
 *
 * If the Stat-pointer is nonnull (the usual case), the corresponding Stat_t block
 * is copied also, so that the caller can do ownership/access testing.
 */
int
rcs_working(char *Name,
	    Stat_t * Stat)
{
    initialize();
    if (Stat != NULL || have_working == NOT_YET) {
	have_working = stat_file(name_working, &stat_working);
	errs_working = have_working ? errno : 0;
    }
    if (Stat != NULL)
	*Stat = stat_working;
    if (Name != NULL)
	(void) strcpy(Name, name_working);
    DEBUG(("++ rcs_working(%s) errs %d\n", name_working, errs_working))
	return (errno = errs_working) ? -1 : 0;
}

int
rcs_archive(char *Name,
	    Stat_t * Stat)
{
    initialize();
    if (Stat != NULL || have_archive == NOT_YET) {
	have_archive = stat_file(name_archive, &stat_archive);
	errs_archive = have_archive ? errno : 0;
    }
    if (Stat != NULL)
	*Stat = stat_archive;
    if (Name != NULL)
	(void) strcpy(Name, name_archive);
    DEBUG(("++ rcs_archive(%s) errs %d\n", name_archive, errs_archive))
	return (errno = errs_archive) ? -1 : 0;
}

int
rcs_located(char *Name,
	    Stat_t * Stat)
{
    initialize();
    if (Stat != NULL || have_located == NOT_YET) {
	have_located = stat_dir(name_located, &stat_located);
	errs_located = have_located ? errno : 0;
    }
    if (Stat != NULL)
	*Stat = stat_located;
    if (Name != NULL)
	(void) strcpy(Name, name_located);
    DEBUG(("++ rcs_located(%s) errs %d\n", name_located, errs_located))
	return (errno = errs_located) ? -1 : 0;
}

/*
 * Function to invoke for argument-parsing.
 */
int
rcsargpair(int This,
	   int Last,		/* same as 'argc' */
	   char **Argv)		/* assumed non-volatile, from 'main()' */
{
    int test_1, test_2;
    char *name_1;
    char *name_2;
    char *working = NULL;
    char *archive = NULL;
    char temp_archive[MAXPATHLEN];

    reinitialize();

    /* gobble up one or two arguments with the same working-name */
    if (This < Last) {

	name_1 = Argv[This++];
	test_1 = is_archive(name_1);
	if (!ok_name(name_1))
	    return Last;

	if (test_1)
	    archive = name_1;
	else
	    working = name_1;

	if (This < Last) {
	    name_2 = Argv[This];
	    test_2 = is_archive(name_2);
	    if (!ok_name(name_2))
		return Last;

	    if ((test_1 ^ test_2) && same_name(name_1, name_2)) {
		This++;
		if (test_2)
		    archive = name_2;
		else
		    working = name_2;
	    }
	}
    }

    /* supply missing directory on archive, for consistency */
    if (archive != NULL && !fleaf_delim(archive))
	archive = pathcat(temp_archive, rcs_dir(NULL, NULL), archive);

    if (archive == NULL && working != NULL) {

	TRACE(("...case 3\n"));
	(void) strcpy(name_working, working);

	(void) strcpy(name_archive, working);	/* copy path */
	FORMAT(leaf_of(name_archive), "%s/%s%s",
	       rcs_dir(NULL, NULL), leaf_of(working), RCS_SUFFIX);

    } else if (archive != NULL && working == NULL) {

	TRACE(("...case 2\n"));
	(void) strcpy(name_archive, archive);

	working = leaf_of(archive);
	FORMAT(name_working, "./%s", working);
	(void) strip_suffix(name_working);

    } else if (archive != NULL && working != NULL) {

	TRACE(("...case 1\n"));
	(void) strcpy(name_working, working);
	(void) strcpy(name_archive, archive);

    } else {

	FPRINTF(stderr, "? missing archive/working names\n");
	return Last;
    }

    /* the pathname must have a non-empty leaf! */
    working = leaf_of(name_working);
    if (!*working) {
	FPRINTF(stderr, "? missing name for %s\n", archive);
	return Last;
    }

    /* determine the directory-name for the RCS-archive */
    name_1 = leaf_of(strcpy(name_located, name_archive));
    if (name_1 == name_located)
	(void) strcpy(name_1, ".");
    else
	name_1[-1] = EOS;

    return This;
}

#ifdef	TEST
/*ARGSUSED*/
_MAIN
{
    int j = 1;

    rcs_working(0, 0);
    while (j < argc) {
	PRINTF("%3d] parameter:%s\n", j, argv[j]);
	j = rcsargpair(j, argc, argv);
	PRINTF("    working   :%s\n", name_working);
	PRINTF("    archive   :%s\n", name_archive);
	PRINTF("    located   :%s\n", name_located);
    }
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif /* TEST */
