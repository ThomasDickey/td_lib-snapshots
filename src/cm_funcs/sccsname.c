#ifndef	lint
static	char	Id[] = "$Id: sccsname.c,v 12.1 1993/09/21 18:54:03 dickey Exp $";
#endif

/*
 * Title:	sccsname.c (derive name of SCCS file)
 * Author:	T.E.Dickey
 * Created:	08 May 1990
 * Modified:
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

#define	LEN_PREFIX	(sizeof(prefix)-1)

static	char	prefix[] = SCCS_PREFIX;

/************************************************************************
 *	local procedures						*
 ************************************************************************/

static
char *	leaf(
	_AR1(char *,	name))
	_DCL(char *,	name)
{
	register char	*s = strrchr(name, '/');
	return ((s != 0) ? s+1 : name);
}

/*
 * Returns TRUE if the name begins with SCCS_PREFIX.
 */
static
int	sccs_prefix(
	_AR1(char *,	name))
	_DCL(char *,	name)
{
	register char	*s = leaf(name);
	return (strlen(s) > LEN_PREFIX && !strncmp(s, prefix, LEN_PREFIX));
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

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/

/*
 * Given the name of either the working file, or the SCCS-file, obtain the name
 * of the working file.
 */
char *
sccs2name(
_ARX(char *,	name)
_AR1(int,	full)
	)
_DCL(char *,	name)
_DCL(int,	full)
{
	char	*s, *t;
static	char	fname[BUFSIZ];

	if (sccs_prefix(name)) {

		s = leaf(strcpy(fname, name));
		t = leaf(name) + LEN_PREFIX;
		while ((*s++ = *t++) != EOS)
			;

		if ((s = leaf(fname)) > fname) {
			char	*d = fname;
			if (full) {
				s[-1] = EOS;
				if (((t = leaf(d)) > d)
				&&  sameleaf(t, sccs_dir()))
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
 * Given the name of either the working file, or the SCCS-file, obtain the name
 * of the SCCS-file.
 */
char *
name2sccs(
_ARX(char *,	name)
_AR1(int,	full)
	)
_DCL(char *,	name)
_DCL(int,	full)
{
static	char	fname[BUFSIZ];

	if (sccs_prefix(name)) {
		(void)strcpy(fname, name);
	} else {
		if (full) {
			trim_leaf(strcpy(fname, name));
			if (sameleaf(fname, sccs_dir()))
				trim_leaf(fname);
			if (*fname)
				(void)strcat(fname, "/");
		} else
			*fname = EOS;
		(void)strcat(
			strcat(
				strcat(
					strcat(fname, sccs_dir()),
					"/"),
					prefix),
			leaf(name));
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
		printf("name2sccs:\n");
		for (j = 1; j < argc; j++) {
			(void)strcpy(old, name2sccs(argv[j], !full));
			new = name2sccs(argv[j], full);
			printf("  %-20s => %s%s\n",
				argv[j], new,
				strcmp(old, new) ? " (*)" : "");
		}
		printf("sccs2name:\n");
		for (j = 1; j < argc; j++) {
			(void)strcpy(old, sccs2name(argv[j], !full));
			new = sccs2name(argv[j], full);
			printf("  %-20s => %s%s\n",
				argv[j], new,
				strcmp(old, new) ? " (*)" : "");
		}
	} else {
		static	char	*test[] = {
			"?",
			"name",			"s.name",
			"path/name",		"path/s.name",
			"path/SCCS/name",	"path/SCCS/s.name",
			"SCCS/name",		"SCCS/s.name"
		};
		(void)putenv("SCCS_DIR=SCCS");
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
