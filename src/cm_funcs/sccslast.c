#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/cm_funcs/RCS/sccslast.c,v 3.0 1988/09/02 09:17:11 ste_cm Rel $";
#endif	lint

/*
 * Title:	sccslast.c (scan for last sccs date)
 * Author:	T.E.Dickey
 * Created:	20 Oct 1986
 * $Log: sccslast.c,v $
 * Revision 3.0  1988/09/02 09:17:11  ste_cm
 * BASELINE Mon Jun 19 13:27:01 EDT 1989
 *
 *		Revision 2.0  88/09/02  09:17:11  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.11  88/09/02  09:17:11  dickey
 *		sccs2rcs keywords
 *		
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

#include	"ptypes.h"
#include	<ctype.h>
extern	long	packdate();
extern	char	*sccs_dir(),
		*strcat(),
		*strcpy(),
		*strrchr(),
		*txtalloc();

/*
 * Set the release.version and date values iff we find a legal sccs-file at
 * 'path[]'.
 */
static	trysccs (path, vers_, date_, lock_)
char	*path;
char	**vers_;
time_t	*date_;
char	**lock_;
{
FILE	*fp = fopen(path, "r");
int	gotten = 0;

	if (fp) {
	char	bfr[BUFSIZ];
		newzone(5,0,FALSE);	/* interpret in EST5EDT */
		while (fgets(bfr, sizeof(bfr), fp)) {
			if (!gotten) {
				if (strncmp(bfr, "\001h", 2))	break;
				gotten++;
			}
			if (!strncmp(bfr, "\001d D ", 4)) {
			int	yy, mm, dd, hr, mn, sc;
			char	ver[80];
				if (sscanf(bfr+4, "%s %d/%d/%d %d:%d:%d ",
					ver,
					&yy, &mm, &dd,
					&hr, &mn, &sc) != 7)	break;
				*vers_ = txtalloc(ver);
				*date_ = packdate (1900+yy, mm, dd, hr, mn, sc);
				break;
			}
		}
		(void) fclose(fp);
		oldzone();		/* restore time-zone */
	}
}

sccslast (working, path, vers_, date_, lock_)
char	*working;		/* working directory (absolute) */
char	*path;			/* pathname to check (may be relative) */
char	**vers_;
time_t	*date_;
char	**lock_;
{
	auto	 char	name[BUFSIZ+1],
			*dname = sccs_dir();
	auto	 int	is_sccs;
	register char	*s, *t;
	struct	 stat	sbfr;

	*lock_ =
	*vers_ = "?";
	*date_ = 0;

	/*
	 * If the file resides in an sccs-directory and its name begins with
	 * an appropriate prefix (lowercase letter followed by '.' and then
	 * more characters) assume it is an sccs file.
	 */
	if (s = strrchr(t = path, '/')) { /* determine directory from path */
		*(t = s) = '\0';
		if (s = strrchr(path, '/'))
			s++;
		else
			s = path;
		is_sccs = !strcmp(s,dname);
		*t++ = '/';
	} else if (s = strrchr(working, '/')) {
		is_sccs = !strcmp(++s,dname);
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
			(void)strcat(strcpy(name+(t-path), "../"), t+2);
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
