/*
 * Title:	rcslast.c (scan for last RCS date)
 * Author:	T.E.Dickey
 * Created:	18 May 1988, from 'sccslast.c'
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		11 Dec 2001, modify rcs_dir() interface to implement $RCS_VAULT
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		01 Feb 1993, last change did not properly parse user:id from
 *			     S_LOCKS case.
 *		29 Oct 1992, use 'rcsedit.c'
 *		07 Feb 1992, use 'rcs2time()'
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		26 Oct 1989, ensure that version, locker are set to "?" if not-
 *			     set.
 *		17 Aug 1989, rewrote the code which computes the path of the
 *			     working-file so that we don't use ".." unless
 *			     necessary, so this works better with symbolic
 *			     links.
 *		02 Sep 1988, use 'rcs_dir()'
 *		30 Jun 1988, use 'newzone()' rather than 'sccszone()'.
 *		01 Jun 1988, added 'locks' decoding.  Recoded using 'rcskeys()'.
 *		23 May 1988, combined 'rel', 'ver' args.
 *
 * Function:	Lookup the last RCS-delta date, and its release.version number
 *		for directory-editor.  If a locked version is found, this is
 *		shown; otherwise the tip-version is shown.
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	<ctype.h>
#include	<time.h>
#include	"rcsdefs.h"

MODULE_ID("$Id: rcslast.c,v 12.9 2004/03/07 16:31:58 tom Exp $")

/*
 * Returns the modification date of the given file, or 0 if not found
 * FIXME: should split-out
 */
static time_t
filedate(char *path)
{
    Stat_t sb;
    if (stat(path, &sb) >= 0)
	return sb.st_mtime;
    return 0;
}

/*
 * Debugging-trace for dates
 */
static void
ShowDate(char *tag, time_t t)
{
    char *s = (t != 0) ? ctime(&t) : "<none>\n";
    PRINTF("++   %s: %s", tag, s);
}

/*
 * Set the release.version and date values iff we find a legal RCS-file at
 * 'path[]'.  We scan for the following:
 */
static void
tryRCS(char *path,
       char **vers_,
       time_t * date_,
       char **lock_)
{
    int finish = FALSE, skip = FALSE, code = S_FAIL;
    char *s = 0, user[BUFSIZ], key[BUFSIZ], arg[BUFSIZ], lstring[BUFSIZ],
    vstring[BUFSIZ];

    if (rcsopen(path, RCS_DEBUG, TRUE)) {

	(void) strcpy(user, uid2s((int) getuid()));
	(void) strcpy(lstring, strcpy(vstring, "?"));

	while (!finish && (s = rcsread(s, code)) != 0) {
	    s = rcsparse_id(key, s);

	    switch (code = rcskeys(key)) {
	    case S_HEAD:
		/*
		 * Get the tip-version in case no lock is found
		 */
		s = rcsparse_id(vstring, s);
		if (rcskeys(vstring) != S_VERS)
		    finish = TRUE;
		break;
	    case S_LOCKS:
		/*
		 * Look for any lock by current user, or any
		 * lock if the current user has none.
		 */
		while ((s != 0) && (*s != ';')) {
		    s = rcsparse_id(key, s);
		    if (*s != ':')
			break;
		    s = rcsparse_id(arg, s + 1);
		    if (!strcmp(key, user)
			|| strcmp(lstring, user)) {
			(void) strcpy(lstring, key);
			(void) strcpy(vstring, arg);
		    }
		}
		break;
	    case S_VERS:
		skip = strcmp(key, vstring);
		break;
	    case S_DATE:
		if (!skip) {
		    s = rcsparse_id(arg, s);
		    *date_ = rcs2time(arg);
		    *lock_ = txtalloc(lstring);
		    *vers_ = txtalloc(vstring);
		    finish = TRUE;
		}
		break;
	    case S_DESC:
	    case S_LOG:
	    case S_TEXT:
		finish = TRUE;
	    }
	}
	rcsclose();
	if (RCS_DEBUG) {
	    PRINTF("++ rcslast %s %s\n", *lock_, *vers_);
	    ShowDate("lockrev", *date_);
	    ShowDate("working", filedate(path));
	}
    }
}

/************************************************************************
 *	main procedure							*
 ************************************************************************/

void
rcslast(char *working,		/* working directory (absolute) */
	char *path,		/* pathname to check (may be relative) */
	char **vers_,
	time_t * date_,
	char **lock_)
{
    static char dotdot[] =
    {'.', '.', PATH_SLASH, EOS};

    static char slash[] =
    {PATH_SLASH, EOS};		/* "/"   */

    char name[BUFSIZ + 1];
    char *dname = rcs_dir(working, path);
    int len_s = strlen(RCS_SUFFIX), is_RCS, len;
    char *s, *t;

    *lock_ =
	*vers_ = "?";
    *date_ = 0;

    /*
     * If the file resides in an RCS-directory and its name ends with
     * an appropriate suffix (i.e., ",v") assume it is an RCS file.
     */
    if ((s = fleaf_delim(t = path)) != NULL) {	/* determine directory from path */
	*(t = s) = EOS;
	if ((s = fleaf(path)) == NULL)
	    s = path;
	is_RCS = !strcmp(s, dname);
	*t++ = PATH_SLASH;
    } else if ((s = fleaf(working)) != NULL) {
	is_RCS = !strcmp(s, dname);
    } else
	return;			/* illegal input: give up */

    /*
     * If the file is an RCS-file, we wish to show the file-modification
     * date in the Z-field since this will highlight better files which are
     * not checked-in, or which are not checked out (possibly obsolete).
     */
    if (is_RCS			/* t => filename */
	&& ((len = strlen(t)) > len_s)
	&& !strcmp(t + len - len_s, RCS_SUFFIX)) {
	tryRCS(strcpy(name, path), vers_, date_, lock_);
	if (*date_) {		/* it was an ok RCS file */
	    /* look for checked-out file */

	    if (t != path) {
		name[t - path - 1] = EOS;
		if ((s = fleaf(name)) == NULL)
		    s = name;
		name[0] = EOS;
		(void) strcat(name, t);
	    } else
		(void) strcat(strcpy(name, dotdot), t);
	    name[strlen(name) - len_s] = EOS;	/* trim suffix */

	    *date_ = filedate(name);
	    return;
	}
    }

    /*
     * The file was not itself an RCS-file.  Construct the name of an
     * RCS-file assuming the standard naming convention, and try again.
     */
    (void) strcpy(name, s = path);
    (void) strcat(
		     strcat(
			       strcat(
					 strcpy(&name[t - s], dname),
					 slash),
			       t),
		     RCS_SUFFIX);
    tryRCS(name, vers_, date_, lock_);
}
