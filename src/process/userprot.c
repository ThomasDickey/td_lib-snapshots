/*
 * Title:	userprot.c (set protection for setuid program's caller)
 * Author:	T.E.Dickey
 * Created:	30 Aug 1988
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		04 Oct 1991, conversion to ANSI
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

MODULE_ID("$Id: userprot.c,v 12.6 1995/04/22 21:09:55 tom Exp $")

#ifdef	unix

static	char	*upr_name;
static	mode_t	upr_mode;
static	time_t	upr_time;

static	
void	upr_func(_AR0) { 
	if (chmod(upr_name, upr_mode) >= 0)
		(void)setmtime(upr_name, upr_time, (time_t)0);
}

int	userprot(
	_ARX(char *,	name)
	_ARX(int,	mode)
	_AR1(time_t,	mtime)
		)
	_DCL(char *,	name)
	_DCL(int,	mode)
	_DCL(time_t,	mtime)
{
	upr_name = name;
	upr_mode = mode;
	upr_time = mtime;
	if (!geteuid()) {
		if (chown(upr_name, (int)getuid(), (int)getgid()) < 0)
			return (-1);
		errno = 0;
		upr_func();
		return (errno != 0) ? -1 : 0;
	}
	return (for_user(upr_func));
}

#endif	/* unix */
