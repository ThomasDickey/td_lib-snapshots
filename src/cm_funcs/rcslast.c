/*
 * Title:	rcslast.c (scan for last RCS date)
 * Author:	T.E.Dickey
 * Created:	18 May 1988, from 'sccslast.c'
 * Modified:
 *		07 Jan 2018, correct pointer arithmetic when ded is scanning
 *			     inside an RCS directory.
 *		27 Dec 2014, coverity warnings
 *		24 May 2010, fix clang --analyze warnings.
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
#include <ptypes.h>
#include <ctype.h>
#include <time.h>
#include <rcsdefs.h>
#include <dyn_str.h>

MODULE_ID("$Id: rcslast.c,v 12.17 2018/01/07 20:49:25 tom Exp $")

/*
 * Returns the modification date of the given file, or 0 if not found
 * FIXME: should split-out
 */
static time_t
filedate(const char *path)
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
ShowDate(const char *tag, time_t t)
{
    const char *s = (t != 0) ? ctime(&t) : "<none>\n";
    PRINTF("++   %s: %s", tag, s);
}

/*
 * Set the release.version and date values iff we find a legal RCS-file at
 * 'path[]'.  We scan for the following:
 */
static void
tryRCS(const char *path,
       const char **vers_,
       time_t * date_,
       const char **lock_)
{
    int finish = FALSE, skip = FALSE, code = S_FAIL;
    char *s = 0;
    char *p;
    char user[BUFSIZ];
    char key[BUFSIZ];
    char arg[BUFSIZ];
    char lstring[BUFSIZ];
    char vstring[BUFSIZ];

    if (rcsopen(path, RCS_DEBUG, TRUE)) {

	p = uid2s(getuid());
	(void) strcpy(user, ((strlen(p) < sizeof(user))
			     ? p
			     : "?"));
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
			(void) strcpy(lstring, ((strlen(key) < sizeof(lstring))
						? key
						: "?"));
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
rcslast(const char *working,	/* working directory (absolute) */
	const char *path,	/* pathname to check (may be relative) */
	const char **vers_,
	time_t * date_,
	const char **lock_)
{
    static char dotdot[] =
    {'.', '.', PATH_SLASH, EOS};

    static char slash[] =
    {PATH_SLASH, EOS};		/* "/"   */

    static DYN local;

    char name[MAXPATHLEN];
    char *dname = rcs_dir(working, path);
    int len_s = strlen(RCS_SUFFIX);
    int is_RCS;
    int len;
    char *s, *t;

    *lock_ = "?";
    *vers_ = "?";
    *date_ = 0;

    if (strlen(path) >= sizeof(name))
	return;

    dyn_copy(&local, path);

    /*
     * If the file resides in an RCS-directory and its name ends with
     * an appropriate suffix (i.e., ",v") assume it is an RCS file.
     */
    if ((s = fleaf_delim(t = dyn_string(&local))) != NULL) {
	/* determine directory from path */
	*(t = s) = EOS;
	if ((s = fleaf(path)) == NULL)
	    s = dyn_string(&local);
	is_RCS = !strcmp(s, dname);
	*t++ = PATH_SLASH;
    } else if ((s = fleaf(working)) != NULL) {
	is_RCS = !strcmp(s, dname);
    } else {
	return;			/* illegal input: give up */
    }

    /*
     * If the file is an RCS-file, we wish to show the file-modification
     * date in the Z-field since this will highlight better files which are
     * not checked-in, or which are not checked out (possibly obsolete).
     */
    if (is_RCS			/* t => filename */
	&& ((len = (int) strlen(t)) > len_s)
	&& !strcmp(t + len - len_s, RCS_SUFFIX)) {
	tryRCS(strcpy(name, path), vers_, date_, lock_);
	if (*date_) {		/* it was an ok RCS file */
	    /* look for checked-out file */

	    if (t != (s = dyn_string(&local))) {
		name[t - s - 1] = EOS;
		if ((s = fleaf(name)) == NULL)
		    s = name;
		s[0] = EOS;
		(void) strcat(name, t);
	    } else
		(void) strcat(strcpy(name, dotdot), t);
	    name[(int) strlen(name) - len_s] = EOS;	/* trim suffix */

	    *date_ = filedate(name);
	    return;
	}
    }

    /*
     * The file was not itself an RCS-file.  Construct the name of an
     * RCS-file assuming the standard naming convention, and try again.
     */
    s = dyn_string(&local);
    if ((strlen(s) + strlen(dname) + 10) < sizeof(name)) {
	(void) strcpy(name, s);
	(void) strcat(
			 strcat(
				   strcat(
					     strcpy(&name[t - s], dname),
					     slash),
				   t),
			 RCS_SUFFIX);
	tryRCS(name, vers_, date_, lock_);
    }
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
