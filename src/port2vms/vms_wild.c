#ifndef	lint
static	char	Id[] = "$Id: vms_wild.c,v 8.0 1992/11/20 11:37:34 ste_cm Rel $";
#endif

/*
 * Title:	vms_iswild.c (vms wildcard-test)
 * Author:	T.E.Dickey
 * Created:	06 Oct 1989
 * Modified:
 *		20 Nov 1992, use prototypes.  Added test-driver.
 *
 * Function:	Tests the given pathname to see if it contains any of the VMS
 *		wildcard indicators.  If so, it returns true.
 */
#define	STR_PTYPES
#include	"portunix.h"

int	vms_iswild(
	_AR1(char *,	name))
	_DCL(char *,	name)
{
	if (strchr(name, '*') != 0
	||  strchr(name, '?') != 0)
		return (TRUE);
	if (name = strchr(name, '[')) {
		while (*name != EOS && *name != ']')
			if (!strncmp(name++, "...", 3))
				return (TRUE);
	}
	return (FALSE);
}

#ifdef	TEST
static
void	do_test(
	_AR1(char *,	path))
	_DCL(char *,	path)
{
	char	tmp[MAXPATHLEN];
	PRINTF("%s => %d\n", path, vms_iswild(path));
}

_MAIN
{
	register int	j;
	if (argc > 1)
		for (j = 1; j < argc; j++)
			do_test(argv[j]);
	else {
		static	char	*tbl[] = {
			"foo",
			"foo.*",
			"foo?",
			"dev:[bar...]foo",
			};
		for (j = 0; j < SIZEOF(tbl); j++)
			do_test(tbl[j]);
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
