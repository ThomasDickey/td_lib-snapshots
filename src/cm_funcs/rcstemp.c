#ifndef	lint
static	char	Id[] = "$Id: rcstemp.c,v 9.10 1991/10/04 12:25:04 dickey Exp $";
#endif

/*
 * Title:	rcstemp.c (rcs to temp-name)
 * Author:	T.E.Dickey
 * Created:	25 Aug 1988
 * Modified:
 *		04 Oct 1991, conversion to ANSI
 *		12 Sep 1991, removed redundant def for 'errno' (VMS C 3.2)
 *		11 Jul 1991, don't need temp-name if suid-root
 *		31 Oct 1989, account for present setting of 'umask'; use chown
 *			     to force the group of the created-directory
 *		24 Aug 1989, recoded to ensure that if the directory's group-id
 *			     is not consistent with the process's group-id, then
 *			     we must relax the directory protection.
 *		07 Apr 1989, if attempt to make tmp-directory more restrictive
 *			     fails, don't give up (after all, if we have world-
 *			     access, it still works).  Bypassed bug in apollo
 *			     SR9 (mkdir ignores mode) by explicitly setting
 *			     protection after doing 'mkdir()'.
 *		20 Mar 1989, create user-level directory under /tmp to avoid
 *			     conflicts between users.
 *		30 Aug 1988, invoke 'filecopy()' to make temp-file look more
 *			     like the working-file.
 *
 * Function:	If we are run in setuid-mode, we must be able to extract and
 *		otherwise manipulate the file into a place where 'co' and 'ci'
 *		will not fail because of 'access()' checks -- which checks
 *		the real uid, rather than resolving directory and file owners.
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	<errno.h>

#define	DEBUG	if (rcs_debug()) PRINTF

char *
rcstemp(
_ARX(char *,	working)
_AR1(int,	copy)
	)
_DCL(char *,	working)
_DCL(int,	copy)
{
	static	char	tmp[BUFSIZ];

	if (geteuid() != getuid()
	 && geteuid() != 0) {
		char	*tf = pathcat(tmp, "/tmp", uid2s((int)getuid()));
		int	mode = ((getgid() == getegid()) ? 0775 : 0777);
		struct	stat	sb;

		DEBUG("mode:%o gid:%d/%d\n", mode, getgid(),getegid());
		if (stat(tf, &sb) < 0) {
			int	oldmask = umask(0);
			DEBUG(".. mkdir %s (mode=%o)\n", tf, mode);
			if (mkdir(tf, mode) < 0) {
				failed(tf);
				/*NOTREACHED*/
			}
			if (chown(tf, (int)geteuid(), (int)getegid()) < 0){
				failed("chown");
				/*NOTREACHED*/
			}
			(void)umask(oldmask);
		} else {
			if ((sb.st_mode & S_IFMT) != S_IFDIR) {
				errno = ENOTDIR;
				failed(tf);
				/*NOTREACHED*/
			}
			DEBUG("group:%d\n", sb.st_gid);
			if (getgid() != sb.st_gid)
				mode = 0777;
			if ((sb.st_mode &= 0777)   != mode) {
				DEBUG(".. chmod %o %s (was %o)\n",
						mode, tf, sb.st_mode);
				if (chmod(tf, mode) < 0) {
					if (sb.st_mode != 0777) {
						failed(tf);
						/*NOTREACHED*/
					}
				}
			}
		}

		tf = pathcat(tmp, tf, pathleaf(working));
		if (filecopy(working, tf, copy) < 0) {
			failed(tf);
			/*NOTREACHED*/
		}
		working = tf;
	}
	return (working);
}
