#ifndef	lint
static	char	Id[] = "$Id: rcs_dir.c,v 12.1 1993/09/21 18:54:03 dickey Exp $";
#endif

/*
 * Title:	rcs_dir.c (returns name of RCS-directory)
 * Author:	T.E.Dickey
 * Created:	02 Sep 1988
 * Modified:
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *		
 *		Revision 8.1  91/05/15  10:04:43  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 * Function:	Encapsulates the name of the RCS directory, permitting the
 *		user to override our default by assigning an environment
 *		variable "RCS_DIR".
 */

#include "ptypes.h"
#include "rcsdefs.h"

char *
rcs_dir(_AR0)
{
	static	char	*name;
	if (name == 0)	name = getenv("RCS_DIR");
	if (name == 0)	name = "RCS";
	return (name);
}
