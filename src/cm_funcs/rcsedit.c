#ifndef	lint
static	char	sccs_id[] = "@(#)rcsedit.c	1.2 88/08/05 14:38:36";
#endif	lint

/*
 * Title:	rcsedit.c (edit RCS file)
 * Author:	T.E.Dickey
 * Created:	26 May 1988
 * Modified:
 *		05 Aug 1988, removed 'rcsname()' call (done in callers).
 *			     changed interpretation of 'show' arg of 'rcsopen()'
 *
 * Function:	Open an RCS file, parse it, optionally modifying fields.
 */

#include	"rcsdefs.h"

#include	<stdio.h>
#include	<ctype.h>
extern	FILE	*tmpfile();
extern	char	*name2rcs();
extern	char	*strcat();
extern	char	*strcpy();
extern	char	*strchr();

/* local definitions */
#define	EOS	'\0'
#define	TRUE	1
#define	FALSE	0

#define	PRINTF	(void) printf
#define	VERBOSE	if (verbose) PRINTF

static	FILE	*fpS, *fpT;
static	char	fname[BUFSIZ];
static	char	buffer[BUFSIZ];
static	int	changed;	/* set if caller changed file */
static	int	lines;		/* total # of records written to temp-file */
static	int	verbose;	/* set if we show informational messages */

/************************************************************************
 *	local procedures						*
 ************************************************************************/

static
delim(c)
{
	if (isspace(c))	return (TRUE);
	return (strchr(";:,@", c) != 0);
}

static
char *
skips(s)
char	*s;
{
	while (isspace(*s))		s++;
	return (s);
}

static
char *
readit()
{
	return (fgets(buffer, sizeof(buffer), fpS));
}

static
writeit()
{
	if (*buffer) {
		fputs(buffer, fpT);
		lines++;
	}
	*buffer = EOS;
}

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/

/*
 * Open the RCS file corresponding to 'name'.
 */
rcsopen(name, show)
char	*name;
{
	(void)strcpy(fname, name);
	changed	= FALSE;
	lines	= 0;
	verbose	= show;
	VERBOSE("++ rcs-%s(%s)\n", (show > 0) ? "edit" : "scan", fname);
	if (!(fpS = fopen(fname, "r"))) {
		PRINTF("?? Cannot open \"%s\"\n", fname);
		return (FALSE);
	}
	if (!fpT)
		fpT = tmpfile();
	rewind(fpT);
	return (TRUE);
}

/*
 * rcsread(@)
 *	Read the next RCS item from the RCS file.  Items are separated by
 *	semicolons.  If the caller did not finish processing the last one,
 *	assume that the semicolon lies in the same record.  This simplifies
 *	parsing (by not requiring that the caller parse all items).
 *
 *	Returns a pointer into our buffer (which may be edited by the caller
 *	before calling this procedure again).
 */
char *
rcsread(s)
char	*s;
{
	do {
		if (s) {
			while (*s && (*s != ';'))	s++;
			while (*s == ';')		s = skips(s+1);
			if (*s)
				return (s);
		}
		writeit();
		if (!(s = readit()))
			break;		/* end-of-file, or error */
		s = skips(s);
	} while (*s == EOS);
	return(s);
}

/*
 * rcsedit(@)
 *	Alter a field in the input buffer.
 */
rcsedit (where, old, new)
char	*where, *old, *new;
{
int	len = strlen(old);
char	tmp[BUFSIZ];

	if ((where < buffer)
	||  (where > buffer + strlen(buffer))
	||  strncmp(old, where, len))
		failed("rcsedit");

	(void)strcpy(tmp, where + len);
	(void)strcat(strcpy(where, new), tmp);
	changed = TRUE;
}

/*
 * rcsclose(@)
 *	Close the RCS-file which was opened, optionally copying back from the
 *	temporary file.
 */
rcsclose()
{
	writeit();
	if (changed) {
		while (readit())
			writeit();
		(void)fclose(fpS);
		VERBOSE("++ rcs-copyback %d lines\n", lines);
		if (!copyback(fpT, fname, 0444, lines))
			perror(fname);
	} else
		(void)fclose(fpS);
}

/************************************************************************
 *	parsing procedures						*
 ************************************************************************/

/* {<digit>{.}} */
char *
rcsparse_num(d, s)
char	*d, *s;
{
	while (*s && (isdigit(*s) || (*s == '.')))	*d++ = *s++;
	*d = EOS;
	return (skips(s));
}

/*
 * rcsparse_id(@)
 *	Parse an id/keyword.  We should check to ensure that it begins with
 *	a letter, but don't, since we allow a <num> to be processed too (see
 *	'rcskeys.c').
 */
char *
rcsparse_id(d, s)
char	*d, *s;
{
	s = skips(s);
	while (*s && !delim(*s))	*d++ = *s++;
	*d = EOS;
	return (skips(s));
}

/*
 * rcsparse_str(@)
 *	Skip over a string, which may cover more than one record.  The string
 *	is delimited by '@' marks, which are doubled to include them.
 */
char *
rcsparse_str(s)
char	*s;
{
	s = skips(s);
	if (*s == '@') {	/* don't try to process if not a string */
		s++;
		do {
			while (*s) {
				if (*s != '@')		s++;
				else if (s[1] == '@')	s += 2;
				else
					return (s+1);
			}
		} while (s = rcsread(s));
	}
	return (s);
}
