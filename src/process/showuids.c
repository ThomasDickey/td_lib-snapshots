#ifndef	lint
static	char	*Id = "$Id: showuids.c,v 11.0 1992/02/07 08:13:58 ste_cm Rel $";
#endif

/*
 * Title:	show_uids.c
 * Author:	T.E.Dickey
 * Created:	07 Feb 1992
 *
 * Function:	prints the real/effective uid+gid in both numeric and symbolic
 *		form to the given file-pointer.  This is used in debug-traces
 *		for set-uid programs.
 */

#include "ptypes.h"

void
show_uids(
_AR1(FILE *,	fp))
_DCL(FILE *,	fp)
{
	FPRINTF(fp, "...uid=%d(%s)",		getuid(),  uid2s(getuid()));
	if (geteuid() != getuid())
		FPRINTF(fp, ", euid=%d(%s)",	geteuid(), uid2s(geteuid()));

	FPRINTF(fp, ", gid=%d(%s)",		getgid(),  gid2s(getgid()));
	if (getegid() != getgid())
		FPRINTF(fp, ", egid=%d(%s)",	getegid(), gid2s(getegid()));

	FPRINTF(fp, "\n");
}
