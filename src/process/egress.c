/*
 * Title:	egress.c ("access" for effective-user)
 * Author:	T.E.Dickey
 * Created:	12 Nov 1994
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *
 * Function:	This duplicates the 'access()' function, with one difference:
 *		the checks are made for the effective user (i.e., whatever the
 *		process is setuid'd to).
 */

#define ACC_PTYPES
#define ERR_PTYPES
#include <ptypes.h>

MODULE_ID("$Id: egress.c,v 12.8 2014/12/28 01:11:01 tom Exp $")

int
egress(char *pathname, int mode)
{
    Stat_t sb;

    if (stat(pathname, &sb) < 0) {
	return -1;
    } else if (mode == 0) {
	return 0;
    } else {
	uid_t uid = geteuid();
	gid_t gid = getegid();
	unsigned mask;
	unsigned umode = (unsigned) mode;

	if (in_group(sb.st_gid))
	    gid = sb.st_gid;

	if (!uid) {		/* root works with u+o+g rights */
	    if (umode & X_OK) {
		if ((sb.st_mode & 0111) != 0)
		    return 0;
		mask = 0;	/* force error-return */
	    } else {		/* root can r/w anything */
		return 0;
	    }
	} else if (uid == sb.st_uid) {
	    mask = umode << 6;
	} else if (gid == sb.st_gid) {
	    mask = umode << 3;
	} else {
	    mask = umode;
	}
	if ((sb.st_mode & mask) == mask)
	    return 0;
    }
    errno = EACCES;
    return -1;
}

/******************************************************************************/
#ifdef	TEST
_MAIN
{
    (void) argc;
    (void) argv;
    exit(EXIT_FAILURE);
    /*NOTREACHED */
}
#endif /* TEST */
