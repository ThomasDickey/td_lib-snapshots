/*
 * Title:	show_uids.c
 * Author:	T.E.Dickey
 * Created:	07 Feb 1992
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	prints the real/effective uid+gid in both numeric and symbolic
 *		form to the given file-pointer.  This is used in debug-traces
 *		for set-uid programs.
 */

#include "ptypes.h"

MODULE_ID("$Id: showuids.c,v 12.3 1993/11/27 14:03:02 tom Exp $")

#ifdef	unix
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

#endif	/* unix */
