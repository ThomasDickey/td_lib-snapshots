#ifndef	lint
static	char	Id[] = "$Id: add2fnam.c,v 9.0 1991/05/15 10:15:44 ste_cm Rel $";
#endif

/*
 * Title:	add2fname.c
 * Author:	T.E.Dickey
 * Created:	05 Jun 1989
 *
 * Function:	Add a suffix to a filename, if it is not already there.
 *		On VMS, we preserve the version-number, if it is given.
 *
 * Returns:	TRUE if the suffix is not present; in this case we append it
 *		to the filename-buffer.
 */

#define	STR_PTYPES
#include "ptypes.h"

add2fname(name, suffix)
char	*name, *suffix;
{
	register char	*s;
#ifdef	vms
	if (s = strrchr(name, ']'))	name = s;
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
#else	/* unix */
	if (s = strrchr(name, '/'))	name = s;
#endif	/* vms/unix */
	if (strlen(name) > strlen(suffix)
	&&  !strcmp(name+strlen(name)-strlen(suffix), suffix))
		return (FALSE);
	(void)strcat(name, suffix);
	return (TRUE);
}
