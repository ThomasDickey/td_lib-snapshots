/*
 * Title:	rcspermit.c
 * Author:	T.E.Dickey
 * Created:	08 Mar 1989
 * Modified:
 *		11 Oct 2022, gcc warnings
 *		27 Dec 2014, coverity warnings.
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		12 Nov 1992, added 'accflag' argument.
 *		04 Oct 1991, conversion to ANSI
 *		06 Sep 1991, modified interface to 'rcsopen()'
 *		05 Jul 1989, if the access-list is empty, we must own the file
 *			     to operate upon it.
 *		04 Apr 1989, if access list of permit-file is empty, assume
 *			     that we are using it solely for the baseline-
 *			     version.  In this case, return the baseline
 *			     version just as we would for permission checking.
 *
 *		10 Mar 1989, rewrote using 'rcsedit' module rather than special-
 *			     purpose file.
 *
 * Function:	Verifies that the 'permit' file exists within the specified
 *		RCS directory, and that the user specified is the one running
 *		the checkin/checkout program.
 *
 *		The permit file is an RCS file which records only the baseline
 *		information.
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	"rcsdefs.h"
#include	"dyn_str.h"
#include	<ctype.h>

MODULE_ID("$Id: rcsperm.c,v 12.13 2025/01/06 23:57:00 tom Exp $")

int
rcspermit(const char *path,
	  char *base,
	  const char **accflag)
{
    static DYN *access_list;
    Stat_t sb;
    int header = TRUE;
    char *s = NULL;
    char tip[80];
    char user[BUFSIZ];
    char key[BUFSIZ];
    char tmp[BUFSIZ];
    int empty = TRUE;		/* assume access-list empty */
    int my_file;
    int code = S_FAIL;
    int ok = FALSE;		/* assume no permission */
    char *t;

    path = vcs_file(path, tmp, FALSE);
    dyn_init(&access_list, (size_t) BUFSIZ);

    /*
     * Reset caller's copy of $RCS_BASE in case we are processing more than
     * one RCS directory.
     */
    if (base != NULL) {
	if ((t = getenv("RCS_BASE")) != NULL
	    && strlen(t) < (MAXPATHLEN - 1)) {
	    (void) strcpy(base, t);
	} else {
	    *base = EOS;
	}
    }

    /*
     * If we find the caller's uid/gid combination in the permission file,
     * (or if the access list is blank)
     * set the corresponding copy of $RCS_BASE and return true.
     */
    t = uid2s(getuid());
    (void) strcpy(user, (strlen(t) < sizeof(user) ? t : "?"));

    if (!rcsopen(path, RCS_DEBUG, TRUE))
	return (FALSE);		/* could not open file anyway */

    /*
     * If access-list is empty, we want to know who owns the file:
     */
    my_file = ((stat(path, &sb) >= 0)	/* ok always! */
	       &&(sb.st_uid == getuid()));

    while (header && (s = rcsread(s, code)) != NULL) {
	s = rcsparse_id(key, s);

	switch (code = rcskeys(key)) {
	case S_HEAD:
	    s = rcsparse_num(tip, s);
	    break;
	case S_ACCESS:
	    for (;;) {
		s = rcsparse_id(tmp, s);
		if (*tmp == EOS)
		    break;
		if (!empty)
		    APPEND(access_list, ",");
		APPEND(access_list, tmp);
		empty = FALSE;
		if (!ok)
		    ok = !strcmp(tmp, user);
	    }
	    if (empty) {
		ok = my_file;
	    }
	    if ((header = ok) != 0
		&& (base != NULL))
		(void) strcpy(base, tip);
	    break;
	case S_LOCKS:
	    s = rcslocks(s, strcpy(key, user), tmp);
	    break;
	case S_VERS:
	    header = FALSE;
	    break;
	}
    }
    rcsclose();
    if (accflag != NULL)
	*accflag = txtalloc(dyn_string(access_list));
    return (ok);
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
