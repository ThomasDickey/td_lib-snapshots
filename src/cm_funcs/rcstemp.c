#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/cm_funcs/RCS/rcstemp.c,v 1.4 1989/03/20 09:31:04 dickey Exp $";
#endif	lint

/*
 * Title:	rcstemp.c (rcs to temp-name)
 * Author:	T.E.Dickey
 * Created:	25 Aug 1988
 * $Log: rcstemp.c,v $
 * Revision 1.4  1989/03/20 09:31:04  dickey
 * sccs2rcs keywords
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

#include	<stdio.h>
#include	<errno.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/stat.h>
extern	int	errno;
extern	char	*uid2s();
extern	char	*pathcat();
extern	char	*pathleaf();

char *
rcstemp(working, copy)
char	*working;
{
	static	char	tmp[BUFSIZ];

	if (getuid() != geteuid()) {
		char	*tf = pathcat(tmp, "/tmp", uid2s(getuid()));
		int	mode = getgid() == getegid() ? 0775 : 0777;
		struct	stat	sb;

		if (stat(tf, &sb) < 0) {
			if (mkdir(tf, mode) < 0)
				failed(tf);
		} else if ((sb.st_mode & S_IFMT) != S_IFDIR) {
			errno = ENOTDIR;
			failed(tf);
		} else if ((sb.st_mode & 0777)   != mode) {
			if (chmod(tf, mode) < 0)
				failed(tf);
		}

		tf = pathcat(tmp, tf, pathleaf(working));
		if (filecopy(working, tf, copy) < 0)
			failed(tf);
		working = tf;
	}
	return (working);
}
