/*
 * Title:	logch.c
 * Author:	T.E.Dickey
 * Created:	06 Aug 1992, from ded/dlog.c
 * Modified:
 *		15 Feb 1998, added home/end/ppage/npage codes.
 *		16 Dec 1995, added mouse codes.
 *		05 Nov 1993, absorb "cmdch.h" into "td_curse.h"
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *
 * Function:	Decode/encode log-file characters (e.g., for 'ded').
 */

#define	CHR_PTYPES
#define	STR_PTYPES
#include	<ptypes.h>
#include	<td_curse.h>

MODULE_ID("$Id: logch.c,v 12.9 2000/12/24 22:32:32 tom Exp $")

#define	CONVERT(base,p,n)	n = (base * n) + (*p++ - '0')

#define L_PAREN '('
#define R_PAREN ')'

int	decode_logch(
	_ARX(char **,	buffer)
	_AR1(int *,	count_)
		)
	_DCL(char **,	buffer)
	_DCL(int *,	count_)
{
	register char	*pointer = *buffer;
	register int	c, j;
#ifndef NO_XTERM_MOUSE
	char	tmp1, tmp2;
#endif

	if (count_) {
		c = 0;
		while (isdigit(*pointer)) {
			CONVERT(10,pointer,c);
		}
		*count_ = c;
	}

	if (*pointer == '\\') {
		switch (*(++pointer)) {
		case '\\':	c = '\\';		break;
		case 'b':	c = '\b';		break;
		case 'f':	c = '\f';		break;
		case 'n':	c = '\n';		break;
		case 'r':	c = '\r';		break;
		case 't':	c = '\t';		break;
		case 'v':	c = '\v';		break;
		case 's':	c = ' ';		break;
		case 'E':	c = '\033';		break;
		case '?':	c = '\177';		break;
		case 'F':	c = CTL('F');		break;
		case 'B':	c = CTL('B');		break;
		case 'W':	c = CTL('W');		break;
		case 'U':	c = KEY_UP;		break;
		case 'D':	c = KEY_DOWN;		break;
		case 'L':	c = KEY_LEFT;		break;
		case 'R':	c = KEY_RIGHT;		break;
		case '^':	c = KEY_HOME;		break;
		case '$':	c = KEY_END;		break;
		case 'P':	c = KEY_PPAGE;		break;
		case 'N':	c = KEY_NPAGE;		break;
#ifndef NO_XTERM_MOUSE
		case 'M':
			c = KEY_MOUSE;
			if (sscanf(pointer+1, "%d%c%d,%d%c",
				&(xt_mouse.button),
				&tmp1,
				&(xt_mouse.row),
				&(xt_mouse.col),
				&tmp2) == 5
			 && (tmp1 == L_PAREN)
			 && (tmp2 == R_PAREN)) {
				xt_mouse.pressed  =
				xt_mouse.released = TRUE;
				pointer = strchr(pointer, R_PAREN);
			}
			break;
#endif
		default:
			if (isdigit(*pointer)) {
				c = 0;
				for (j = 0; j < 3; j++) {
					if (isdigit(*pointer))
						CONVERT(8,pointer,c);
					else
						break;	/* error? */
				}
				pointer--;	/* point to last digit */
			} else {
				c = *(--pointer);	/* recover */
			}
		}
		pointer++;	/* point past decoded char */
	} else
		c = (*pointer++);

	*buffer = pointer;
	return c;
}

void	encode_logch(
	_ARX(char *,	buffer)
	_ARX(int *,	count_)
	_AR1(int,	c)
		)
	_DCL(char *,	buffer)
	_DCL(int *,	count_)
	_DCL(int,	c)
{
	register char	*s = buffer;

	*s = EOS;

	if (count_)
		FORMAT(s + strlen(s), "%d", *count_);
	s += strlen(s);

	if (c == '\\')
		FORMAT(s, "\\\\");
	else if (c < 256 && isascii(c) && isgraph(c))
		FORMAT(s, "%c", c);
	else switch (c) {
		case '\b':	FORMAT(s, "\\b");	break;
		case '\f':	FORMAT(s, "\\f");	break;
		case '\n':	FORMAT(s, "\\n");	break;
		case '\r':	FORMAT(s, "\\r");	break;
		case '\t':	FORMAT(s, "\\t");	break;
		case '\v':	FORMAT(s, "\\v");	break;
		case ' ':	FORMAT(s, "\\s");	break;
		case '\033':	FORMAT(s, "\\E");	break;
		case '\177':	FORMAT(s, "\\?");	break;
		case CTL('F'):	FORMAT(s, "\\F");	break;
		case CTL('B'):	FORMAT(s, "\\B");	break;
		case CTL('W'):	FORMAT(s, "\\W");	break;
		case KEY_UP:	FORMAT(s, "\\U");	break;
		case KEY_DOWN:	FORMAT(s, "\\D");	break;
		case KEY_LEFT:	FORMAT(s, "\\L");	break;
		case KEY_RIGHT:	FORMAT(s, "\\R");	break;
		case KEY_HOME:	FORMAT(s, "\\^");	break;
		case KEY_END:	FORMAT(s, "\\$");	break;
		case KEY_PPAGE:	FORMAT(s, "\\P");	break;
		case KEY_NPAGE:	FORMAT(s, "\\N");	break;
		default:	FORMAT(s, "\\%03o", c);	break;
#ifndef NO_XTERM_MOUSE
		case KEY_MOUSE:
			FORMAT(s, "\\M%d(%d,%d)",
				xt_mouse.button,
				xt_mouse.row,
				xt_mouse.col);
			break;
#endif
	}
}
