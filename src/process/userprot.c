#ifndef	lint
static	char	Id[] = "$Id: userprot.c,v 9.2 1991/09/12 08:00:55 dickey Exp $";
#endif

/*
 * Title:	userprot.c (set protection for setuid program's caller)
 * Author:	T.E.Dickey
 * Created:	30 Aug 1988
 * Modified:
 *		12 Sep 1991, removed redundant def for 'errno' (VMS C 3.2)
 *		11 Jul 1991, if effective-user is root, set ownership and act
 *			     within the current process.
 *		13 Sep 1988, rewrote using 'for_user()'
 *
 * Function:	For a set-uid program, sets protection (and modification time)
 *		for the user.  By forking, we can reset our uid; this is
 *		faster than an exec.
 *
 * Returns:	-1 if an error was found; sets 'errno'.
 */

#include	"ptypes.h"
#include	<errno.h>

static	char	*upr_name;
static	int	upr_mode;
static	time_t	upr_time;

static	
upr_func() { 
	if (chmod(upr_name, upr_mode) >= 0)
		(void)setmtime(upr_name, upr_time);
}

userprot(name, mode, mtime)
char	*name;
int	mode;
time_t	mtime;
{
	upr_name = name;
	upr_mode = mode;
	upr_time = mtime;
	if (!geteuid()) {
		if (chown(upr_name, getuid(), getgid()) < 0)
			return (-1);
		errno = 0;
		upr_func();
		return (errno != 0) ? -1 : 0;
	}
	return (for_user(upr_func));
}
