#ifndef	lint
static	char	Id[] = "$Id: rawgets.c,v 11.0 1991/10/03 17:20:37 ste_cm Rel $";
#endif

/*
 * Author:	T.E.Dickey
 * Title:	rawgets.c (raw-mode 'gets()')
 * Created:	29 Sep 1987 (from 'fl.c')
 * Modified:
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, mods to compile under apollo sr10.3
 *		02 Mar 1990, modified so that if this is invoked in no-wrap
 *			     mode, and the output buffer is wider than the
 *			     screen, we automatically scroll left/right.  Also,
 *			     permit arrow keys to work in non-insert mode. 
 *			     Finally, added a test-driver to exercise the code.
 *		04 Oct 1989, lint (apollo SR10.1)
 *		03 Aug 1989, broke into two procedures, 'rawgets()' and
 *			     'wrawgets()'.  Return the terminating character,
 *			     allowing up/down arrow as one of these.  The
 *			     'wrawgets()' procedure operates solely in a
 *			     specified window; made additional modifications to
 *			     keep the wraparound and clearing under better
 *			     control.
 *		25 Jul 1989, recompiled with apollo SR10 -- mods for function
 *			     prototypes
 *		19 Jan 1989, move to end of string when exiting.
 *		28 Apr 1988, use CTL/B, CTL/F for inline movement
 *		27 Apr 1988, interfaced to 'cmdch()'.  General cleanup to fix
 *			     bugs in wraparound.
 *		24 Nov 1987, moved to my SPC library, use under 'curses'.
 *
 * Function:	Accept input from the screen, permitting a user to either
 *		alter an existing string, or to enter a new one.  This
 *		procedure is called in raw/noecho mode, and is used by the
 *		directory editor to enter new command text.
 *
 *		Only printing (including space) characters will be returned
 *		in 'bfr[]'.
 *
 *		On entry, the caller has printed a prompt/tag to the screen
 *		(before the buffer to be returned), and the screen position
 *		is the end of the string.
 *
 *		On normal exit, the user provides a newline, which is echoed.
 */

#define		CUR_PTYPES
#define		STR_PTYPES
#include	"ptypes.h"
#include	<ctype.h>
#include	"cmdch.h"

#define	SHIFT	5

static	WINDOW	*Z;		/* window we use in this module */
static	int	xbase,	ybase,	/* base-position of 'bfr[]' */
		xlast,		/* last usable column in screen */
		shift,		/* amount shifted in no-wrap mode */
		wrap,		/* if we echo newline, assume wrappable */
		errs,		/* flag for error/illegal command */
		Imode;
static	char	*bbase;		/* 'bfr[]' copy */

/*
 * Clear the remainder of the current line to the 'xlast' position.  Don't
 * use wclrtoeol(), since xlast may not be on the end.
 */
static
ClearIt(_AR0)
{
	register int	x;
	for (x = Z->_curx; x < xlast; x++)
		(void)waddch(Z,' ');
}

/*
 * Position the cursor at the given index in the string.  This permits us to
 * ignore details such as wraparound, and whether the cursor is at the end of
 * string.
 */
static
MoveTo(
_AR1(char *,	new))
_DCL(char *,	new)
{
register
int	y = ybase,
	x = xbase,
	z = new-(bbase+shift);
	int	original = shift;

	while (z < 0) {		/* nowrap: shift-left */
		shift -= SHIFT;
		z += SHIFT;
	}
	while (z-- > 0) {
		if (++x >= xlast) {
			if (wrap) {
				x = 0;
				y++;
			} else {
				shift += SHIFT;
				x -= SHIFT;
			}
		}
	}
	if (shift != original) {
		(void)wmove(Z, ybase, xbase);
		ShowAt(bbase+shift);
	}
	(void)wmove(Z,y,x);
}

/*
 * Repaint the string starting at a given position
 */
static
ShowAt(
_AR1(char *,	at))
_DCL(char *,	at)
{
int	y,x, row, col, len, max;

	getyx(Z, y, x);
	for (row = y, col = x; *at && (row < Z->_maxy); row++) {
		(void)wmove(Z, row, col);
		len = strlen(at);
		max = xlast - col;
		if (len > max)	len = max;
		(void)wprintw(Z,"%.*s", len, at);
		at += len;
		col = 0;
		if (!wrap)	break;
	}
	ClearIt();
	(void)wmove(Z,y,x);
}

/*
 * Insert a character in the screen and into 'bfr[]' at the given position.
 */
static
insert(
_ARX(char *,	at)
_AR1(int,	c)
	)
_DCL(char *,	at)
_DCL(int,	c)
{
register int	d  = c;
register char	*s = at;

	do {
		c = d;
		d = *s;
	} while (*s++ = c);
	ShowAt(at);
	MoveTo(at+1);
}

/*
 * Delete the character(s) before the given position in 'bfr[]'.
 */
static
char *
delete(
_ARX(char *,	at)
_AR1(int,	count)
	)
_DCL(char *,	at)
_DCL(int,	count)
{
	if (at > bbase) {
	int	old, new, x;
	char	*d = at,
		*s = at;

		while (count-- > 0) {
			at--;
			if (--d == bbase)
				break;
		}
		while (*d++ = *s++);

		getyx(Z, old, x);
		MoveTo(at);
		ShowAt(at);

		getyx(Z, new, x);
		while (old > new) {
			(void)wmove(Z,old,0);
			ClearIt();
			old--;
		}
		(void)wmove(Z,new,x);
	} else errs++;
	return(at);
}

/*
 * Toggle the insert/scroll mode, and show the state of this flag by
 * overwriting the ":" position of the prompt which is written before
 * calling this procedure.
 */
static
toggle(_AR0)
{
int	y,x;
	Imode = !Imode;
	getyx(Z,y,x);
	(void)wmove(Z,ybase,xbase-2);
	(void)waddch(Z,Imode ? ':' : '^');
	(void)wmove(Z,y,x);
}

/*
 * Move to end of the buffer
 */
static
char *
move_end(
_ARX(char *,	at)
_AR1(int,	c)
	)
_DCL(char *,	at)
_DCL(int,	c)
{
	if (c == CTL('B'))	at = bbase;
	else if (c == CTL('F'))	at = bbase + strlen(bbase);
	else			errs++;

	if (!errs)		MoveTo(at);
	return (at);
}

/************************************************************************
 *	main procedure							*
 ************************************************************************/

wrawgets (
_ARX(WINDOW *,	win)
_ARX(char *,	bfr)
_ARX(int,	size)
_AR1(int,	newline)
	)
_DCL(WINDOW *,	win)
_DCL(char *,	bfr)
_DCL(int,	size)
_DCL(int,	newline)
{
register char	*tag;
register c;
int	ec = erasechar(),
	kc = killchar(),
	count;

	Z = win;
	Imode = 1;
	errs  = 0;
	bbase = bfr;
	shift = 0;
	getyx(Z,ybase,xbase);		/* get my initial position */
	xlast = xbase + size;
	if (xlast >= Z->_maxx)
		xlast = Z->_maxx - 1;
	if (!(wrap = newline))
		while (strlen(bfr) > (shift + xlast - xbase))
			shift += SHIFT;

	ShowAt((tag = bfr) + shift);
	tag += strlen(tag);
	MoveTo(tag);			/* ...and end-of-string */

	for (;;) {
		if (errs) {
			errs = 0;
			beep();
		} else
			(void)wrefresh(Z);
		c = cmdch(Imode ? (int *)0 : &count);

		/*
		 * We return only one of three types of thing:
		 *	up/down arrow,
		 *	kill-character
		 *	or return/newline
		 * so that we can interlock this with a history-mechanism.
		 */
		if (c == '\n' || c == '\r') {
			(void)move_end(tag,CTL('F'));
			if (newline) (void)waddch(Z,'\n');
			break;
		}
		if (c == ARO_DOWN || c == ARO_UP)
			break;

		if (c == '\t') {
			toggle();
		} else if (!Imode || !isascii(c)) {
			/* process scroll-mode ops */
			if (Imode)
				count = 1;
			if ((c == '\b') || (c == ARO_LEFT)) {
				if (tag > bfr) {
				char	*s = tag;
					while (count-- > 0)
						if (--s == bfr)
							break;
					MoveTo(tag = s);
				} else
					errs++;
			} else if ((c == '\f') || (c == ARO_RIGHT)) {
				if (*tag) {
				char	*s = tag;
					while (count-- > 0)
						if (*(++s) == '\0')
							break;
					MoveTo(tag = s);
				} else
					errs++;
			} else if (c == ec || c == kc) {
				tag = delete(tag,count);
			} else
				tag = move_end(tag,c);
		} else {	/* process insert-mode ops */
			if (c == ec) {
				tag = delete(tag,1);
			} else if (c == kc) {
				count = strlen(bfr);
				(void)delete(bfr+count, count);
				break;
			} else if (isprint(c)) {
				if (tag-bfr < size-3)
					insert(tag++,c);
				else	errs++;
			} else
				tag = move_end(tag,c);
		}
	}
	(void)wrefresh(Z);
	return (c);	/* returns character which terminated this call */
}

rawgets (bfr,size,newline)
char	*bfr;
int	size;
int	newline;
{
	return (wrawgets (stdscr, bfr,size,newline));
}

/************************************************************************
 *	test procedure							*
 ************************************************************************/
#ifdef	TEST
_MAIN
{
	register int	j	= 0;
	auto	 int	wrap	= (argc > 1 && !strcmp(argv[1], "-w"));
	auto	 char	bfr[BUFSIZ];

	initscr();
	rawterm();
	*bfr = EOS;
	while (strlen(bfr) < 3 * COLS) {
		(void)strcat(bfr, "abcdefghijklmnopqrstuvwxyz.");
		(void)sprintf(bfr + strlen(bfr), "%d.", j++);
	}
	for (j = 0; j < LINES; j++) {
		move(j,0);
		clrtobot();
		move(j,0);
		printw("%05d> ", j);
		rawgets(bfr, sizeof(bfr), wrap);
	}
	endwin();
}
#endif	/* TEST */
