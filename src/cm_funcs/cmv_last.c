/*
 * Title:	cmv_last.c (scan for last CmVision date)
 * Author:	T.E.Dickey
 * Created:	02 Aug 1994, from 'sccslast.c'
 * Modified:
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
#include	<cmv_defs.h>

MODULE_ID("$Id: cmv_last.c,v 12.7 1995/02/18 00:04:00 tom Exp $")

/*
 * Set the release.version and date values iff we find a legal sccs-file at
 * 'path[]'.
 * this is copied from 'sccslast.c', but adapted for CmVision's special
 * change-comment:
 *	\{number}\{comment}\^AO{uid}:G{gid}:P{protection}:M{modified}
 * (except for the last line, each also ends with a ':').
 */
static
void	ScanSCCS (
	_ARX(char *,	path)
	_ARX(char **,	vers_)
	_AR1(time_t *,	date_)
		)
	_DCL(char *,	path)
	_DCL(char **,	vers_)
	_DCL(time_t *,	date_)
{
	auto	FILE	*fp = fopen(path, "r");
	auto	int	gotten = 0;
	auto	int	match  = FALSE;
	auto	char	bfr[BUFSIZ];
	auto	char	*s;
	auto	int	yy, mm, dd, hr, mn, sc;
	auto	char	ver[80];

	if (fp) {
		newzone(5,0,FALSE);	/* interpret in EST5EDT */
		while (fgets(bfr, sizeof(bfr), fp)) {
			if (*bfr != '\001')
				break;
			if (!gotten) {
				if (bfr[1] != 'h')
					break;
				gotten++;
			}
			if ((strncmp(bfr+1, "d D ", 3) == 0)
			 && (sscanf(bfr+4, "%s %d/%d/%d %d:%d:%d ",
				ver,
				&yy, &mm, &dd,
				&hr, &mn, &sc) == 7)
			 && (strcmp(*vers_, ver) == 0)) {
				match = TRUE;
				*date_ = packdate (1900+yy, mm, dd, hr, mn, sc);
			}
			if (match
			 && !strncmp(bfr+1, "c ", 2)) {
				time_t	when;
				if ((s = strstr(bfr+1, "\\\001O")) != 0) {
					while (strncmp(s, ":M", 2) && *s)
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

void	cmv_last (
	_ARX(char *,	working)	/* working directory (absolute) */
	_ARX(char *,	path)		/* pathname to check (may be relative) */
	_ARX(char **,	vers_)
	_ARX(time_t *,	date_)
	_AR1(char **,	lock_)
		)
	_DCL(char *,	working)
	_DCL(char *,	path)
	_DCL(char **,	vers_)
	_DCL(time_t *,	date_)
	_DCL(char **,	lock_)
{
	auto	 char	*archive = cmv_file(working, path);

	*lock_ =
	*vers_ = "?";
	*date_ = 0;

	if (archive != 0) {
		get_cmv_lock(working, path, lock_, vers_);
		ScanSCCS(archive, vers_, date_);
	}
}

#ifdef	TEST
_MAIN
{
	char	current[MAXPATHLEN];
	int	n;

	(void)getwd(current);

	for (n = 1; n < argc; n++) {
		char	*vers;
		char	*lock;
		time_t	date;
		cmv_last(current, argv[n], &vers, &date, &lock);
		PRINTF("%s vers %s, lock %s %s",
			argv[n],
			vers ? vers : "<null>",
			lock ? lock : "<null>",
			ctime(&date));
	}
	exit(EXIT_SUCCESS);
	/*NOTREACHED*/
}
#endif
