#ifndef	lint
static	char	sccs_id[] = "@(#)rawgets.c	1.2 87/11/24 09:40:35";
#endif	lint

/*
 * Author:	T.E.Dickey
 * Title:	rawgets.c (raw-mode 'gets()')
 * Created:	29 Sep 1987 (from 'fl.c')
 * Modified:
 *		24 Nov 1987, moved to my SPC library, use under 'curses'.
 *
 * Function:	Accept input from the screen, permitting a user to either
 *		alter an existing string, or to enter a new one.  This
 *		procedure is called in raw/noecho mode, and is used by 'fl'
 *		to enter new command text.
 *
 *		Only printing (including space) characters will be returned
 *		in 'bfr[]'.
 *
 *		On entry, the caller has printed the contents of 'bfr[]' to
 *		the screen, and the screen position is the end of the string.
 *
 *		On normal exit, the user provides a newline, which is echoed.
 *
 *		If a kill character is given, 'bfr[]' is nulled, but the
 *		cursor position is not altered.
 */
#include	<curses.h>
#include	<ctype.h>

static	int	xbase,	ybase,	/* base-position of 'bfr[]' */
		Imode;
static	char	*bbase;		/* 'bfr[]' copy */

/*
 * Position the cursor at the given index in the string.  This permits us to
 * ignore details such as wraparound, and whether the cursor is at the end of
 * string.
 */
static
char	*MoveTo(new)
char	*new;
{
register
int	y = ybase,
	x = xbase,
	z = new-bbase;
	while (z-- > 0) {
		if (++x > COLS) {
			x = 0;
			y++;
		}
	}
	move(y,x);
	refresh();
	return(new);
}

/*
 * Insert a character in the screen and into 'bfr[]' at the given position.
 */
static
insert(at,c)
char	*at;
{
int	y,x;
register int	d  = c;
register char	*s = at;

	addch(c);		/* cf: 'insch()' */
	getyx(stdscr,y,x);
	if (*at) {
		addstr(at);
		clrtoeol();
		move(y,x);
	}
	do {
		c = d;
		d = *s;
	} while (*s++ = c);
	refresh();
}

/*
 * Delete the character before the given position in 'bfr[]'.
 */
static
char *
delete(at)
char	*at;
{
	if (at > bbase) {
	int	y,x;
	register char	*s = --at;
		while (s[0] = s[1]) s++;
		at = MoveTo(s = at);
		getyx(stdscr,y,x);
		addstr(s);
		clrtoeol();
		move(y,x);
		refresh();
	} else beep();
	return(at);
}

/*
 * Toggle the insert/scroll mode, and show the state of this flag by
 * overwriting the ":" position of the prompt which is written before
 * calling this procedure.
 */
static
toggle()
{
int	y,x;
	Imode = !Imode;
	getyx(stdscr,y,x);
	move(ybase,xbase-2);
	addch(Imode ? ':' : '^');
	move(y,x);
	refresh();
}

/************************/
rawgets (bfr,size,newline)
register char	*bfr;
{
register char	*tag;
register c;
int	ec = erasechar(),
	kc = killchar();

	Imode = 1;
	bbase = bfr;
	getyx(stdscr,ybase,xbase);	/* get my initial position */
	xbase -= strlen(bfr);

	tag = bfr + strlen(bfr);	/* ...and end-of-string */

	for (;;) {
		c = getchar();
		if (c == '\t') {
			toggle();
			continue;
		}
		if (c == '\n' || c == '\r') {
			if (newline) addch('\n');
			break;
		}
		if (!Imode) {	/* process scroll-mode ops */
			if (isprint(c)) {
				toggle();
			} else if (c == '\b') {
				if (tag > bfr)	tag = MoveTo(tag-1);
				else		beep();
			} else if (c == '\f') {
				if (*tag)	tag = MoveTo(tag+1);
				else		beep();
			} else if (c == ec || c == kc) {
				tag = delete(tag);
			} else	beep();
		}
		if (Imode) {	/* process insert-mode ops */
			if (c == ec) {
				tag = delete(tag);
			} else if (c == kc) {
				*bfr = '\0';
				break;
			} else if (isprint(c)) {
				if (tag-bfr < size-3)
					insert(tag++,c);
				else	beep();
			} else	beep();
		}
	}
}
