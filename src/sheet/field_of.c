/*
 * Title:	field_of.c
 * Author:	T.E.Dickey
 * Created:	03 Feb 1992
 * Modified:
 *		24 May 2010, fix clang --analyze warnings.
 *		07 Mar 2004, remove K&R support, indent'd.
 *		30 May 1998, compile with g++
 *		01 Jul 1994, use autoconf to control use of 'memmove()'
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		24 Jul 1992, use dynamic-strings.
 *		17 Jul 1992, port to Apollo SR10.2 (no 'memmove()')
 *		24 Jun 1992, port to SunOs (no 'memmove()')
 *
 * Function:	insert/extract field in comma-separated list a la spreadsheet
 *
 *		Assumes that any field can be quoted with either '"' or "'"
 *		characters; that quoted-quotes are doubled quote-marks.
 */

#define	STR_PTYPES
#include "td_sheet.h"
#include <ctype.h>

MODULE_ID("$Id: field_of.c,v 12.11 2025/01/07 00:14:07 tom Exp $")

static int opt_Blanks;

#define	COMMA		','
#define	QUOTE		'"'
#define	isquote(c)	((c) == QUOTE || (c) == '\'')

/*
 * Looks to see if we must quote a field.
 */
static int
must_quote(char *buffer)
{
    int c;
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
static char *
QuotedField(char *src)
{
    static DYN *tmp;
    int c;

    dyn_init(&tmp, (size_t) BUFSIZ);
    if (*src) {
	tmp = dyn_append_c(tmp, QUOTE);
	while ((c = *src++) != EOS) {
	    if (c == QUOTE)	/* quoted-quote */
		tmp = dyn_append_c(tmp, c);
	    tmp = dyn_append_c(tmp, c);
	}
	tmp = dyn_append_c(tmp, QUOTE);
    }
    return dyn_string(tmp);
}

/*
 * Skips past the current position if it has a comma
 */
static char *
skip_past_comma(char *src)
{
    if (*src == COMMA)
	src++;
    return src;
}

/*
 * Skips to the comma ending the current field.  Note that if the field is
 * empty, this may be the first character in the field.
 */
static char *
skip_to_comma(char *src)
{
    int quote = EOS;
    int c;

    while ((c = *src) != EOS) {
	src++;
	if (quote) {		/* looking for end-of-quote? */
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
static int
skip_to_field(char *list,
	      int N,
	      char **first,
	      char **last)
{
    if ((*first = list) != NULL) {
	*last = skip_to_comma(*first);
	while (N > 0) {
	    if (**last != EOS) {
		N--;
		*first = skip_past_comma(*last);
		*last = skip_to_comma(*first);
	    } else {
		*first = *last;
		break;
	    }
	}
    } else {
	*first = NULL;
	*last = txtalloc("");
	N = -1;
    }
    return N;
}

/*
 * Strips quotes and extraneous whitespace from a field
 */
static char *
UnquotedField(DYN ** dst, char *src)
{
    char *next = skip_to_comma(src);
    int quote = EOS;
    int first = TRUE;
    int last = -1;
    int c;

    dyn_init(dst, (size_t) BUFSIZ);
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
	    } else		/* point to last nonblank */
		last = (int) dyn_length(*dst);
	}

	*dst = dyn_append_c(*dst, c);
	first = FALSE;
    }

    if (last >= 0) {
	dyn_string(*dst)[last + 1] = EOS;
	(*dst)->cur_length = (size_t) last;	/* patch */
    }

    return dyn_string(*dst);
}

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/

/*
 * Set/clear a flag which we can use to control whether embedded blanks in a
 * field must be quoted.
 */
void
field_uses_quotes(int flag)
{
    opt_Blanks = !flag;
}

/*
 * Returns the N'th comma-separated field, with leading/trailing blanks
 * removed.
 */
char *
get_field_of(char *list,
	     int N,
	     char *dftval,
	     DYN ** result)
{
    char *item, *next;

    if ((skip_to_field(list, N, &item, &next) == 0)
	&& (*item != EOS))
	return UnquotedField(result, item);

    if (dftval != NULL) {
	*result = dyn_copy(*result, dftval);
	return dyn_string(*result);	/* did not find field */
    }

    return NULL;
}

/*
 * Sets the N'th field in the comma-separated list.  This assumes that 'list'
 * is an allocated variable, or null.
 */
char *
set_field_of(char *list,
	     int N,
	     char *buffer)
{
    char *item, *next, *last = list;
    size_t need;

    /* find the point at which we replace the field */
    if ((N = skip_to_field(list, N, &item, &next)) < 0)
	N = 0;			/* list was null */

    /* ensure that the argument is nonnull */
    if (buffer == NULL)
	buffer = txtalloc("");

    /* check to see if we must quote the string */
    if (must_quote(buffer))
	buffer = QuotedField(buffer);

    /* allocate sufficient space for the new data */
    need = strlen(buffer) + strlen(next) + (size_t) ((item - list) + N);
    list = doalloc(list, need + 1);

    /*
     * If we had prior contents, must reformat
     */
    need = strlen(buffer);
    if (last != NULL) {
	item = (item - last) + list;
	next = (next - last) + list;
	(void) memmove(item + N + need, next, strlen(next) + 1);
    } else {
	item = list;
	need++;			/* for trailing null */
    }

    /*
     * Insert the new buffer, and fill in missing commas, if any
     */
    (void) memmove(item + N, buffer, need);
    while (N-- > 0)
	item[N] = ',';

    return list;
}

/************************************************************************
 *	test-driver							*
 ************************************************************************/
#ifdef	TEST
_MAIN
{
    /*NOT IMPLEMENTED */
}
#endif
