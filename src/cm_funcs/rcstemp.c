/*
 * Title:	rcstemp.c (rcs to temp-name)
 * Author:	T.E.Dickey
 * Created:	25 Aug 1988
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		21 Dec 1992, RCS 5 on Posix setuid does not need temp-dir
 *		06 Feb 1992, use 'stat_dir()'
 *		22 Oct 1991, ensure that we unlink the temp-file if it already
 *			     exists.
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
#include "ptypes.h"
#include "rcsdefs.h"
#include <errno.h>

MODULE_ID("$Id: rcstemp.c,v 12.7 1994/07/16 15:25:20 tom Exp $")

#define	DEBUG	if (RCS_DEBUG) PRINTF

#define	CHMOD_MASK	0777
#define	WORLD_OPEN	0777
#define	GROUP_OPEN	0775

char *	rcstemp(
	_ARX(char *,	working)
	_AR1(int,	copy)
		)
	_DCL(char *,	working)
	_DCL(int,	copy)
{
	static	char	tmp[BUFSIZ];

	if (geteuid() != getuid()
	 && geteuid() != 0
#if	RCS_VERSION >= 5
	 && !saves_uid()
#endif
	 ) {
		char	*tf = pathcat(tmp, "/tmp", uid2s((int)getuid()));
		int	mode = ((getgid() == getegid())
				? GROUP_OPEN
				: WORLD_OPEN);
		Stat_t	sb;

		DEBUG(".. rcstemp mode is %o gid:%d(%s) egid:%d(%s)\n",
			mode,
			(int)getgid(),	gid2s((int)getgid()),
			(int)getegid(),	gid2s((int)getegid()));

		if (stat_dir(tf, &sb) < 0) {
			int	oldmask = umask(0);

			DEBUG("%% mkdir %s\n", tf);
			if (mkdir(tf, (mode_t)mode) < 0) {
				failed(tf);
				/*NOTREACHED*/
			}

			/* make sure we get the right group */
			if (chown(tf, (int)geteuid(), (int)getegid()) < 0){
				failed("chown");
				/*NOTREACHED*/
			}

			(void)umask((mode_t)oldmask);
		} else {
			DEBUG(".. %s group is %d(%s)\n",
				tf,
				(int)(sb.st_gid),
				gid2s((int)(sb.st_gid)));

			if (getgid() != sb.st_gid)
				mode = WORLD_OPEN;

			if ((sb.st_mode &= CHMOD_MASK) != mode) {
				DEBUG("%% chmod %o %s (was %o)\n",
						mode, tf,
						(int)(sb.st_mode));
				if (chmod(tf, (mode_t)mode) < 0) {
					if (sb.st_mode != WORLD_OPEN) {
						failed(tf);
						/*NOTREACHED*/
					}
				}
			}
		}

		tf = pathcat(tmp, tf, pathleaf(working));
		(void)unlink(tf);
		if (filecopy(working, tf, copy) < 0) {
			failed(tf);
			/*NOTREACHED*/
		}
		working = tf;
	}
	return (working);
}
