#ifndef	lint
static	char	*Id = "$Id: showuids.c,v 12.0 1993/04/29 10:15:57 ste_cm Rel $";
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
	FPRINTF(fp, "...uid=%d(%s)",		(int)getuid(),  uid2s((int)getuid()));
	if (geteuid() != getuid())
		FPRINTF(fp, ", euid=%d(%s)",	(int)geteuid(), uid2s((int)geteuid()));

	FPRINTF(fp, ", gid=%d(%s)",		(int)getgid(),  gid2s((int)getgid()));
	if (getegid() != getgid())
		FPRINTF(fp, ", egid=%d(%s)",	(int)getegid(), gid2s((int)getegid()));

	FPRINTF(fp, "\n");
}
