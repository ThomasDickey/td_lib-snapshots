/*
 * Title:	sccslast.c (scan for last sccs date)
 * Author:	T.E.Dickey
 * Created:	20 Oct 1986
 * Modified:
 *		27 Dec 2014, coverity warnings.
 *		07 Mar 2004, remove K&R support, indent'd.
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

MODULE_ID("$Id: sccslast.c,v 12.28 2025/01/07 00:03:17 tom Exp $")

/*
 * Post-Y2K years require special decoding
 */
int
sccsyear(char *src)
{
    char *p = NULL;
    long value;

    if (*src == ':') {
	*src = '0';
    }
    if ((value = strtol(src, &p, 10)) <= 0
	|| p == NULL
	|| *p == EOS) {
	value = 0;
	if (strlen(src) == 2) {
	    int tens = (*src++) - '0';
	    if (tens >= 0 && isdigit((int) *src)) {
		value = 10 * tens + (*src - '0');
	    }
	}
    }
    if (value < 38)		/* 2037 is last 32-bit date */
	value += 100;
    return (int) value;
}

/*
 * Set the release.version and date values iff we find a legal sccs-file at
 * 'path[]'.
 */
static void
trySCCS(const char *path,
	const char **vers_,
	time_t *date_,
	const char **lock_)
{
    FILE *fp;
    int gotten = 0;
    char bfr[BUFSIZ];
    char bfr2[BUFSIZ];
    char tmp1[MAXPATHLEN];
    char *s;
    int yy, mm, dd, hr, mn, sc;
    char ver[80];
    int have_rev = FALSE;

#if defined(S_FILES_14)
    char temp[MAXPATHLEN];
    if (filesize(path) < 0
	&& fleaf14(strcpy(temp, path))
	&& filesize(temp) > 0) {
	path = temp;
    }
#endif

    if (strlen(path) >= MAXPATHLEN)
	return;

    if ((fp = fopen(path, "r")) != NULL) {
	newzone(5, 0, FALSE);	/* interpret in EST5EDT */
	while (fgets(bfr, (int) sizeof(bfr), fp)) {
	    if (*bfr != '\001')
		break;
	    if (!gotten) {
		if (strncmp(bfr, "\001h", (size_t) 2))
		    break;
		gotten++;
	    }
	    if (!strncmp(bfr, "\001d D ", (size_t) 4)) {
		if (sscanf(bfr + 4, "%s %[^/]/%d/%d %d:%d:%d ",
			   ver,
			   bfr2, &mm, &dd,
			   &hr, &mn, &sc) != 7
		    || ((yy = sccsyear(bfr2)) <= 0))
		    break;
		if (strchr(ver, '.') != strrchr(ver, '.'))
		    continue;
		*vers_ = txtalloc(ver);
		*date_ = packdate(yy, mm, dd, hr, mn, sc);
		have_rev = TRUE;
	    }
#ifdef CMV_PATH			/* for CmVision */
	    if (have_rev && !strncmp(bfr, "\001c ", (size_t) 3)) {
		long when;
		if ((s = strstr(bfr, "\\\001O")) != NULL) {
		    while (strncmp(s, ":M", (size_t) 2) && *s)
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
	if ((s = fleaf(strcpy(tmp1, path))) == NULL)
	    s = tmp1;
	*s = 'p';
	if ((fp = fopen(tmp1, "r")) != NULL) {
	    if (fgets(bfr, (int) sizeof(bfr), fp)) {
		if (sscanf(bfr, "%d.%d %d.%d %s",
			   &yy, &mm, &dd, &hr, ver) == 5)
		    *lock_ = txtalloc(ver);
	    }
	    FCLOSE(fp);
	}
    }
}

#define	LEN_PREFIX	sizeof(SCCS_PREFIX)-1

void
sccslast(const char *working,	/* working directory (absolute) */
	 const char *path,	/* pathname to check (may be relative) */
	 const char **vers_,
	 time_t *date_,
	 const char **lock_)
{
    Stat_t sbfr;
    char fname[MAXPATHLEN];
    char local[MAXPATHLEN];
    char *dname = sccs_dir(working, path);
    int is_sccs;
    char *s, *the_leaf;

    *lock_ = "?";
    *vers_ = "?";
    *date_ = 0;

    if (strlen(dname) >= sizeof(fname))
	return;
    if (strlen(path) >= sizeof(local))
	return;
    path = strcpy(local, path);

    /*
     * If the file resides in an sccs-directory and its name begins with
     * an appropriate prefix (lowercase letter followed by '.' and then
     * more characters) assume it is an sccs file.
     */
    if ((s = fleaf_delim(the_leaf = local)) != NULL) {	/* determine directory from path */
	*(the_leaf = s) = EOS;
	if ((s = fleaf(local)) == NULL)
	    s = local;
	is_sccs = sameleaf(s, dname) || sameleaf(dname, s);
	*the_leaf++ = PATH_SLASH;
    } else if ((s = fleaf(working)) != NULL) {
	is_sccs = sameleaf(s, dname);
    } else {
	return;			/* illegal input: give up */
    }

    /*
     * If the file is an sccs-file, we wish to show the file-modification
     * date in the Z-field since this will highlight better files which are
     * not checked-in, or which are not checked out (possibly obsolete).
     */
    if (is_sccs			/* the_leaf => filename */
	&& (strlen(the_leaf) > LEN_PREFIX)
	&& (isalpha((int) *the_leaf) && islower((int) *the_leaf))
	&& (the_leaf[1] == '.')) {
	char xx = *the_leaf;

	*the_leaf = 's';
	(void) strcpy(fname, path);
	*the_leaf = xx;
	trySCCS(fname, vers_, date_, lock_);
	if (*date_) {		/* it was an ok sccs file */
	    /* look for checked-out file */

	    if (the_leaf != path) {
		pathcat(fname, working, the_leaf + LEN_PREFIX);
	    } else {
		pathcat(fname, "../", the_leaf + LEN_PREFIX);
	    }
	    *date_ = 0;		/* use actual modification-date! */
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
	(void) strcpy(fname, path);
    } else {
	fname[0] = EOS;
    }

    if (isSlash(*dname) || *dname == '~') {
	(void) strcpy(fname, dname);
    } else if ((strlen(fname) + strlen(dname) + 3) < sizeof(fname)) {
	(void) pathcat(fname, fname, dname);
    } else {
	return;
    }

    if ((strlen(fname)
	 + strlen(SCCS_PREFIX)
	 + strlen(the_leaf) + 5) < sizeof(fname)) {
	(void) strcat(pathcat(fname, fname, SCCS_PREFIX), the_leaf);
	trySCCS(fname, vers_, date_, lock_);
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
