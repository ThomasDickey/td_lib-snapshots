#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: cmv_last.c,v 12.2 1994/08/15 23:39:38 tom Exp $";
#endif

/*
 * Title:	cmv_last.c (scan for last CmVision date)
 * Author:	T.E.Dickey
 * Created:	02 Aug 1994, from 'sccslast.c'
 * Modified:
 *		11 Aug 1994, CMVision encodes file modification time in the
 *			     change-comment.
 *
 * Function:	Lookup the last sccs-delta date, and its release.version number
 *		for directory-editor.
 */

#define	CHR_PTYPES
#define	STR_PTYPES
#define	TIM_PTYPES
#include	<ptypes.h>
#include	<cmv_defs.h>

/*
 * Set the release.version and date values iff we find a legal sccs-file at
 * 'path[]'.
 * this is copied from 'sccslast.c', but adapted for CmVision's special
 * change-comment:
 *	\{number}\{comment}\^AO{uid}:G{gid}:P{protection}:M{modified}:
 */
static
void	trysccs (
	_ARX(char *,	path)
	_ARX(char **,	vers_)
	_ARX(time_t *,	date_)
	_AR1(char **,	lock_)
		)
	_DCL(char *,	path)
	_DCL(char **,	vers_)
	_DCL(time_t *,	date_)
	_DCL(char **,	lock_)
{
	auto	FILE	*fp = fopen(path, "r");
	auto	int	gotten = 0;
	auto	char	bfr[BUFSIZ];
	auto	char	*s;
	auto	int	yy, mm, dd, hr, mn, sc;
	auto	char	ver[80];

	if (fp) {
		newzone(5,0,FALSE);	/* interpret in EST5EDT */
		while (fgets(bfr, sizeof(bfr), fp)) {
			if (!gotten) {
				if (strncmp(bfr, "\001h", 2))	break;
				gotten++;
			}
			if (!strncmp(bfr, "\001d D ", 4)) {
				if (sscanf(bfr+4, "%s %d/%d/%d %d:%d:%d ",
					ver,
					&yy, &mm, &dd,
					&hr, &mn, &sc) != 7)	break;
				if (strchr(ver, '.') != strrchr(ver, '.'))
					continue;
				*vers_ = txtalloc(ver);
				*date_ = packdate (1900+yy, mm, dd, hr, mn, sc);
			}
			if (!strncmp(bfr, "\001c ", 3)) {
				time_t	when;
				if ((s = strstr(bfr, "\\\001O")) != 0) {
					while (strncmp(s, ":M", 2) && *s)
						s++;
					if (sscanf(s, ":M%ld:", &when))
						*date_ = when;
				}
				break;
			}
		}
		FCLOSE(fp);
		oldzone();		/* restore time-zone */
	}

	if (gotten) {
		if ((s = fleaf(strcpy(bfr, path))) == NULL)
			s = bfr;
		*s = 'p';
		if ((fp = fopen(bfr, "r")) != 0) {
			if (fgets(bfr, sizeof(bfr), fp)) {
				if (sscanf(bfr, "%d.%d %d.%d %s",
					&yy, &mm, &dd, &hr, ver) == 5)
					*lock_ = txtalloc(ver);
			}
			FCLOSE(fp);
		}
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

	if (archive != 0)
		trysccs(archive, vers_, date_, lock_);
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
