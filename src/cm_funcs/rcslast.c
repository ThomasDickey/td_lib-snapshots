#ifndef	lint
static	char	sccs_id[] = "@(#)rcslast.c	1.3 88/06/01 10:01:43";
#endif	lint

/*
 * Title:	rcslast.c (scan for last RCS date)
 * Author:	T.E.Dickey
 * Created:	18 May 1988, from 'sccslast.c'
 * Modified:
 *		01 Jun 1988, added 'locks' decoding.  Recoded using 'rcskeys()'.
 *		23 May 1988, combined 'rel', 'ver' args.
 *
 * Function:	Lookup the last RCS-delta date, and its release.version number
 *		for directory-editor.  If a locked version is found, this is
 *		shown; otherwise the tip-version is shown.
 */

#include	<stdio.h>
#include	<ctype.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	"rcsdefs.h"

extern	char	*strcat(),
		*strcpy(),
		*strchr(),
		*strrchr();

extern	long	packdate();
extern	long	sccszone();
extern	char	*txtalloc();

#ifndef	TRUE
#define	TRUE	(1)
#define	FALSE	(0)
#endif	TRUE

#define	EOS	'\0'

#define	SKIP(s)	while (isspace(*s)) s++;
#define	COPY(name)\
		SKIP(s);\
		while (*s && !isspace(*s))		*name++ = *s++;\
		*name = EOS;

static	FILE	*rfp;
static	char	bfr[BUFSIZ];

/*
 * Read the current line and parse for keywords, which are in quasi-free format,
 * separated by semicolons.
 */
static
parse(key, arg)
char	*key, *arg;
{
register
char	*mark, *s;

	while (*bfr == EOS || *bfr == '\n')
		if (fgets(bfr, sizeof(bfr), rfp) == 0)
			return (FALSE);
	if (mark = strchr(s = bfr, ';')) {
		*mark++ = EOS;
		while (isspace(*mark))			mark++;
	} else
		mark = s + strlen(s);

	COPY(key);
	COPY(arg);
	for (s = bfr; *s++ = *mark++;);
	return (TRUE);
}

/*
 * Parse the lock-argument to obtain the locker's name and the locked version.
 * patch: this looks only for the first lock.
 */
static
parse2(arg, locker, version)
char	*arg, *locker, *version;
{
register char *s;

	if (s = strchr(arg, ':')) {
		*s++ = EOS;
		SKIP(s);
		COPY(version);
		s = arg;
		COPY(locker);
	}
}

/*
 * Set the release.version and date values iff we find a legal RCS-file at
 * 'path[]'.  We scan for the following:
 */
static
tryRCS (path, vers_, date_, lock_)
char	*path;
char	**vers_;
time_t	*date_;
char	**lock_;
{
int	yy, mm, dd, hr, mn, sc;
int	finish	= FALSE,
	skip	= FALSE;
char	key[BUFSIZ],
	arg[BUFSIZ],
	lstring[BUFSIZ],
	vstring[BUFSIZ];

	if (rfp = fopen(path, "r")) {
		*lstring =
		*vstring =
		*bfr = EOS;			/* initialize scanner */
		while (!finish && parse(key, arg)) {
			switch (rcskeys(key)) {
			case S_HEAD:
				if (rcskeys(strcpy(vstring, arg)) != S_VERS)
					finish = TRUE;
				break;
			case S_COMMENT:
				*bfr = EOS;	/* force new-scan */
				break;
			case S_LOCKS:
				parse2(arg, lstring, vstring);
				break;
			case S_VERS:
				skip = strcmp(key, vstring);
				break;
			case S_DATE:
				if (skip)	break;
				if (sscanf(arg, FMT_DATE,
					&yy, &mm, &dd, &hr, &mn, &sc)
					== 6) {
					*lock_ = txtalloc(lstring);
					*vers_ = txtalloc(vstring);
					*date_ = packdate (1900+yy, mm, dd, hr, mn, sc)
						- sccszone();
					finish = TRUE;
				}
				break;
			case S_DESC:
				finish = TRUE;
			}
		}
		(void) fclose(rfp);
	}
}

/************************************************************************
 *	main procedure							*
 ************************************************************************/

rcslast (working, path, vers_, date_, lock_)
char	*working;		/* working directory (absolute) */
char	*path;			/* pathname to check (may be relative) */
char	**vers_;
time_t	*date_;
char	**lock_;
{
char	name[BUFSIZ+1];
int	len_s	= strlen(RCS_SUFFIX),
	is_RCS,
	len;
register char *s, *t;
struct	stat	sbfr;

	*lock_ =
	*vers_ = "?";
	*date_ = 0;

	/*
	 * If the file resides in an RCS-directory and its name ends with
	 * an appropriate suffix (i.e., ",v") assume it is an RCS file.
	 */
	if (s = strrchr(t = path, '/')) { /* determine directory from path */
		*(t = s) = '\0';
		if (s = strrchr(path, '/'))
			s++;
		else
			s = path;
		is_RCS = !strcmp(s,RCS_DIR);
		*t++ = '/';
	} else if (s = strrchr(working, '/')) {
		is_RCS = !strcmp(++s,RCS_DIR);
	} else
		return;			/* illegal input: give up */

	/*
	 * If the file is an RCS-file, we wish to show the file-modification
	 * date in the Z-field since this will highlight better files which are
	 * not checked-in, or which are not checked out (possibly obsolete).
	 */
	if (is_RCS			/* t => filename */
	&&  ((len = strlen(t)) > len_s)
	&&  !strcmp(t + len - len_s, RCS_SUFFIX) ) {
		tryRCS(strcpy(name,path), vers_, date_, lock_);
		if (*date_) {		/* it was an ok RCS file */
			/* look for checked-out file */
			(void)strcat(strcpy(name+(t-path), "../"), t);
			name[(t-path)+len+3-len_s] = '\0'; /* trim suffix */
			*date_ = 0;	/* use actual modification-date! */
			if (stat(name, &sbfr) >= 0)
				*date_ = sbfr.st_mtime;
			return;
		}
	}

	/*
	 * The file was not itself an RCS-file.  Construct the name of an
	 * RCS-file assuming the standard naming convention, and try again.
	 */
	(void)strcpy(name,  s = path);
	(void)strcat(
		strcat(
			strcat(
				strcpy(&name[t-s],RCS_DIR),
				"/"),
			t),
		RCS_SUFFIX);
	tryRCS(name, vers_, date_, lock_);
}
