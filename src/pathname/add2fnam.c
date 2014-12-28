/*
 * Title:	add2fname.c
 * Author:	T.E.Dickey
 * Created:	05 Jun 1989
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
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

MODULE_ID("$Id: add2fnam.c,v 12.7 2014/12/28 01:10:51 tom Exp $")

int
add2fname(char *name,
	  char *suffix)
{
    char *s;
#ifdef	vms
    if (s = fleaf(name))
	name = s;
    if (s = strrchr(name, ';')) {
	char version[256];
	(void) strcpy(version, s);
	*s = EOS;
	if (add2fname(name, suffix)) {
	    (void) strcat(name, version);
	    return (TRUE);
	} else {
	    (void) strcpy(name, version);
	    return (FALSE);
	}
    }
#else /* SYS_UNIX or MSDOS */
    if ((s = fleaf(name)) != 0)
	name = s;
#endif /* SYS_UNIX/unix */
    if (strlen(name) > strlen(suffix)
	&& !strcmp(name + strlen(name) - strlen(suffix), suffix))
	return (FALSE);
    (void) strcat(name, suffix);
    return (TRUE);
}

/******************************************************************************/
#ifdef	TEST
_MAIN
{
    (void) argc;
    (void) argv;
    exit(EXIT_FAILURE);
    /*NOTREACHED */
}
#endif /* TEST */
