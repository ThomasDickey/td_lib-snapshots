#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: add2fnam.c,v 12.3 1993/11/27 16:11:44 dickey Exp $";
#endif

/*
 * Title:	add2fname.c
 * Author:	T.E.Dickey
 * Created:	05 Jun 1989
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		03 Oct 1991, converted to ANSI
 *
 * Function:	Add a suffix to a filename, if it is not already there.
 *		On VMS, we preserve the version-number, if it is given.
 *
 * Returns:	TRUE if the suffix is not present; in this case we append it
 *		to the filename-buffer.
 */

#define	STR_PTYPES
#include "ptypes.h"

int	add2fname(
	_ARX(char *,	name)
	_AR1(char *,	suffix)
		)
	_DCL(char *,	name)
	_DCL(char *,	suffix)
{
	register char	*s;
#ifdef	vms
	if (s = fleaf(name))
		name = s;
	if (s = strrchr(name, ';')) {
		auto	char	version[256];
		(void)strcpy(version, s);
		*s = EOS;
		if (add2fname(name, suffix)) {
			(void)strcat(name, version);
			return (TRUE);
		} else {
			(void)strcpy(name, version);
			return (FALSE);
		}
	}
#else	/* unix or MSDOS */
	if ((s = fleaf(name)) != 0)
		name = s;
#endif	/* vms/unix */
	if (strlen(name) > strlen(suffix)
	&&  !strcmp(name+strlen(name)-strlen(suffix), suffix))
		return (FALSE);
	(void)strcat(name, suffix);
	return (TRUE);
}
