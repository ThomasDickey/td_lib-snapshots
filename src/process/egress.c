/*
 * Title:	egress.c ("access" for effective-user)
 * Author:	T.E.Dickey
 * Created:	12 Nov 1994
 *
 * Function:	This duplicates the 'access()' function, with one difference:
 *		the checks are made for the effective user (i.e., whatever the
 *		process is setuid'd to).
 */

#define ACC_PTYPES
#define ERR_PTYPES
#include <ptypes.h>

MODULE_ID("$Id: egress.c,v 12.2 1994/11/12 22:33:11 tom Exp $")

int	egress(
	_ARX(char *,	pathname)
	_AR1(int,	mode)
		)
	_DCL(char *,	pathname)
	_DCL(int,	mode)
{
	Stat_t	sb;

	if (stat(pathname, &sb) < 0) {
		return -1;
	} else if (mode == 0) {
		return 0;
	} else {
		int	uid = geteuid();
		int	gid = getegid();
		unsigned mask;

		if (in_group(sb.st_gid))
			gid = sb.st_gid;

		if (!uid) {		/* root works with u+o+g rights */
			if (mode & X_OK) {
				if ((sb.st_mode & 0111) != 0)
					return 0;
				mask = 0;	/* force error-return */
			} else {	/* root can r/w anything */
				return 0;
			}
		} else if (uid == sb.st_uid) {
			mask = mode << 6;
		} else if (gid == sb.st_gid) {
			mask = mode << 3;
		} else {
			mask = mode;
		}
		if ((sb.st_mode & mask) == mask)
			return 0;
	}
	errno = EACCES;
	return -1;
}
