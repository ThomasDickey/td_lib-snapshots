#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/cm_funcs/RCS/rcsedit.c,v 5.0 1989/07/25 09:19:55 ste_cm Rel $";
#endif	lint

/*
 * Title:	rcsedit.c (edit RCS file)
 * Author:	T.E.Dickey
 * Created:	26 May 1988
 * $Log: rcsedit.c,v $
 * Revision 5.0  1989/07/25 09:19:55  ste_cm
 * BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *
 *		Revision 4.0  89/07/25  09:19:55  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.1  89/07/25  09:19:55  dickey
 *		recompiled with apollo SR10 -- mods for function prototypes
 *		
 *		Revision 3.0  89/03/15  13:00:22  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  89/03/15  13:00:22  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.9  89/03/15  13:00:22  dickey
 *		sccs2rcs keywords
 *		
 *		08 Mar 1988, use temp-file for copying RCS archive back (safer)
 *		19 Aug 1988, modified 'rcsopen()' to properly check that we
 *			     are opening a file.  Corrected 'rcsparse_str()',
 *			     which must find delimiter before skipping it!
 *			     Added 'str_func' argument to 'rcs_parse_str()'.
 *		12 Aug 1988, corrected call on 'copyback()'; wasn't saving the
 *			     file's original protection.
 *		05 Aug 1988, removed 'rcsname()' call (done in callers).
 *			     changed interpretation of 'show' arg of 'rcsopen()'
 *
 * Function:	Open an RCS file, parse it, optionally modifying fields.
 */

#include	"ptypes.h"
#include	"rcsdefs.h"

#include	<string.h>
#include	<ctype.h>
extern	FILE	*tmpfile();
extern	char	*name2rcs();
extern	char	*strchr();

/* local definitions */
#define	VERBOSE	if (verbose) PRINTF

static	FILE	*fpS, *fpT;
static	char	fname[BUFSIZ];
static	char	buffer[BUFSIZ];
static	int	fmode;		/* original protection of file */
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
int	show;
{
	struct	stat	sb;

	(void)strcpy(fname, name);
	changed	= FALSE;
	lines	= 0;
	verbose	= show;
	VERBOSE("++ rcs-%s(%s)\n", (show > 0) ? "edit" : "scan", fname);
	if (	(stat(fname, &sb) >= 0)
	&&	((sb.st_mode & S_IFMT) == S_IFREG)
	&&	(fpS = fopen(fname, "r")) ) {
		fmode	= sb.st_mode & 0555;
			/* retain protection for copyback */
		if (!fpT)
			fpT = tmpfile();
		rewind(fpT);
		return (TRUE);
	}
	VERBOSE("?? Cannot open \"%s\"\n", fname);
	return (FALSE);
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
		int	fd;
		char	tname[BUFSIZ];

		while (readit())
			writeit();
		(void)fclose(fpS);

		VERBOSE("++ rcs-copyback %d lines\n", lines);
		(strcpy(tname, fname))[strlen(fname)-1] = ',';
		/* give 'copyback' something to work with... */
		if ((fd = creat(tname, 0644)) < 0) {
			perror(tname);
			return;
		}
		(void)close(fd);

		if (!copyback(fpT, tname, fmode, lines))
			perror(fname);
		else if (rename(tname, fname) < 0)
			perror("rename");
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
#define	STR_FUNC(c)	if (str_func != 0)	str_func(c)

char *
rcsparse_str(s, str_func)
register char	*s;
int	(*str_func)();		/* copies string as we read it */
{
	register int c;

	if (s != 0) {
		s = skips(s);
		while (*s != '@')
			if ((s = rcsread(s)) == 0)		goto done;
		s++;		/* skip past opening '@' */
		do {
			while (*s) {
				if ((c = *s++) == '@') {
					if (*s != '@')		goto done;
					s++;
					STR_FUNC('@');
				} else {
					STR_FUNC(c);
				}
			}
		} while (s = rcsread(s));
	}
done:	STR_FUNC(EOS);
	return (s);
}
