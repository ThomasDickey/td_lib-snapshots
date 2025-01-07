/*
 * Title:	rcssymbols.c (RCS symbols-parsing)
 * Author:	T.E.Dickey
 * Created:	07 Feb 1992
 * Modified:
 *		24 May 2010, fix clang --analyze warnings.
 *		07 Mar 2004, remove K&R support, indent'd.
 *		30 May 1998, compile with g++
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *
 * Function:	Within the 'rcsedit' state S_SYMBOLS, parse for information to
 *		complete our knowledge of the revision of an RCS file.  This is
 *		filled in according to which arguments are non-null when this
 *		is invoked:
 *
 *		who	=> obtain revision which is locked
 *		rev	=> obtain user who has locked it
 *		(both)	=> obtain any lock; override if we find one for the
 *			   current user.
 *
 * Returns:	the scan position past the last symbol.
 */

#define	STR_PTYPES
#include "ptypes.h"
#include "rcsdefs.h"
#include <ctype.h>

MODULE_ID("$Id: rcssymbs.c,v 12.11 2025/01/06 23:57:00 tom Exp $")

#define	isname(c)	(isalnum(UCH(c)) || (c == '_'))

static void
compress(char *in_out)
{
    char *s, *d;

    for (s = in_out; (*s == '0') && isname(s[1]); s++) ;
    if (s != in_out)
	for (d = in_out; (*d++ = *s++) != EOS;) ;

    for (s = in_out; (*s != EOS); s++) {
	if (*s == '.') {
	    compress(s + 1);
	    break;
	}
    }
}

static void
substitute(char *in_out,
	   char *name,
	   char *value)
{
    if (in_out != NULL) {
	if (!strcmp(in_out, name))
	    (void) strcpy(in_out, value);
	compress(in_out);
    }
}

static void
expand(char *in_out,
       char *name,
       char *value)
{
    char buffer[BUFSIZ];
    char *base = buffer;
    int first = TRUE;

    char *d = buffer, *s = in_out;
    int item;

    while (*s == '.')
	s++;
    *d = EOS;

    while (*s) {
	if ((item = *s++) == '.') {
	    substitute(base, name, value);
	    d = base + strlen(base);
	    first = TRUE;
	} else if (first) {
	    first = FALSE;
	    base = d;
	}
	*d++ = (char) item;
	*d = EOS;
    }
    substitute(base, name, value);
    (void) strcpy(in_out, buffer);
}

char *
rcssymbols(char *s,		/* current scan position */
	   char *dst,
	   const char *src)
{
    char identifier[BUFSIZ];
    char revision[BUFSIZ];
    char temp[BUFSIZ];

    if (src == NULL)
	src = "";
    (void) strcpy(temp, src);

    do {
	s = rcsparse_id(identifier, s);
	if (*s == ':')
	    s++;
	s = rcsparse_num(revision, s);
	if (*identifier && *revision && *src)
	    expand(temp, identifier, revision);
    } while (*identifier);

    if (strcmp(src, temp)) {
	if (RCS_DEBUG)
	    PRINTF("++ expand %s => %s\n", src, temp);
	(void) strcpy(dst, temp);
    }
    return (s);
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
