#ifndef	lint
static	char	sccs_id[] = "@(#)rcslast.c	1.2 88/05/23 06:39:32";
#endif	lint

/*
 * Title:	rcslast.c (scan for last RCS date)
 * Author:	T.E.Dickey
 * Created:	18 May 1988, from 'sccslast.c'
 * Modified:
 *		23 May 1988, combined 'rel', 'ver' args.
 *
 * Function:	Lookup the last RCS-delta date, and its release.version number
 *		for directory-editor.
 */

#include	<stdio.h>
#include	<ctype.h>
#include	<sys/types.h>
#include	<sys/stat.h>
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

#define	SUFFIX	",v"
#define	LEN_S	2		/* strlen(SUFFIX) */

/*
 * Parse for a keyword, converting the buffer to hold the value of the keyword
 * if it is found.
 */
static
keyRCS(string, key)
char	*string, *key;
{
char	*s,
	first[BUFSIZ],
	second[BUFSIZ];

	if (s = strchr(string, ';')) {
		*s = '\0';
		if (sscanf(string, "%s %s;", first, second) == 2)
			if (!strcmp(first, key)) {
				(void)strcpy(string, second);
				return (TRUE);
			}
	}
	return (FALSE);
}

/*
 * Set the release.version and date values iff we find a legal RCS-file at
 * 'path[]'.  We scan for the following:
 */
#define	S_HEAD	0	/* head <version_string>;	*/
#define	S_HEAD2	1	/* <more header lines>		*/
#define	S_SKIP	2	/* <blank lines>		*/
#define	S_VERS	3	/* <version_string>		*/
#define	S_DATE	4	/* date <date>; <some text>	*/
#define	S_EXIT	5

static	tryRCS (path, vers_, date_)
char	*path;
char	**vers_;
time_t	*date_;
{
FILE	*fp = fopen(path, "r");
int	state = S_HEAD;
int	yy, mm, dd, hr, mn, sc;
char	vstring[BUFSIZ],
	bfr[BUFSIZ],
	*s;

	if (fp) {
		while ((state < S_EXIT) && fgets(bfr, sizeof(bfr), fp)) {
			for (s = bfr + strlen(bfr) - 1;
				(s >= bfr) && isspace(*s);
					*(s--) = '\0');
			switch (state) {
			case S_HEAD:
				if (keyRCS(bfr, "head")) {
					s = strcpy(vstring, bfr);
					while (*s) {
						if (!isdigit(*s) && *s != '.') {
							state = S_EXIT;
							break;
						}
						s++;
					}
					state++;
				} else
					state = S_EXIT;
				break;
			case S_HEAD2:
				if (!*bfr)	state++;
				break;
			case S_SKIP:
				if (*bfr)
					state++;
				else
					break;
			case S_VERS:
				if (!strcmp(bfr, vstring))
					state++;
				else
					state = S_EXIT;
				break;
			case S_DATE:
				state = S_EXIT;	/* has to be here! */
				if (!keyRCS(bfr, "date"))	break;
				if (sscanf(bfr, "%d.%d.%d.%d.%d.%d",
					&yy, &mm, &dd, &hr, &mn, &sc)
					== 6) {
					*vers_ = txtalloc(vstring);
					*date_ = packdate (1900+yy, mm, dd, hr, mn, sc)
						- sccszone();
				}
			}
		}
		(void) fclose(fp);
	}
}

/************************************************************************
 *	main procedure							*
 ************************************************************************/

rcslast (working, path, vers_, date_)
char	*working;		/* working directory (absolute) */
char	*path;			/* pathname to check (may be relative) */
char	**vers_;
time_t	*date_;
{
char	name[BUFSIZ+1];
int	is_RCS,
	len;
register char *s, *t;
struct	stat	sbfr;

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
		is_RCS = !strcmp(s,"RCS");
		*t++ = '/';
	} else if (s = strrchr(working, '/')) {
		is_RCS = !strcmp(++s,"RCS");
	} else
		return;			/* illegal input: give up */

	/*
	 * If the file is an RCS-file, we wish to show the file-modification
	 * date in the Z-field since this will highlight better files which are
	 * not checked-in, or which are not checked out (possibly obsolete).
	 */
	if (is_RCS			/* t => filename */
	&&  ((len = strlen(t)) > LEN_S)
	&&  !strcmp(t + len - LEN_S, SUFFIX) ) {
		tryRCS(strcpy(name,path), vers_, date_);
		if (*date_) {		/* it was an ok RCS file */
			/* look for checked-out file */
			(void)strcat(strcpy(name+(t-path), "../"), t);
			name[(t-path)+len+3-LEN_S] = '\0'; /* trim suffix */
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
	(void)strcat(strcat(strcpy(&name[t-s],"RCS/"), t), SUFFIX);
	tryRCS(name, vers_, date_);
}
