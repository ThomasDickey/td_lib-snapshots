/*
 * Title:	in_group.c (test if given group is in user's group-list)
 * Author:	T.E.Dickey
 * Created:	12 Nov 1994
 *
 * Function:	Given a group, this function tests if the user belongs to that
 *		access group.
 */

#include <ptypes.h>
#include <grp.h>

MODULE_ID("$Id: in_group.c,v 12.6 2002/07/05 11:22:40 tom Exp $")

#if defined(HAVE_GETGROUPS)
#  if defined(HAVE_SYS_PARAM_H)
#    include <sys/param.h>
#  endif
#  if defined(HAVE_LIMITS_H)
#    include <limits.h>
#  endif
#endif

int	in_group (
	_AR1(int,	given_gid))
	_DCL(int,	given_gid)
{
#ifdef	HAVE_GETGROUPS
	register int i;
	static int ngroups = -1;
#ifdef	NGROUPS_MAX
	static GETGROUPS_T groups[NGROUPS_MAX];
#define	ngroups_max	NGROUPS_MAX
#else
	static GETGROUPS_T *groups = 0;
	static int ngroups_max;
	if (groups == 0)
	{
		ngroups_max = NGROUPS_MAX;
		groups = (GETGROUPS_T *) malloc (ngroups_max * sizeof (GETGROUPS_T));
	}
#endif
	if (groups != 0 && ngroups == -1)
		ngroups = getgroups (ngroups_max, groups);
#endif	/* Have getgroups.  */

#ifdef	HAVE_GETGROUPS
	for (i = 0; i < ngroups; ++i)
		if ((int) groups[i] == given_gid)
			return TRUE;
#else
	if (given_gid == getgid())
		return TRUE;
#endif	/* Have getgroups.  */
	return FALSE;
}
