/*
 * Title:	rcsedit.c (edit RCS file)
 * Author:	T.E.Dickey
 * Created:	26 May 1988
 * Modified:
 *		26 Dec 2014, coverity warnings
 *		07 Mar 2004, remove K&R support, indent'd.
 *		30 May 1998, compile with g++
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		17 Nov 1992, modified _FNX macro
 *		26 Oct 1992, RCS version 5 uses multiline format.
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
#define	OPN_PTYPES
#define	STR_PTYPES
#include	"ptypes.h"
#include	"rcsdefs.h"
#include	<ctype.h>

MODULE_ID("$Id: rcsedit.c,v 12.16 2014/12/26 13:02:08 tom Exp $")

/* local definitions */
#define	VERBOSE	if (verbose) PRINTF
#define	SEMICOLON	';'
#define	AT_SYMBOL	'@'

#define	DEBUG(s)	if(RCS_DEBUG > 1) PRINTF s;

static FILE *fpS, *fpT;
static char fname[MAXPATHLEN];	/* patch: should be dynamic */
static char buffer[BUFSIZ];	/* patch: should be dynamic */
static char tmp_name[MAXPATHLEN];	/* patch: should be dynamic */
static char *edit_at;
static int changes;		/* set if caller changed file */
static int verbose;		/* set if we show informational messages */
static int in_string;		/* true only while parsing string */

/************************************************************************
 *	local procedures						*
 ************************************************************************/

static void
Show(const char *tag, const char *text)
{
    if ((RCS_DEBUG > 1) && text != 0 && verbose) {
	FFLUSH(stdout);
	FFLUSH(stderr);
	PRINTF("%s %d%s%s",
	       tag,
	       changes,
	       in_string ? "\"\t" : "\t",
	       text);
    }
}

static int
dir_access(void)
{
    char *s;
    char temp[MAXPATHLEN];
    int uid = (int) geteuid();
    int gid = (int) getegid();
    Stat_t sb;
    int rc = FALSE;

    if (strlen(fname) < sizeof(temp)) {
	if ((s = fleaf_delim(strcpy(temp, fname))) != NULL) {
	    *s = EOS;
	} else {
	    (void) strcpy(temp, ".");
	}
	if (stat_dir(temp, &sb) >= 0) {
	    if (!uid) {			/* root can do anything */
		uid = (int) sb.st_uid;
		gid = (int) sb.st_gid;
	    }
	    if (uid == (int) sb.st_uid)
		rc = (int) (sb.st_mode & S_IWUSR);
	    else if (gid == (int) sb.st_gid)
		rc = (int) (sb.st_mode & (S_IWUSR >> 3));
	    else
		rc = (int) (sb.st_mode & (S_IWUSR >> 6));
	}
    }
    return rc;
}

static int
delim(int c)
{
    if (isspace(c))
	return (TRUE);
    return (strchr(";:,@", c) != 0);
}

static char *
readit(void)
{
    char *p = fgets(buffer, (int) sizeof(buffer), fpS);
    Show("<", p);
    edit_at = 0;
    return p;
}

static void
writeit(void)
{
    if (*buffer != EOS && fpT != 0) {
	Show(">", buffer);
	(void) fputs(buffer, fpT);
    }
    *buffer = EOS;
    edit_at = 0;
}

static void
closeit(void)
{
    if (fpT != 0) {
	FCLOSE(fpT);
	fpT = 0;
    }
}

/*
 * Flush the current buffer and read a new one
 */
static char *
ReadNewBuffer(void)
{
    writeit();
    return readit();
}

/*
 * Skips whitespace to the beginning of a token
 */
static char *
SkipBlanks(char *s)
{
    int c;
    while (s != 0) {
	while ((c = *s) != EOS) {
	    if (!isspace(c))
		return (edit_at = s);
	    s++;
	}
	s = ReadNewBuffer();
    }
    return 0;
}

/*
 * Skip past the semicolon which ends RCS header-records
 */
static char *
SkipPastSemicolon(char *s)
{
    while (s != 0) {
	while ((s != 0) && (*s != EOS)) {
	    if (*s == AT_SYMBOL)
		s = rcsparse_str(s, (RcsparseStr) 0);
	    else if (*s++ == SEMICOLON)
		return s;
	}
	s = ReadNewBuffer();
    }
    return 0;			/* got an end-of-file */
}

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/

/*
 * Open the RCS file corresponding to 'name'.
 */
int
rcsopen(const char *name, int show, int readonly)
{
    Stat_t sb;
    int fd;
    int rc = FALSE;

    if (strlen(name) < sizeof(fname)) {
	(void) strcpy(fname, name);
	fpT = 0;
	changes = 0;
	verbose = show;
	edit_at = 0;
	VERBOSE("++ rcs-%s(%s)\n", readonly ? "scan" : "edit", fname);
	if ((stat_file(fname, &sb) >= 0)
	    && (fpS = fopen(fname, "r")) != 0) {
	    int fmode;

	    if (readonly) {
		rc = TRUE;
	    } else {
		if (dir_access())
		    strcpy(tmp_name, fname)[strlen(fname) - 1] = 'V';
		else
		    FORMAT(tmp_name, "%s/rcsedit%d", P_tmpdir, (int) getpid());

		fmode = (int) (sb.st_mode & 0555);
		if ((fd = open(tmp_name, O_CREAT | O_EXCL | O_WRONLY, fmode)) < 0
		    || !(fpT = fdopen(fd, "w"))) {
		    perror(tmp_name);
		} else {
		    VERBOSE(".. opened \"%s\", copy \"%s\"\n", fname, tmp_name);
		    rc = TRUE;
		}
	    }
	} else {
	    VERBOSE("?? Cannot open \"%s\"\n", fname);
	}
    }
    return rc;
}

/*
 * rcsread(@)
 *	Given the last parse-position, and the code for the first token in an
 *	RCS record.
 *
 *	Read the next RCS item from the RCS file.  Most items are separated by
 *	semicolons.  If the caller did not finish processing the last one,
 *	skip past that semicolon.  This simplifies parsing (by not requiring
 *	that the caller parse all items).
 *
 *	Returns a pointer into our buffer (which may be edited by the caller
 *	before calling this procedure again).
 */
char *
rcsread(char *s, int code)
{
    if (s == 0)
	s = ReadNewBuffer();
    if ((s = SkipBlanks(s)) != NULL) {
	switch (code) {
	case S_FAIL:		/* initial value before first call */
	case S_VERS:		/* has no semicolon! */
	    break;
	case S_DESC:
	case S_LOG:
	case S_TEXT:
	    if (*s == AT_SYMBOL)
		s = rcsparse_str(s, (RcsparseStr) 0);
	    break;
	default:
	    s = SkipPastSemicolon(s);
	    break;
	}
    }
    return s;
}

/*
 * rcsedit(@)
 *	Alter a field in the input buffer.  This applies only to the most
 *	recent call on 'rcsparse_id()' or 'rcsparse_num()'.
 */
void
rcsedit(char *oldname, char *newname)
{
    size_t len = strlen(oldname);
    char tmp[BUFSIZ];

    if ((edit_at < buffer)
	|| (edit_at > buffer + strlen(buffer))
	|| strncmp(oldname, edit_at, len)) {
	failed("rcsedit");
    } else {
	(void) strcpy(tmp, edit_at + len);
	(void) strcat(strcpy(edit_at, newname), tmp);
	changes++;
    }
}

/*
 * rcsclose(@)
 *	Close the RCS-file which was opened, optionally copying back from the
 *	temporary file.
 */
void
rcsclose(void)
{
    if (fpT != 0) {
	if (changes) {
	    do
		writeit();
	    while (readit());
	    closeit();
	    if (rename(tmp_name, fname) < 0) {
		perror("rename");
		(void) unlink(tmp_name);
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
rcsparse_num(char *d, char *s)
{
    if ((s = SkipBlanks(s)) != NULL) {
	while (*s && (isdigit(UCH(*s)) || (*s == '.')))
	    *d++ = *s++;
    }
    *d = EOS;
    return s;
}

/*
 * rcsparse_id(@)
 *	Parse an id/keyword.  We should check to ensure that it begins with
 *	a letter, but don't, since we allow a <num> to be processed too (see
 *	'rcskeys.c').
 */
char *
rcsparse_id(char *d, char *s)
{
    if ((s = SkipBlanks(s)) != NULL) {
	while (*s && !delim(*s))
	    *d++ = *s++;
    }
    *d = EOS;
    return s;
}

/*
 * rcsparse_str(@)
 *	Skip over a string, which may cover more than one record.  The string
 *	is delimited by '@' marks, which are doubled to include them.
 */
#define	STR_FUNC(c)	if (str_func != 0)	str_func(c)

char *
rcsparse_str(char *s,
	     RcsparseStr str_func)
{
    int c;

    s = SkipBlanks(s);
    if (*s == AT_SYMBOL) {
	in_string = TRUE;
	s++;			/* skip past leading quote */
	do {
	    while ((s != 0) && (*s != EOS)) {
		if ((c = *s++) == AT_SYMBOL) {
		    if (*s != AT_SYMBOL) {
			STR_FUNC(EOS);
			in_string = FALSE;
			return s;
		    } else	/* skip escaped quote */
			s++;
		}
		STR_FUNC(c);
	    }
	} while ((s = ReadNewBuffer()) != NULL);

	STR_FUNC(EOS);		/* got an end-of-file */
	DEBUG(("EOF encountered in rcsparse_str\n"))
	    in_string = FALSE;
    }
    return s;
}
