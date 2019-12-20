/*
 * Title:	valid_ch.c (check validity of char for curses/shell)
 * Author:	T.E.Dickey
 * Created:	01 Dec 2019
 * Modified:
 *		19 Dec 2019, adapt Latin1 check to Solaris10
 */

#define STR_PTYPES
#include	"td_curse.h"
#include	<ctype.h>

#ifdef LOCALE
#include	<locale.h>
#ifdef HAVE_LANGINFO_CODESET
#include <langinfo.h>
#endif
#endif

MODULE_ID("$Id: valid_ch.c,v 12.2 2019/12/20 00:37:12 tom Exp $")

/* Latin-1 should be easy to identify, even if the codeset name is odd */
#define C1_CONTROLS	128
#define LATIN1_BASE	160
#define LATIN1_END	255

/* ...but Solaris10's Latin-1 omits most of the punctuation  */
#define Latin1Upper(c)	((c) >= 192 && (c) <= 222 && (c) != 215)
#define Latin1Lower(c)	((c) >= 223 && (c) <= 255 && (c) != 247)

typedef enum {
    cUNKNOWN = -1,
    cPOSIX = 0,
    cISO8859 = 1,
    cUTF8 = 2
} CHARSET;

static int charset = cUNKNOWN;

#ifdef LOCALE
#ifdef HAVE_LANGINFO_CODESET
static int
match_encoding(const char *pattern, const char *value)
{
    int result = 1;
    while (*pattern != EOS) {
	if (*pattern == '-') {
	    ++pattern;
	    while (*value == '-') {
		++value;
	    }
	} else if (*pattern != *value) {
	    result = 0;
	    break;
	} else if (*pattern != EOS) {
	    ++pattern;
	    ++value;
	}
    }
    return result;
}
#endif

static int
check_latin1(void)
{
    int result = 1;
    int ch;

    for (ch = C1_CONTROLS; ch <= LATIN1_END; ++ch) {
	if (iscntrl(ch)) {
	    if (ch >= LATIN1_BASE) {
		result = 0;
		break;
	    }
	} else if (isprint(ch)) {
	    if (ch < LATIN1_BASE) {
		result = 0;
		break;
	    } else if (Latin1Upper(ch) && !isupper(ch)) {
		result = 0;
		break;
	    } else if (Latin1Lower(ch) && !islower(ch)) {
		result = 0;
		break;
	    }
	} else if (Latin1Upper(ch) || Latin1Lower(ch)) {
	    result = 0;
	    break;
	}
    }
    return result;
}
#endif

static void
initialize(void)
{
    charset = cPOSIX;
#ifdef LOCALE
    setlocale(LC_ALL, "");
#ifdef HAVE_LANGINFO_CODESET
    {
	char *env;
	if ((env = nl_langinfo(CODESET)) != NULL) {
	    if (match_encoding("UTF-8", env))
		charset = cUTF8;
	    else if (match_encoding("xISO-8859-1", env))
		charset = cISO8859;
	    else if (check_latin1())
		charset = cISO8859;
	}
    }
#else /* !CODESET */
    if (check_latin1())
	charset = cISO8859;
#endif
#endif
}

#define initialize() if (charset < 0) (initialize)()

int
valid_curses_char(int ch)
{
    int result = FALSE;
    initialize();
    ch &= 0xff;
    if (isascii(ch)) {
	result = isprint(ch) && !iscntrl(ch);
    }
#ifdef HAVE_TYPE_CCHAR_T
    else if (charset == cUTF8) {
	result = TRUE;
    } else if (charset == cISO8859) {
	result = (ch >= LATIN1_BASE);
    }
#endif
    return result;
}

int
valid_shell_char(int ch)
{
    int result = FALSE;
    initialize();
    ch &= 0xff;
    if (isascii(ch)) {
	result = isprint(ch) && !iscntrl(ch);
    } else if (charset == cUTF8) {
	result = TRUE;
    } else if (charset == cISO8859) {
	result = (ch >= LATIN1_BASE);
    }
    return result;
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
