#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: rcsname.c,v 12.2 1993/10/29 17:35:24 dickey Exp $";
#endif

/*
 * Title:	rcsname.c (derive name of RCS file)
 * Author:	T.E.Dickey
 * Created:	27 May 1988
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		18 Apr 1990, added 'full' argument to 'rcs2name()' and
 *			     'name2rcs()' so we can force the conversion to
 *			     preserve pathnames, rather than coercing the
 *			     conversion toward the current directory.  Added
 *			     to test-case also.
 *		04 Oct 1989, lint (apollo SR10.1)
 *		02 Sep 1988, use 'rcs_dir()'.
 *		05 Aug 1988, rewrote to provide bidirectional translation, to
 *			     make 'checkin' and 'checkout' more foolproof.
 *
 * Function:	Performs translation between RCS archive and working filenames,
 *		according to the following rules:
 *
 *		to RCS-file:
 *			{path/}name	=> RCS/name,v
 *		to working-file:
 *			{path/}name,v	=> name
 *
 *		If a name ends with RCS_SUFFIX, always assume that it is an
 *		RCS-file, even if no RCS-directory is specified.  Similarly,
 *		if it does not, it must be a working-file.
 *
 *		These rules are compatible with the assumptions built into
 *		'ci' and 'co'.
 *
 *		These functions return a pointer to static storage which is
 *		overwritten by subsequent calls.  The arguments are unmodified.
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	"rcsdefs.h"

#include	<ctype.h>

#define	LEN_SUFFIX	(sizeof(suffix)-1)

static	char	suffix[] = RCS_SUFFIX;

/************************************************************************
 *	local procedures						*
 ************************************************************************/

/*
 * Returns TRUE if the name ends with RCS_SUFFIX.
 */
static
int	rcs_suffix(
	_AR1(char *,	name))
	_DCL(char *,	name)
{
	int	len = strlen(name);
	if (len >= LEN_SUFFIX)
		if (!strcmp(suffix, name + len - LEN_SUFFIX))
			return (TRUE);
	return (FALSE);
}

static
void	trim_leaf(
	_AR1(char *,	name))
	_DCL(char *,	name)
{
	register char *s = strrchr(name, '/');
	if (s != 0) name = s;
	*name = EOS;
}

static
char *
leaf(
_AR1(char *,	name))
_DCL(char *,	name)
{
	register char	*s = strrchr(name, '/');
	return ((s != 0) ? s+1 : name);
}

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/

/*
 * Given the name of either the working file, or the RCS-file, obtain the name
 * of the working file.
 */
char *
rcs2name(
_ARX(char *,	name)
_AR1(int,	full)
	)
_DCL(char *,	name)
_DCL(int,	full)
{
	char	*s, *t;
static	char	fname[BUFSIZ];

	if (rcs_suffix(name)) {
		int	len = strlen(strcpy(fname, name));

		fname[len - LEN_SUFFIX] = EOS;
		if ((s = leaf(fname)) > fname) {
			char	*d = fname;
			if (full) {
				s[-1] = EOS;
				if (((t = leaf(d)) > d)
				&&  sameleaf(t, rcs_dir()))
					d = t;
			}
			while ((*d++ = *s++) != EOS)
				;
		}
	} else {
		(void)strcpy(fname, name);
	}
	return (fname);
}

/*
 * Given the name of either the working file, or the RCS-file, obtain the name
 * of the RCS-file.
 */
char *
name2rcs(
_ARX(char *,	name)
_AR1(int,	full)
	)
_DCL(char *,	name)
_DCL(int,	full)
{
static	char	fname[BUFSIZ];

	if (rcs_suffix(name)) {
		(void)strcpy(fname, name);
	} else {
		if (full) {
			trim_leaf(strcpy(fname, name));
			if (sameleaf(fname, rcs_dir()))
				trim_leaf(fname);
			if (*fname)
				(void)strcat(fname, "/");
		} else
			*fname = EOS;
		(void)strcat(
			strcat(
				strcat(
					strcat(fname, rcs_dir()),
					"/"),
				leaf(name)),
			suffix);
	}
	return (fname);
}

#ifdef	TEST
void	do_test(
	_arx(int,	argc)
	_arx(char **,	argv)
	_ar1(int,	full));

void	do_test(
	_ARX(int,	argc)
	_ARX(char **,	argv)
	_AR1(int,	full)
		)
	_DCL(int,	argc)
	_DCL(char **,	argv)
	_DCL(int,	full)
{
	int	j;
	char	old[BUFSIZ], *new;

	if (argc > 1) {
		printf("** %s-path\n", full ? "full" : "local");
		printf("name2rcs:\n");
		for (j = 1; j < argc; j++) {
			(void)strcpy(old, name2rcs(argv[j], !full));
			new = name2rcs(argv[j], full);
			printf("  %-20s => %s%s\n",
				argv[j], new,
				strcmp(old, new) ? " (*)" : "");
		}
		printf("rcs2name:\n");
		for (j = 1; j < argc; j++) {
			(void)strcpy(old, rcs2name(argv[j], !full));
			new = rcs2name(argv[j], full);
			printf("  %-20s => %s%s\n",
				argv[j], new,
				strcmp(old, new) ? " (*)" : "");
		}
	} else {
		static	char	*test[] = {
			"?",
			"name",			"name,v",
			"path/name",		"path/name,v",
			"path/RCS/name",	"path/RCS/name,v",
			"RCS/name",		"RCS/name,v"
		};
		do_test (SIZEOF(test), test, full);
	}
}

_MAIN
{
	do_test(argc, argv, FALSE);
	do_test(argc, argv, TRUE);
	exit(SUCCESS);
}
#endif
