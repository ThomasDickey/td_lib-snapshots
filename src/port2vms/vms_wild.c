#ifndef	NO_IDENT
static	char	Id[] = "$Id: vms_wild.c,v 12.1 1994/08/21 18:40:53 tom Exp $";
#endif

/*
 * Title:	vms_iswild.c (vms wildcard-test)
 * Author:	T.E.Dickey
 * Created:	06 Oct 1989
 * Modified:
 *		01 Dec 1993, ifdefs.
 *		22 Sep 1993, gcc warnings
 *		20 Nov 1992, use prototypes.  Added test-driver.
 *
 * Function:	Tests the given pathname to see if it contains any of the VMS
 *		wildcard indicators.  If so, it returns true.
 */

#define	STR_PTYPES
#include	"port2vms.h"

int	vms_iswild(
	_AR1(char *,	name))
	_DCL(char *,	name)
{
	if (strchr(name, '*') != 0
	||  strchr(name, '?') != 0)
		return (TRUE);
	if ((name = strchr(name, '[')) != NULL) {
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
#endif	/* TEST */
