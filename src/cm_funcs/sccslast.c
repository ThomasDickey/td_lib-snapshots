/*
 * Title:	sccslast.c (scan for last sccs date)
 * Author:	T.E.Dickey
 * Created:	20 Oct 1986
 * Modified:
 *		30 Apr 2002, check for SunOS 4.x's pre-Y2K years (a colon).
 *		21 Jul 2000, mods to support $SCCS_VAULT
 *		09 Jan 2000, implement 2037 window for Y2K years.
 *		04 Jan 2000, decode Solaris' 2-digit pre-Y2K years.
 *		31 Dec 1999, move 1900's to packdate().
 *		03 Sep 1996, ignore removed-revisions
 *		14 Oct 1995, mods for 14-character names
 *		11 Aug 1994, decode CMVision date in comment.
 *		15 Jul 1994, mods for SCCS_VAULT
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

#define	CHR_PTYPES
#define	STR_PTYPES
#include	<ptypes.h>
#include	<sccsdefs.h>
#include	<ctype.h>

MODULE_ID("$Id: sccslast.c,v 12.16 2002/04/30 11:07:07 tom Exp $")

/*
 * Post-Y2K years require special decoding
 */
int	sccsyear (
	_AR1(char *,	src))
	_DCL(char *,	src)
{
	char *	p = 0;
	long	value;

	if (*src == ':') {
		*src = '0';
	}
	if ((value = strtol(src, &p, 10)) <= 0
	 || p == 0
	 || *p == EOS) {
		value = 0;
		if (strlen(src) == 2) {
			int tens = (*src++) - '0';
			if (tens >= 0 && isdigit((int)*src)) {
				value = 10 * tens + (*src - '0');
			}
		}
	}
	if (value < 38)		/* 2037 is last 32-bit date */
		value += 100;
	return value;
}

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
	auto	FILE	*fp;
	auto	int	gotten = 0;
	auto	char	bfr[BUFSIZ];
	auto	char	bfr2[BUFSIZ];
	auto	char	*s;
	auto	int	yy, mm, dd, hr, mn, sc;
	auto	char	ver[80];
	auto	int	have_rev = FALSE;

#if S_FILES_14
	auto	char	temp[MAXPATHLEN];
	if (filesize(path) < 0
	 && fleaf14(strcpy(temp, path))
	 && filesize(temp) > 0) {
		path = temp;
	}
#endif

	if ((fp = fopen(path, "r")) != 0) {
		newzone(5,0,FALSE);	/* interpret in EST5EDT */
		while (fgets(bfr, sizeof(bfr), fp)) {
			if (*bfr != '\001')
				break;
			if (!gotten) {
				if (strncmp(bfr, "\001h", 2))	break;
				gotten++;
			}
			if (!strncmp(bfr, "\001d D ", 4)) {
				if (sscanf(bfr+4, "%s %[^/]/%d/%d %d:%d:%d ",
					ver,
					bfr2, &mm, &dd,
					&hr, &mn, &sc) != 7
				 || ((yy = sccsyear(bfr2)) <= 0))
					break;
				if (strchr(ver, '.') != strrchr(ver, '.'))
					continue;
				*vers_ = txtalloc(ver);
				*date_ = packdate (yy, mm, dd, hr, mn, sc);
				have_rev = TRUE;
			}
#ifdef CMV_PATH		/* for CmVision */
			if (have_rev && !strncmp(bfr, "\001c ", 3)) {
				time_t	when;
				if ((s = strstr(bfr, "\\\001O")) != 0) {
					while (strncmp(s, ":M", 2) && *s)
						s++;
					if (sscanf(s, ":M%ld:", &when))
						*date_ = when;
				}
				break;
			}
#endif /* CMV_PATH */
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

static	char	the_prefix[] = SCCS_PREFIX;
#define	LEN_PREFIX	sizeof(the_prefix)-1

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
	auto	 char	fname[MAXPATHLEN],
			local[MAXPATHLEN],
			*dname = sccs_dir(working, path);
	auto	 int	is_sccs;
	register char	*s, *the_leaf;

	path   = strcpy(local, path);
	*lock_ =
	*vers_ = "?";
	*date_ = 0;

	/*
	 * If the file resides in an sccs-directory and its name begins with
	 * an appropriate prefix (lowercase letter followed by '.' and then
	 * more characters) assume it is an sccs file.
	 */
	if ((s = fleaf_delim(the_leaf = path)) != NULL) { /* determine directory from path */
		*(the_leaf = s) = EOS;
		if ((s = fleaf(path)) == NULL)
			s = path;
		is_sccs = sameleaf(s,dname) || sameleaf(dname, s);
		*the_leaf++ = PATH_SLASH;
	} else if ((s = fleaf(working)) != NULL) {
		is_sccs = sameleaf(s,dname);
	} else {
		return;			/* illegal input: give up */
	}

	/*
	 * If the file is an sccs-file, we wish to show the file-modification
	 * date in the Z-field since this will highlight better files which are
	 * not checked-in, or which are not checked out (possibly obsolete).
	 */
	if (is_sccs			/* the_leaf => filename */
	&&  (strlen(the_leaf) > LEN_PREFIX)
	&&  (isalpha((int)*the_leaf) && islower((int)*the_leaf))
	&&  (the_leaf[1] == '.')) {
		char	xx = *the_leaf;

		*the_leaf = 's';
		(void)strcpy(fname, path);
		*the_leaf = xx;
		trysccs(fname, vers_, date_, lock_);
		if (*date_) {		/* it was an ok sccs file */
			/* look for checked-out file */

			if (the_leaf != path) {
				pathcat(fname, working, the_leaf + LEN_PREFIX);
			} else {
				pathcat(fname, "../", the_leaf + LEN_PREFIX);
			}
			*date_ = 0;	/* use actual modification-date! */
			if (stat(fname, &sbfr) >= 0) {
				*date_ = sbfr.st_mtime;
			}
			return;
		}
	}

	/*
	 * The file was not itself an sccs-file.  Construct the name of an
	 * sccs-file assuming the standard naming convention, and try again.
	 *
	 * If the value 'dname[]' from 'sccs_dir()' is not a relative path,
	 * assume that SCCS_VAULT was set.
	 */
	if (the_leaf != path) {
		the_leaf[-1] = EOS;
		(void)strcpy(fname, path);
	} else {
		fname[0] = EOS;
	}

	if (isSlash(*dname) || *dname == '~') {
		(void)strcpy(fname, dname);
	} else {
		(void)pathcat(fname, fname, dname);
	}
	(void)strcat(pathcat(fname, fname, SCCS_PREFIX), the_leaf);

	trysccs(fname, vers_, date_, lock_);
}
