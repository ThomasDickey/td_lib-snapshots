#ifndef	lint
static	char	sccs_id[] = "@(#)rcsname.c	1.2 88/08/05 14:24:29";
#endif	lint

/*
 * Title:	rcsname.c (derive name of RCS file)
 * Author:	T.E.Dickey
 * Created:	27 May 1988
 * Modified:
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

#include	"rcsdefs.h"

#include	<stdio.h>
#include	<ctype.h>
extern	char	*strcat();
extern	char	*strcpy();
extern	char	*strrchr();

#define	TRUE	(1)
#define	FALSE	(0)
#define	EOS	'\0'

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
char *
leaf(name)
char	*name;
{
	char	*s = strrchr(name, '/');
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
rcs2name(name)
char	*name;
{
	char	*s, *t;
static	char	fname[BUFSIZ];

	if (rcs_suffix(name)) {
		int	len = strlen(strcpy(fname, name));

		fname[len - LEN_SUFFIX] = EOS;
		if ((s = leaf(fname)) > fname)
			for (t = fname; *t++ = *s++;);
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
name2rcs(name)
char	*name;
{
static	char	fname[BUFSIZ];

	if (rcs_suffix(name)) {
		(void)strcpy(fname, name);
	} else {
		(void)strcat(
			strcat(
				strcat(
					strcpy(fname, RCS_DIR),
					"/"),
				leaf(name)),
			suffix);
	}
	return (fname);
}

#ifdef	TEST
main(argc, argv)
char	*argv[];
{
	int	j;
	if (argc > 1) {
		printf("name2rcs:\n");
		for (j = 1; j < argc; j++) {
			printf("  %-20s => %s\n", argv[j], name2rcs(argv[j]));
		}
		printf("rcs2name:\n");
		for (j = 1; j < argc; j++) {
			printf("  %-20s => %s\n", argv[j], rcs2name(argv[j]));
		}
	} else {
		static	char	*test[] = {
			"?",
			"name",			"name,v",
			"path/name",		"path/name,v",
			"path/RCS/name",	"path/RCS/name,v",
			"RCS/name",		"RCS/name,v"
		};
		(void) main (sizeof(test)/sizeof(test[0]), test);
	}
	return(0);
}
#endif	TEST
