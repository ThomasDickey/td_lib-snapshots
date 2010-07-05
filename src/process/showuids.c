/*
 * Title:	show_uids.c
 * Author:	T.E.Dickey
 * Created:	07 Feb 1992
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	prints the real/effective uid+gid in both numeric and symbolic
 *		form to the given file-pointer.  This is used in debug-traces
 *		for set-uid programs.
 */

#include "ptypes.h"

MODULE_ID("$Id: showuids.c,v 12.6 2010/07/04 15:26:46 tom Exp $")

#ifdef	SYS_UNIX
void
show_uids(FILE *fp)
{
    FPRINTF(fp, "...uid=%d(%s)", (int) getuid(), uid2s(getuid()));
    if (geteuid() != getuid())
	FPRINTF(fp, ", euid=%d(%s)", (int) geteuid(), uid2s(geteuid()));

    FPRINTF(fp, ", gid=%d(%s)", (int) getgid(), gid2s(getgid()));
    if (getegid() != getgid())
	FPRINTF(fp, ", egid=%d(%s)", (int) getegid(), gid2s(getegid()));

    FPRINTF(fp, "\n");
}

#endif /* SYS_UNIX */
