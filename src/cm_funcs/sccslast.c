#ifndef	lint
static	char	sccs_id[] = "@(#)sccslast.c	1.4 88/05/17 12:20:01";
#endif	lint

/*
 * Title:	sccslast.c (scan for last sccs date)
 * Author:	T.E.Dickey
 * Created:	20 Oct 1986
 * Modified:
 *		17 May 1988, lint.
 *		30 Sep 1987, if file is sccs-file, determine the data of the
 *			     corresponding checked-out file.
 *
 * Function:	Lookup the last sccs-delta date, and its release.version number
 *		for directory-editor.
 */

#include	<stdio.h>
#include	<ctype.h>
#include	<sys/types.h>
#include	<sys/stat.h>
extern	char	*strcat(),
		*strcpy(),
		*strrchr();

extern	long	packdate();

#define	MAXPATH	256

/*
 * Set the release.version and date values iff we find a legal sccs-file at
 * 'path[]'.
 */
static	trysccs (path, rels_, vers_, date_)
char	*path;
unsigned char *rels_, *vers_;
time_t	*date_;
{
FILE	*fp = fopen(path, "r");
int	gotten = 0;

	if (fp) {
	char	bfr[BUFSIZ];
		while (fgets(bfr, sizeof(bfr), fp)) {
			if (!gotten) {
				if (strncmp(bfr, "\001h", 2))	break;
				gotten++;
			}
			if (!strncmp(bfr, "\001d D ", 4)) {
			int	rel, ver, yy, mm, dd, hr, mn, sc;
				if (sscanf(bfr+4, "%d.%d %d/%d/%d %d:%d:%d ",
					&rel, &ver,
					&yy, &mm, &dd,
					&hr, &mn, &sc) != 8)	break;
				*rels_ = rel;
				*vers_ = ver;
				*date_ = packdate (1900+yy, mm, dd, hr, mn, sc);
				break;
			}
		}
		(void) fclose(fp);
	}
}

sccslast (working, path, rels_, vers_, date_)
char	*working;		/* working directory (absolute) */
char	*path;			/* pathname to check (may be relative) */
unsigned char *rels_, *vers_;
time_t	*date_;
{
char	name[MAXPATH+1];
int	is_sccs;
register char *s, *t;
struct	stat	sbfr;

	*rels_ = *vers_ = 0;
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
		*t++ = '/';
		is_sccs = !strncmp(s,"sccs",4);
	} else if (s = strrchr(working, '/')) {
		is_sccs = !strcmp(++s,"sccs");
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
		trysccs(name, rels_, vers_, date_);
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
	(void)strcat(strcpy(&name[t-s],"sccs/s."), t);
	trysccs(name, rels_, vers_, date_);
}
