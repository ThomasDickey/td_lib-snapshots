#ifndef	lint
static	char	Id[] = "$Id: rcsname.c,v 9.0 1991/05/15 09:41:26 ste_cm Rel $";
#endif

/*
 * Title:	rcsname.c (derive name of RCS file)
 * Author:	T.E.Dickey
 * Created:	27 May 1988
 * $Log: rcsname.c,v $
 * Revision 9.0  1991/05/15 09:41:26  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  09:41:26  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  90/04/18  13:55:07  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  90/04/18  13:55:07  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.1  90/04/18  13:55:07  dickey
 *		added 'full' argument to 'rcs2name()' and 'name2rcs()' so we
 *		can force the conversion to preserve pathnames, rather than
 *		coercing the conversion toward the current directory.  Added
 *		to test-case also.
 *		
 *		Revision 6.0  89/10/04  12:14:54  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/10/04  12:14:54  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.1  89/10/04  12:14:54  dickey
 *		lint (apollo SR10.1)
 *		
 *		Revision 4.0  88/09/02  09:28:32  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/09/02  09:28:32  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/09/02  09:28:32  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.4  88/09/02  09:28:32  dickey
 *		sccs2rcs keywords
 *		
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
extern	char	*rcs_dir();

#define	LEN_SUFFIX	(sizeof(suffix)-1)

static	char	suffix[] = RCS_SUFFIX;

/************************************************************************
 *	local procedures						*
 ************************************************************************/

/*
 * Returns TRUE if the name ends with RCS_SUFFIX.
 */
static
rcs_suffix(name)
char	*name;
{
	int	len = strlen(name);
	if (len >= LEN_SUFFIX)
		if (!strcmp(suffix, name + len - LEN_SUFFIX))
			return (TRUE);
	return (FALSE);
}

static
trim_leaf(name)
char	*name;
{
	register char *s = strrchr(name, '/');
	if (s != 0) name = s;
	*name = EOS;
}

static
char *
leaf(name)
char	*name;
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
rcs2name(name,full)
char	*name;
int	full;
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
			while (*d++ = *s++);
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
name2rcs(name,full)
char	*name;
int	full;
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
do_test(argc, argv, full)
char	*argv[];
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
		do_test (sizeof(test)/sizeof(test[0]), test, full);
	}
}

main(argc, argv)
char	*argv[];
{
	do_test(argc, argv, FALSE);
	do_test(argc, argv, TRUE);
	exit(0);
}
#endif
