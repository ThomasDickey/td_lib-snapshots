#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: rcssymbs.c,v 12.2 1993/10/29 17:35:24 dickey Exp $";
#endif

/*
 * Title:	rcssymbols.c (RCS symbols-parsing)
 * Author:	T.E.Dickey
 * Created:	07 Feb 1992
 * Modified:
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

#define	isname(c)	(isalnum(c) || (c == '_'))

static
void
compress(
_AR1(char *,	in_out))
_DCL(char *,	in_out)
{
	register char	*s, *d;

	for (s = in_out; (*s == '0') && isname(s[1]); s++)
		;
	if (s != in_out)
		for (d = in_out; (*d++ = *s++) != EOS; )
			;

	for (s = in_out; (*s != EOS);  s++) {
		if (*s == '.') {
			compress(s+1);
			break;
		}
	}
}

static
void
substitute(
_ARX(char *,	in_out)
_ARX(char *,	name)
_AR1(char *,	value)
	)
_DCL(char *,	in_out)
_DCL(char *,	name)
_DCL(char *,	value)
{
	if (in_out != 0) {
		if (!strcmp(in_out, name))
			(void) strcpy(in_out, value);
		compress(in_out);
	}
}

static
void
expand(
_ARX(char *,	in_out)
_ARX(char *,	name)
_AR1(char *,	value)
	)
_DCL(char *,	in_out)
_DCL(char *,	name)
_DCL(char *,	value)
{
	auto	char	buffer[BUFSIZ];
	auto	char	*base = 0;
	auto	int	first = TRUE;

	register char	*d = buffer,
			*s = in_out;
	register int	this;

	while (*s == '.')
		s++;
	*d = EOS;

	while (*s) {
		if ((this = *s++) == '.') {
			substitute(base, name, value);
			d     = base + strlen(base);
			first = TRUE;
		} else if (first) {
			first = FALSE;
			base  = d;
		}
		*d++ = this;
		*d = EOS;
	}
	substitute(base, name, value);
	(void)strcpy(in_out, buffer);
}

char *
rcssymbols(
_ARX(char *,	s)			/* current scan position */
_ARX(char *,	dst)
_AR1(char *,	src)
	)
_DCL(char *,	s)
_DCL(char *,	dst)
_DCL(char *,	src)
{
	char	identifier[BUFSIZ],
		revision[BUFSIZ],
		temp[BUFSIZ];

	if (src == 0)
		src = "";
	(void)strcpy(temp, src);

	do {
		s = rcsparse_id(identifier, s);
		if (*s == ':')	s++;
		s = rcsparse_num(revision, s);
		if (*identifier && *revision && *src)
			expand(temp, identifier, revision);
	} while (*identifier);

	if (strcmp(src, temp)) {
		if (RCS_DEBUG)
			PRINTF("++ expand %s => %s\n", src, temp);
		(void)strcpy(dst, temp);
	}
	return (s);
}
