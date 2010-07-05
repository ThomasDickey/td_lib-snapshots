/*
 * Title:	filesize.c
 * Author:	T.E.Dickey
 * Created:	13 Sep 1991
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		06 Feb 1992, use 'stat_file()'
 *		04 Oct 1991, conversion to ANSI
 *
 * Function:	returns a file's size (in bytes), or -1 if none is available.
 */

#include "ptypes.h"

MODULE_ID("$Id: filesize.c,v 12.5 2010/07/04 10:27:36 tom Exp $")

off_t
filesize(const char *name)
{
    Stat_t sb;

    return (stat_file(name, &sb) < 0) ? -1 : sb.st_size;
}
