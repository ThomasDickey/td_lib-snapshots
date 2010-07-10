/*
 * Title:	cmv_last.c (scan for last CmVision date)
 * Author:	T.E.Dickey
 * Created:	02 Aug 1994, from 'sccslast.c'
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		31 Dec 1999, move 1900's to packdate().
 *		21 Aug 1998, change cmv_lock to return binary file's mod-times.
 *		05 Jan 1995, CMVision stores all branches in the main trunk,
 *			     must use r-curr file to find the actual tip-version
 *		28 Sep 1994, CMVision encodes the file locks in the r-file.
 *		11 Aug 1994, CMVision encodes file modification time in the
 *			     change-comment.
 *
 * Function:	Lookup the last sccs-delta date, and its release.version number
 *		for directory-editor and similar utilities.
 *
 * FIXME:	This only knows how to scan SCCS files.  A CMVision archive may
 *		also be an RCS file.
 */

#define	CHR_PTYPES
#define	STR_PTYPES
#define	TIM_PTYPES
#include	<ptypes.h>
#include	<sccsdefs.h>
#include	<cmv_defs.h>

MODULE_ID("$Id: cmv_last.c,v 12.16 2010/07/10 00:08:21 tom Exp $")

/*
 * Set the release.version and date values iff we find a legal sccs-file at
 * 'path[]'.
 * this is copied from 'sccslast.c', but adapted for CmVision's special
 * change-comment:
 *	\{number}\{comment}\^AO{uid}:G{gid}:P{protection}:M{modified}
 * (except for the last line, each also ends with a ':').
 */
static void
ScanSCCS(const char *path,
	 const char **vers_,
	 time_t * date_)
{
    FILE *fp = fopen(path, "r");
    int gotten = 0;
    int match = FALSE;
    char bfr[BUFSIZ];
    char bfr2[BUFSIZ];
    char *s;
    int yy, mm, dd, hr, mn, sc;
    char ver[80];

    if (fp) {
	newzone(5, 0, FALSE);	/* interpret in EST5EDT */
	while (fgets(bfr, (int) sizeof(bfr), fp)) {
	    if (*bfr != '\001')
		break;
	    if (!gotten) {
		if (bfr[1] != 'h')
		    break;
		gotten++;
	    }
	    if (strncmp(bfr + 1, "d D ", (size_t) 3) == 0
		&& sscanf(bfr + 4, "%s %[^/]/%d/%d %d:%d:%d ",
			  ver,
			  bfr2, &mm, &dd,
			  &hr, &mn, &sc) == 7
		&& (yy = sccsyear(bfr2)) > 0
		&& (strcmp(*vers_, ver) == 0)) {
		match = TRUE;
		*date_ = packdate(yy, mm, dd, hr, mn, sc);
	    }
	    if (match
		&& !strncmp(bfr + 1, "c ", (size_t) 2)) {
		long when;
		if ((s = strstr(bfr + 1, "\\\001O")) != 0) {
		    while (strncmp(s, ":M", (size_t) 2) && *s)
			s++;
		    if (sscanf(s, ":M%ld", &when))
			*date_ = when;
		}
		break;
	    }
	}
	FCLOSE(fp);
	oldzone();		/* restore time-zone */
    }
}

/******************************************************************************/

void
cmv_last(const char *working,	/* working directory (absolute) */
	 const char *path,	/* pathname to check (may be relative) */
	 const char **vers_,
	 time_t * date_,
	 const char **lock_)
{
    char *archive = cmv_file(working, path);

    *lock_ =
	*vers_ = "?";
    *date_ = 0;

    if (archive != 0) {
	char *arcleaf = fleaf(archive);
	get_cmv_lock(working, path, lock_, vers_, date_);
	if (strncmp(arcleaf, "b-", (size_t) 2) || isdigit((int) (*vers_)[0]))
	    ScanSCCS(archive, vers_, date_);
    }
}

#ifdef	TEST
_MAIN
{
    char current[MAXPATHLEN];
    int n;

    if (getcwd(current, sizeof(current)) == 0)
	strcpy(current, ".");

    for (n = 1; n < argc; n++) {
	const char *vers;
	const char *lock;
	time_t date;
	cmv_last(current, argv[n], &vers, &date, &lock);
	PRINTF("%s vers %s, lock %s %s",
	       argv[n],
	       vers ? vers : "<null>",
	       lock ? lock : "<null>",
	       ctime(&date));
    }
    exit(EXIT_SUCCESS);
    /*NOTREACHED */
}
#endif
