#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: logch.c,v 12.2 1993/10/29 17:35:25 dickey Exp $";
#endif

/*
 * Title:	logch.c
 * Author:	T.E.Dickey
 * Created:	06 Aug 1992, from ded/dlog.c
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *
 * Function:	Decode/encode log-file characters (e.g., for 'ded').
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	"cmdch.h"
#include	<ctype.h>

#define	CONVERT(base,p,n)	n = (base * n) + (*p++ - '0')

int	decode_logch(
	_ARX(char **,	buffer)
	_AR1(int *,	count_)
		)
	_DCL(char **,	buffer)
	_DCL(int *,	count_)
{
	register char	*pointer = *buffer;
	register int	c, j;

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
		case 'U':	c = ARO_UP;		break;
		case 'D':	c = ARO_DOWN;		break;
		case 'L':	c = ARO_LEFT;		break;
		case 'R':	c = ARO_RIGHT;		break;
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
	c &= 0xff;

	if (count_)
		FORMAT(s + strlen(s), "%d", *count_);
	s += strlen(s);

	if (c == '\\')
		FORMAT(s, "\\\\");
	else if (isascii(c) && isgraph(c))
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
		case ARO_UP:	FORMAT(s, "\\U");	break;
		case ARO_DOWN:	FORMAT(s, "\\D");	break;
		case ARO_LEFT:	FORMAT(s, "\\L");	break;
		case ARO_RIGHT:	FORMAT(s, "\\R");	break;
		default:	FORMAT(s, "\\%03o", c);
	}
}
