#ifndef	lint
static	char	*Id = "$Id: rcsedit.c,v 11.1 1992/09/02 15:36:12 dickey Exp $";
#endif

/*
 * Title:	rcsedit.c (edit RCS file)
 * Author:	T.E.Dickey
 * Created:	26 May 1988
 * Modified:
 *		06 Feb 1992, use 'stat_file()'
 *		22 Oct 1991, broke logic of 'rcs_close()' on 6-sep (must always
 *			     cleanup after writing the tempfile mode) -- fixed.
 *		04 Oct 1991, conversion to ANSI
 *		13 Sep 1991, corrected rcsparse_str -- was not passing spaces
 *			     back to caller.
 *		06 Sep 1991, added 'readonly' arg; suppress tempfile-creation
 *			     if true.
 *		10 Jul 1991, write directly into lock-file to avoid redundant
 *			     copying, unless we have no permission in the RCS
 *			     directory.
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

#define	ACC_PTYPES
#define	STR_PTYPES
#include	"ptypes.h"
#include	"rcsdefs.h"
#include	<ctype.h>

/* local definitions */
#define	VERBOSE	if (verbose) PRINTF

static	FILE	*fpS, *fpT;
static	char	fname[MAXPATHLEN];
static	char	buffer[BUFSIZ];
static	char	tmp_name[MAXPATHLEN];
static	int	changes;	/* set if caller changed file */
static	int	verbose;	/* set if we show informational messages */

/************************************************************************
 *	local procedures						*
 ************************************************************************/

static
Show(
_ARX(char *,	tag)
_AR1(char *,	text)
	)
_DCL(char *,	tag)
_DCL(char *,	text)
{
	if ((RCS_DEBUG > 1) && text != 0 && verbose) {
		FFLUSH(stdout);
		FFLUSH(stderr);
		PRINTF("%s %d\t%s", tag, changes, text);
	}
}

static
dir_access(_AR0)
{
	register char	*s;
	char	temp[MAXPATHLEN];
	int	uid = geteuid();
	int	gid = getegid();
	struct	stat	sb;

	if (s = strrchr(strcpy(temp, fname), '/'))
		*s = EOS;
	else
		(void)strcpy(temp, ".");
	if (stat_dir(temp, &sb) < 0)
		return FALSE;

	if (!uid) {		/* root can do anything */
		uid = sb.st_uid;
		gid = sb.st_gid;
	}
	if (uid == sb.st_uid)
		return (sb.st_mode & S_IWRITE);
	else if (gid == sb.st_gid)
		return (sb.st_mode & (S_IWRITE >> 3));
	return (sb.st_mode & (S_IWRITE >> 6));
}

static
delim(
_AR1(int,	c))
_DCL(int,	c)
{
	if (isspace(c))	return (TRUE);
	return (strchr(";:,@", c) != 0);
}

static
char *
skips(
_AR1(char *,	s))
_DCL(char *,	s)
{
	while (isspace(*s))		s++;
	return (s);
}

static
char *
readit(_AR0)
{
	char	*p = fgets(buffer, sizeof(buffer), fpS);
	Show("<", p);
	return p;
}

static
writeit(_AR0)
{
	if (*buffer != EOS && fpT != 0) {
		Show(">", buffer);
		(void)fputs(buffer, fpT);
	}
	*buffer = EOS;
}

static
closeit(_AR0)
{
	if (fpT != 0) {
		FCLOSE(fpT);
		fpT = 0;
	}
}

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/

/*
 * Open the RCS file corresponding to 'name'.
 */
rcsopen(
_ARX(char *,	name)
_ARX(int,	show)
_AR1(int,	readonly)
	)
_DCL(char *,	name)
_DCL(int,	show)
_DCL(int,	readonly)
{
	struct	stat	sb;
	int	fd;

	(void)strcpy(fname, name);
	fpT     = 0;
	changes	= 0;
	verbose	= show;
	VERBOSE("++ rcs-%s(%s)\n", readonly ? "scan" : "edit", fname);
	if (	(stat_file(fname, &sb) >= 0)
	&&	(fpS = fopen(fname, "r")) ) {
		int	fmode;

		if (readonly)
			return (TRUE);
		else if (dir_access())
			strcpy(tmp_name, fname)[strlen(fname)-1] = 'V';
		else
			FORMAT(tmp_name, "%s/rcsedit%d", P_tmpdir, getpid());

		fmode	= sb.st_mode & 0555;
		if ((fd = open(tmp_name, O_CREAT|O_EXCL|O_WRONLY, fmode)) < 0
		 || !(fpT = fdopen(fd, "w"))) {
			perror(tmp_name);
			return(FALSE);
		}
		VERBOSE(".. opened \"%s\", copy \"%s\"\n", fname, tmp_name);
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
rcsread(
_AR1(char *,	s))
_DCL(char *,	s)
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
rcsedit (
_ARX(char *,	where)
_ARX(char *,	old)
_AR1(char *,	new)
	)
_DCL(char *,	where)
_DCL(char *,	old)
_DCL(char *,	new)
{
size_t	len = strlen(old);
char	tmp[BUFSIZ];

	if ((where < buffer)
	||  (where > buffer + strlen(buffer))
	||  strncmp(old, where, len))
		failed("rcsedit");

	(void)strcpy(tmp, where + len);
	(void)strcat(strcpy(where, new), tmp);
	changes++;
}

/*
 * rcsclose(@)
 *	Close the RCS-file which was opened, optionally copying back from the
 *	temporary file.
 */
rcsclose(_AR0)
{
	if (fpT != 0) {
		if (changes) {
			do writeit();
			while (readit());
			closeit();
			if (rename(tmp_name, fname) < 0) {
				perror("rename");
				(void)unlink(tmp_name);
				exit(FAIL);
			}
		} else
			closeit();
	} else if (changes && RCS_DEBUG)
		FPRINTF(stderr, "?? %d change(s) lost (readonly)\n", changes);

	FCLOSE(fpS);
	fpS = 0;
	*buffer = EOS;
}

/************************************************************************
 *	parsing procedures						*
 ************************************************************************/

/* {<digit>{.}} */
char *
rcsparse_num(
_ARX(char *,	d)
_AR1(char *,	s)
	)
_DCL(char *,	d)
_DCL(char *,	s)
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
rcsparse_id(
_ARX(char *,	d)
_AR1(char *,	s)
	)
_DCL(char *,	d)
_DCL(char *,	s)
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
rcsparse_str(
_ARX(register char *,	s)
_FN1(int,		str_func)	/* copies string as we read it */
	)
_DCL(register char *,	s)
_DCL(int,		(*str_func)())
{
	register int c;

	if (s != 0) {
		while (isspace(*s)) {
			STR_FUNC(*s);
			s++;
		}
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
		} while (s = readit());
	}
done:	STR_FUNC(EOS);
	return (s);
}
