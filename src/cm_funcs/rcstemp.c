#ifndef	lint
static	char	Id[] = "$Id: rcstemp.c,v 8.0 1989/12/07 15:21:19 ste_cm Rel $";
#endif	lint

/*
 * Title:	rcstemp.c (rcs to temp-name)
 * Author:	T.E.Dickey
 * Created:	25 Aug 1988
 * $Log: rcstemp.c,v $
 * Revision 8.0  1989/12/07 15:21:19  ste_cm
 * BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *
 *		Revision 7.0  89/12/07  15:21:19  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/12/07  15:21:19  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.2  89/12/07  15:21:19  dickey
 *		lint (SunOs 3.4, Apollo SR10.1)
 *		
 *		Revision 5.1  89/10/31  13:32:06  dickey
 *		account for present setting of 'umask'; use chown to force
 *		the group of the created-directory
 *		
 *		Revision 5.0  89/10/04  13:09:32  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.2  89/10/04  13:09:32  dickey
 *		lint (apollo SR10.1)
 *		
 *		Revision 4.1  89/08/24  16:06:36  dickey
 *		recoded to ensure that if the directory's group-id is not
 *		consistent with the process's group-id, then we must relax
 *		the directory protection.
 *		
 *		Revision 4.0  89/04/07  16:26:34  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  89/04/07  16:26:34  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  89/04/07  16:26:34  ste_cm
 *		BASELINE Fri Apr  7 16:41:37 EDT 1989
 *		
 *		Revision 1.6  89/04/07  16:26:34  dickey
 *		if attempt to make tmp-directory more restrictive fails, don't
 *		give up (after all, if we have world-access, it still works).
 *		
 *		Revision 1.5  89/04/07  16:12:49  dickey
 *		bypassed bug in apollo SR9 (mkdir ignores mode) by explicitly
 *		setting protection after doing 'mkdir()'.
 *		
 *		Revision 1.4  89/03/20  09:31:04  dickey
 *		sccs2rcs keywords
 *		
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
extern	int	errno;
extern	char	*uid2s();
extern	char	*pathcat();
extern	char	*pathleaf();

#define	DEBUG	if (rcs_debug()) PRINTF

char *
rcstemp(working, copy)
char	*working;
{
	static	char	tmp[BUFSIZ];

	if (getuid() != geteuid()) {
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
