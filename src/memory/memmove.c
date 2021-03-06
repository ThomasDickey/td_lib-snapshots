/*
 * Title:	memmove.c
 * Author:	T.E.Dickey
 * Created:	17 Jul 1992, for 'field_of.c'
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		01 Jul 1994, split out from 'field_of.c'
 *
 * Function:	substitute for the commonly available function 'memmove'
 */
#define	STR_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: memmove.c,v 12.8 2014/12/28 01:10:47 tom Exp $")

#if	!defined(HAVE_MEMMOVE)
char *
memmove(char *s1, char *s2, size_t n)
{
    if (n != 0) {
	if ((s1 + n > s2) && (s2 + n > s1)) {
	    static char *buffer;
	    static unsigned length;
	    register size_t j;
	    if (length < n)
		buffer = doalloc(buffer, length = n);
	    for (j = 0; j < n; j++)
		buffer[j] = s2[j];
	    s2 = buffer;
	}
	while (n-- != 0)
	    s1[n] = s2[n];
    }
    return s1;
}
#else
void dummy_memmove(void);
void
dummy_memmove(void)
{
}
#endif /* HAVE_MEMMOVE */

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
