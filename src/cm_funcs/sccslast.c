#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: sccslast.c,v 12.4 1994/05/21 20:18:44 tom Exp $";
#endif

/*
 * Title:	sccslast.c (scan for last sccs date)
 * Author:	T.E.Dickey
 * Created:	20 Oct 1986
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		18 Oct 1991, look only for tip-version, not for "last" version
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		22 Jun 1990, if we have successfully opened/scanned "s." file,
 *			     try to do the same for the "p." file so that we can
 *			     show lock-owners.
 *		17 Aug 1989, rewrote the code which computes the path of the
 *			     working-file so that we don't use ".." unless
 *			     necessary, so this works better with symbolic
 *			     links.
 *		02 Sep 1988, use 'sccs_dir()'
 *		30 Jun 1988, use 'newzone()' instead of 'sccszone()'.
 *		01 Jun 1988, use SCCS_DIR environment variable.
 *		23 May 1988, combined rels/vers args.
 *		18 May 1988, moved 'sccszone' call here
 *		30 Sep 1987, if file is sccs-file, determine the data of the
 *			     corresponding checked-out file.
 *
 * Function:	Lookup the last sccs-delta date, and its release.version number
 *		for directory-editor.
 */

#define	STR_PTYPES
#include	<ptypes.h>
#include	<sccsdefs.h>
#include	<ctype.h>

/*
 * Set the release.version and date values iff we find a legal sccs-file at
 * 'path[]'.
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

void	sccslast (
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
	auto	 Stat_t	sbfr;
	auto	 char	name[BUFSIZ+1],
			*dname = sccs_dir();
	auto	 int	is_sccs;
	register char	*s, *t;

	*lock_ =
	*vers_ = "?";
	*date_ = 0;

	/*
	 * If the file resides in an sccs-directory and its name begins with
	 * an appropriate prefix (lowercase letter followed by '.' and then
	 * more characters) assume it is an sccs file.
	 */
	if ((s = fleaf_delim(t = path)) != NULL) { /* determine directory from path */
		*(t = s) = EOS;
		if ((s = fleaf(path)) == NULL)
			s = path;
		is_sccs = !strcmp(s,dname);
		*t++ = PATH_SLASH;
	} else if ((s = fleaf(working)) != NULL) {
		is_sccs = !strcmp(s,dname);
	} else
		return;			/* illegal input: give up */

	/*
	 * If the file is an sccs-file, we wish to show the file-modification
	 * date in the Z-field since this will highlight better files which are
	 * not checked-in, or which are not checked out (possibly obsolete).
	 */
	if (is_sccs			/* t => filename */
	&&  (strlen(t) > 2)
	&&  (isalpha(*t) && islower(*t))
	&&  (t[1] == '.')) {
	char	xx = *t;
		*t = 's';
		(void)strcpy(name, path);
		*t = xx;
		trysccs(name, vers_, date_, lock_);
		if (*date_) {		/* it was an ok sccs file */
			/* look for checked-out file */

			if (t != path) {
				name[t - path - 1] = EOS;
				if ((s = fleaf(name)) != NULL)
					*s = EOS;
				else
					name[0] = EOS;
				(void)strcat(name, t+2);
			} else
				(void)strcat(strcpy(name, "../"), t+2);
			*date_ = 0;	/* use actual modification-date! */
			if (stat(name, &sbfr) >= 0)
				*date_ = sbfr.st_mtime;
			return;
		}
	}

	/*
	 * The file was not itself an sccs-file.  Construct the name of an
	 * sccs-file assuming the standard naming convention, and try again.
	 */
	(void)strcpy(name,  s = path);
	(void)strcat(strcat(strcpy(&name[t-s], dname), "/s."), t);
	trysccs(name, vers_, date_, lock_);
}
