#ifndef	lint
static	char	Id[] = "$Id: sccs_dir.c,v 12.1 1993/09/21 18:54:03 dickey Exp $";
#endif

/*
 * Title:	sccs_dir.c (returns name of SCCS-directory)
 * Author:	T.E.Dickey
 * Created:	02 Sep 1988
 * Modified:
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		22 Jun 1990, changed default value from "sccs" to "SCCS" since
 *			     this seems to be the currently more-common usage.
 *		
 * Function:	Encapsulates the name of the SCCS directory, permitting the
 *		user to override our default by assigning an environment
 *		variable "SCCS_DIR".
 */

#include "ptypes.h"
#include "sccsdefs.h"

char *
sccs_dir(_AR0)
{
	static	char	*name;
	if (name == 0)	name = getenv("SCCS_DIR");
	if (name == 0)	name = "SCCS";
	return (name);
}
