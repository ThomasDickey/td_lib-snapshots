/*
 * Title:	valid_ch.c (check validity of char for curses/shell)
 * Author:	T.E.Dickey
 * Created:	01 Dec 2019
 * Modified:
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

MODULE_ID("$Id: valid_ch.c,v 12.1 2019/12/01 19:40:19 tom Exp $")

#define C1_CONTROLS	128
#define LATIN1_BASE	160
#define LATIN1_END	255

typedef enum {
    cUNKNOWN = -1,
    cPOSIX = 0,
    cISO8859 = 1,
    cUTF8 = 2
} CHARSET;

static int charset = cUNKNOWN;

static void
initialize(void)
{
#ifdef LOCALE
    setlocale(LC_ALL, "");
#ifdef HAVE_LANGINFO_CODESET
    {
	char *env;
	if ((env = nl_langinfo(CODESET)) != NULL) {
	    if (!strcmp(env, "UTF-8"))
		charset = cUTF8;
	    else if (!strncmp(env, "ISO-8859", 8))
		charset = cISO8859;
	    else
		charset = cPOSIX;
	}
    }
#else /* !CODESET */
    charset = cISO8859;
    {
	int ch;
	for (ch = C1_CONTROLS; ch <= LATIN1_END; ++ch) {
	    if (iscntrl(ch)) {
		if (ch >= LATIN1_BASE) {
		    charset = cPOSIX;
		    break;
		}
	    } else if (isprint(ch)) {
		if (ch < LATIN1_BASE) {
		    charset = cPOSIX;
		    break;
		}
	    } else {
		charset = cPOSIX;
		break;
	    }
	}
    }
#endif
#else
    charset = cPOSIX;
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
    } else
#ifdef HAVE_TYPE_CCHAR_T
    if (charset == cUTF8) {
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
