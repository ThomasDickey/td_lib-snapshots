#ifndef	lint
static	char	*Id = "$Id: rcsargpr.c,v 10.5 1992/02/05 14:26:10 dickey Exp $";
#endif

/*
 * Title:	rcsfilepair.c
 * Author:	T.E.Dickey
 * Created:	05 Feb 1992
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
 *				n = rcsfilepair(n, argc, argv);
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

/************************************************************************
 *	local data							*
 ************************************************************************/
#ifdef	TEST
#define	TRACE(s)	PRINTF s;
#else
#define	TRACE(s)
#endif

#define	NOT_YET		1	/* neither -1 nor 0 */

#define	LEN_SUFFIX	(sizeof(suffix)-1)

static	char	suffix[] = RCS_SUFFIX;

static	STAT	stat_working,
		stat_archive,
		stat_located;

static	int	have_working = NOT_YET,
		have_archive = NOT_YET,
		have_located = NOT_YET;

static	char	*name_working,	/* complete path of working file */
		*name_archive,	/* complete path of RCS file */
		*name_located;	/* directory in which archive is located */

/************************************************************************
 *	local procedures						*
 ************************************************************************/

/*
 * Returns TRUE if the name ends with RCS_SUFFIX, making the filename that of
 * an RCS-archive.
 */
static
int
is_archive(
_AR1(char *,	name))
_DCL(char *,	name)
{
	int	len = strlen(name);
	if (len >= LEN_SUFFIX)
		if (!strcmp(suffix, name + len - LEN_SUFFIX))
			return (TRUE);
	return (FALSE);
}

/*
 * Returns a pointer to the leaf (i.e., filename) of a path.
 */
static
char *
leaf_of(
_AR1(char *,	name))
_DCL(char *,	name)
{
	register char	*s = strrchr(name, '/');
	return ((s != 0) ? s+1 : name);
}

/*
 * Strips the RCS-suffix from a path (modifies/returns its argument)
 */
static
char *
strip_suffix(
_AR1(char *,	name))
_DCL(char *,	name)
{
	size_t	len = strlen(name);
	if (len >= LEN_SUFFIX)
		if (!strcmp(suffix, name + len - LEN_SUFFIX))
			name[len - LEN_SUFFIX] = EOS;
	return name;
}

/*
 * Compares two paths to see if they have the same leaf (ignoring suffix)
 */
static
int
same_name(
_ARX(char *,	name1)
_AR1(char *,	name2)
	)
_DCL(char *,	name1)
_DCL(char *,	name2)
{
	auto	char	leaf1[MAXPATHLEN],
			leaf2[MAXPATHLEN];

	return !strcmp(
		strip_suffix(strcpy(leaf1, leaf_of(name1))),
		strip_suffix(strcpy(leaf2, leaf_of(name2)))
			);
}

/*
 * Verify that a pathname is not too long, and properly formatted.
 */
static
int
ok_name(
_AR1(char *,	name))
_DCL(char *,	name)
{
	auto	size_t	len = strlen(name) + strlen(rcs_dir()) + LEN_SUFFIX + 2;
	static	char	delim	= '$';

	if (len > MAXPATHLEN) {
		FPRINTF(stderr, "? name too long:%s\n", name);
		return FALSE;
	}
	if (strchr(name, delim)) {
		FPRINTF(stderr, "? name contains '%c':%s\n", delim, name);
		return FALSE;
	}
	return TRUE;		/* ok I guess */
}

/*
 * Initializes this module
 */
static
void
initialize(_AR0)
{
	static	int	done;

	if (!done) {
		done = TRUE;

		*(name_working = doalloc((char *)0, MAXPATHLEN+1)) = EOS;
		*(name_archive = doalloc((char *)0, MAXPATHLEN+1)) = EOS;
		*(name_located = doalloc((char *)0, MAXPATHLEN+1)) = EOS;
	}
}

/*
 * Re-initializes (for use in 'rcsfilepair()')
 */
static
void
reinitialize(_AR0)
{
	initialize();
	have_working =
	have_archive =
	have_located = NOT_YET;
}

/*
 * Perform a 'stat()' when caller requests this for a particular pathname.
 * Note that this assumes 'stat()' returns zero on success, negative on error.
 */
static
void
stat_on_demand(
_ARX(char *,	path)
_ARX(STAT *,	sb)
_AR1(int *,	code)
	)
_DCL(char *,	path)
_DCL(STAT *,	sb)
_DCL(int *,	code)
{
	if (*code == NOT_YET) {	/* did I ask for this one yet? */
		if (*path)
			*code = stat(path, sb);
		else {
			FPRINTF(stderr, "? %s bug at line %d\n",
				__FILE__, __LINE__);
			*code = -1;
		}
	}
}

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/

/*
 * Functions that copy the corresponding name into caller's buffer, and return
 * 0 iff the object already exists (-1 otherwise).
 *
 * If the Stat-pointer is nonnull (the usual case), the corresponding STAT block
 * is copied also, so that the caller can do ownership/access testing.
 */
int
rcs_working(
_ARX(char *,	Name)
_AR1(STAT *,	Stat)
	)
_DCL(char *,	Name)
_DCL(STAT *,	Stat)
{
	initialize();
	stat_on_demand(name_working, &stat_working, &have_working);
	if (Stat != 0)	*Stat = stat_working;
	if (Name != 0)	(void)strcpy(Name, name_working);
	return have_working;
}

int
rcs_archive(
_ARX(char *,	Name)
_AR1(STAT *,	Stat)
	)
_DCL(char *,	Name)
_DCL(STAT *,	Stat)
{
	initialize();
	stat_on_demand(name_archive, &stat_archive, &have_archive);
	if (Stat != 0)	*Stat = stat_archive;
	if (Name != 0)	(void)strcpy(Name, name_archive);
	return have_archive;
}

int
rcs_located(
_ARX(char *,	Name)
_AR1(STAT *,	Stat)
	)
_DCL(char *,	Name)
_DCL(STAT *,	Stat)
{
	initialize();
	stat_on_demand(name_located, &stat_located, &have_located);
	if (Stat != 0)	*Stat = stat_located;
	if (Name != 0)	(void)strcpy(Name, name_located);
	return have_located;
}

/*
 * Function to invoke for argument-parsing.
 */
int
rcsfilepair(
_ARX(int,	This)
_ARX(int,	Last)	/* same as 'argc' */
_AR1(char **,	Argv)	/* assumed non-volatile, from 'main()' */
	)
_DCL(int,	This)
_DCL(int,	Last)
_DCL(char **,	Argv)
{
	auto	int	test_1,
			test_2;
	auto	char	*name_1,
			*name_2,
			*working = 0,
			*archive = 0,
			temp_archive[MAXPATHLEN];

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
	if (archive != 0 && !strchr(archive, '/'))
		archive = pathcat(temp_archive, rcs_dir(), archive);

	if (archive == 0) {

		TRACE(("...case 3\n"))
		(void)strcpy(name_working, working);

		(void)strcpy(name_archive, working);	/* copy path */
		FORMAT(leaf_of(name_archive), "%s/%s%s",
			rcs_dir(), leaf_of(working), RCS_SUFFIX);

	} else if (working == 0) {

		TRACE(("...case 2\n"))
		(void)strcpy(name_archive, archive);

		working = leaf_of(archive);
		FORMAT(name_working, "./%s", working);
		(void)strip_suffix(name_working);

	} else {

		TRACE(("...case 1\n"))
		(void)strcpy(name_working, working);
		(void)strcpy(name_archive, archive);

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
		(void)strcpy(name_1, ".");
	else
		name_1[-1] = EOS;

	return This;
}

#ifdef	TEST
/*ARGSUSED*/
_MAIN
{
	register int	j = 1;

	rcs_working(0,0);
	while (j < argc) {
		PRINTF("%3d] parameter:%s\n", j, argv[j]);
		j = rcsfilepair(j, argc, argv);
		PRINTF("    working   :%s\n", name_working);
		PRINTF("    archive   :%s\n", name_archive);
		PRINTF("    located   :%s\n", name_located);
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif	/* TEST */
