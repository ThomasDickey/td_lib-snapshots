#ifndef	lint
static	char	Id[] = "$Id: vms_wild.c,v 7.0 1991/05/20 17:18:16 ste_cm Rel $";
#endif

/*
 * Title:	vms_iswild.c (vms wildcard-test)
 * Author:	T.E.Dickey
 * Created:	06 Oct 1989
 *
 * Function:	Tests the given pathname to see if it contains any of the VMS
 *		wildcard indicators.  If so, it returns true.
 */
#define	STR_PTYPES
#include	"portunix.h"

vms_iswild(name)
char	*name;
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
