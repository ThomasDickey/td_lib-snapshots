#ifndef	lint
static	char	*Id = "$Id: field_of.c,v 10.7 1992/06/24 07:54:16 dickey Exp $";
#endif

/*
 * Title:	field_of.c
 * Author:	T.E.Dickey
 * Created:	03 Feb 1992
 * Modified:
 *		24 Jun 1992, port to SunOs (no 'memmove()')
 *
 * Function:	insert/extract field in comma-separated list a la spreadsheet
 *
 *		Assumes that any field can be quoted with either '"' or "'"
 *		characters; that quoted-quotes are doubled quote-marks.
 *
 * Limitations:	the returned value is limited to BUFSIZ to simplify the
 *		parameter-passing a little.
 */

#define	STR_PTYPES
#include "spreadsheet.h"

static	int	opt_Blanks;

#define	COMMA		','
#define	isquote(c)	(c == '"' || c == '\'')

#ifdef	sun
static
char *	memmove(
	_ARX(char *,	s1)
	_ARX(char *,	s2)
	_AR1(size_t,	n)
		)
	_DCL(char *,	s1)
	_DCL(char *,	s2)
	_DCL(size_t,	n)
{
	if (n != 0) {
		if ((s1+n > s2) && (s2+n > s1)) {
			static	char	*buffer;
			static	unsigned length;
			register int	j;
			if (length < n)
				buffer = doalloc(buffer, length = n);
			for (j = 0; j < n; j++)
				buffer[j] = s2[j];
			s2 = buffer;
		}
		while (n-- != 0)
			s1[n] = s2[n];
	}
	return s1;
}
#endif

/*
 * Looks to see if we must quote a field.
 */
static
int
must_quote(
_AR1(char *,	buffer))
_DCL(char *,	buffer)
{
	register int	c;
	while ((c = *buffer++) != EOS) {
		if (c == COMMA || isquote(c))
			return TRUE;
		else if (!opt_Blanks && isspace(c))
			return TRUE;
	}
	return FALSE;
}

/*
 * Applies quotes to a field.
 */
static
char *
quoted_field(
_ARX(char *,	dst)
_AR1(char *,	src)
	)
_DCL(char *,	dst)
_DCL(char *,	src)
{
	auto	char	*base = dst;
	register int	c;

	if (*src) {
		*dst++ = '"';			/* quote the whole field */
		while (c = (*dst++ = *src++)) {
			if (c == *base)		/* quoted-quote */
				*dst++ = c;
		}
		dst[-1] = *base;
	}
	dst[0]  = EOS;
	return base;
}

/*
 * Skips past the current position if it has a comma
 */
static
char *
skip_past_comma(
_AR1(char *,	src))
_DCL(char *,	src)
{
	if (*src == COMMA)
		src++;
	return src;
}

/*
 * Skips to the comma ending the current field.  Note that if the field is
 * empty, this may be the first character in the field.
 */
static
char *
skip_to_comma(
_AR1(char *,	src))
_DCL(char *,	src)
{
	auto	int	quote	= EOS;
	register int	c;

	while ((c = *src) != EOS) {
		src++;
		if (quote) {			/* looking for end-of-quote? */
			if (c == quote) {
				if (c == *src)	/* doubled-quote? */
					src++;
				else
					quote = EOS;
			}
		} else if (isquote(c)) {	/* beginning a quote? */
			quote = c;
		} else if (c == COMMA) {
			src--;
			break;
		}
	}
	return src;
}

/*
 * Computes first/last pointers to the N'th field in a comma-separated list
 */
static
int
skip_to_field(
_ARX(char *,	list)
_ARX(int,	N)
_ARX(char **,	first)
_AR1(char **,	last)
	)
_DCL(char *,	list)
_DCL(int,	N)
_DCL(char **,	first)
_DCL(char **,	last)
{
	if ((*first = list) != 0) {
		*last	= skip_to_comma(*first);
		while (N > 0) {
			if (**last != EOS) {
				N--;
				*first	= skip_past_comma(*last);
				*last	= skip_to_comma(*first);
			} else {
				*first	= *last;
				break;
			}
		}
		return N;
	}
	*first	= 0;
	*last	= "";
	return -1;
}

/*
 * Strips quotes and extraneous whitespace from a field
 */
static
char *
unquoted_field(
_ARX(char *,	dst)
_AR1(char *,	src)
	)
_DCL(char *,	dst)
_DCL(char *,	src)
{
	auto	char	*next	= skip_to_comma(src),
			*base	= dst,
			*last	= 0;
	auto	int	quote	= EOS,
			first	= TRUE;
	register int	c;

	while (src != next) {
		c = *src++;
		if (quote) {
			if (c == quote) {
				if (c == *src) {
					src++;
				} else {
					quote = EOS;
					continue;
				}
			}
		} else if (isquote(c)) {
			quote = c;
			continue;
		} else if (!opt_Blanks) {
			if (isspace(c)) {
				if (first)	/* ignore leading blank */
					continue;
			} else
				last = dst;	/* point to last nonblank */
		}

		if (dst - base >= BUFSIZ)
			break;
		*dst++ = c;
		first = FALSE;
	}

	if (last != 0)
		last[1] = EOS;
	else
		*dst = EOS;

	return base;
}

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/

/*
 * Set/clear a flag which we can use to control whether embedded blanks in a
 * field must be quoted.
 */
void
field_uses_quotes(
_AR1(int,	flag))
_DCL(int,	flag)
{
	opt_Blanks = !flag;
}

/*
 * Returns the N'th comma-separated field, with leading/trailing blanks
 * removed.
 */
char *
get_field_of(
_ARX(char *,	list)
_ARX(int,	N)
_ARX(char *,	buffer)
_AR1(char *,	dftval)
	)
_DCL(char *,	list)
_DCL(int,	N)
_DCL(char *,	buffer)
_DCL(char *,	dftval)
{
	auto	char	*this,
			*next;

	if (((N = skip_to_field(list, N, &this, &next)) == 0)
	 && (*this != EOS))
		return unquoted_field(buffer, this);

	return dftval;	/* did not find field */
}

/*
 * Sets the N'th field in the comma-separated list.  This assumes that 'list'
 * is an allocated variable, or null.
 */
char *
set_field_of(
_ARX(char *,	list)
_ARX(int,	N)
_AR1(char *,	buffer)
	)
_DCL(char *,	list)
_DCL(int,	N)
_DCL(char *,	buffer)
{
	auto	char	tmp_buffer[BUFSIZ];
	auto	char	*this,
			*next,
			*last	= list;
	auto	size_t	need;

	/* find the point at which we replace the field */
	if ((N = skip_to_field(list, N, &this, &next)) < 0)
		N = 0;		/* list was null */

	/* ensure that the argument is nonnull */
	if (buffer == 0)
		buffer = "";

	/* check to see if we must quote the string */
	if (must_quote(buffer))
		buffer = quoted_field(tmp_buffer, buffer);

	/* allocate sufficient space for the new data */
	need = strlen(buffer) + strlen(next) + (this - list) + N;
	list = doalloc(list, (unsigned)need + 1);

	/*
	 * If we had prior contents, must reformat
	 */
	need = strlen(buffer);
	if (last != 0) {
		this	= (this - last) + list;
		next	= (next - last) + list;
		(void)memmove(this + N + need, next, strlen(next)+1);
	} else {
		this	= list;
		need++;	/* for trailing null */
	}

	/*
	 * Insert the new buffer, and fill in missing commas, if any
	 */
	(void)memmove(this + N, buffer, need);
	while (N-- > 0)
		this[N] = ',';

	return list;
}

/************************************************************************
 *	test-driver							*
 ************************************************************************/
#ifdef	TEST
_MAIN
{
}
#endif
