#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/curses/RCS/rawgets.c,v 3.0 1989/01/19 09:21:22 ste_cm Rel $";
#endif	lint

/*
 * Author:	T.E.Dickey
 * Title:	rawgets.c (raw-mode 'gets()')
 * Created:	29 Sep 1987 (from 'fl.c')
 * $Log: rawgets.c,v $
 * Revision 3.0  1989/01/19 09:21:22  ste_cm
 * BASELINE Mon Jun 19 13:27:01 EDT 1989
 *
 *		Revision 2.0  89/01/19  09:21:22  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.10  89/01/19  09:21:22  dickey
 *		sccs2rcs keywords
 *		
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
#include	"ptypes.h"
#include	<ctype.h>
#include	"cmdch.h"

#define	WRAP	(COLS-1)

static	int	xbase,	ybase,	/* base-position of 'bfr[]' */
		wrap,		/* if we echo newline, assume wrappable */
		errs,		/* flag for error/illegal command */
		Imode;
static	char	*bbase;		/* 'bfr[]' copy */

/*
 * Position the cursor at the given index in the string.  This permits us to
 * ignore details such as wraparound, and whether the cursor is at the end of
 * string.
 */
static
MoveTo(new)
char	*new;
{
register
int	y = ybase,
	x = xbase,
	z = new-bbase;
	while (z-- > 0) {
		if (++x >= WRAP) {
			if (wrap) {
				x = 0;
				y++;
			} else {
				errs++;
				break;
			}
		}
	}
	move(y,x);
}

/*
 * Repaint the string starting at a given position
 */
static
ShowAt(at)
char	*at;
{
int	y,x, row, col, len, max;

	getyx(stdscr, y, x);
	for (row = y, col = x; *at && (row < LINES); row++) {
		move(row, col);
		len = strlen(at);
		max = WRAP - col;
		if (len > max)	len = max;
		(void)printw("%.*s", len, at);
		at += len;
		col = 0;
		if (!wrap)	break;
	}
	clrtoeol();
	move(y,x);
}

/*
 * Insert a character in the screen and into 'bfr[]' at the given position.
 */
static
insert(at,c)
char	*at;
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
delete(at,count)
char	*at;
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

		getyx(stdscr, old, x);
		MoveTo(at);
		ShowAt(at);

		getyx(stdscr, new, x);
		while (old > new) {
			move(old,0);
			clrtoeol();
			old--;
		}
		move(new,x);
	} else errs++;
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
}

/*
 * Move to end of the buffer
 */
static
char *
move_end(at,c)
char	*at;
{
	if (c == CTL(B))	at = bbase;
	else if (c == CTL(F))	at = bbase + strlen(bbase);
	else			errs++;

	if (!errs)		MoveTo(at);
	return (at);
}

/************************************************************************
 *	main procedure							*
 ************************************************************************/

rawgets (bfr,size,newline)
register char	*bfr;
{
register char	*tag;
register c;
int	ec = erasechar(),
	kc = killchar(),
	count;

	Imode = 1;
	errs  = 0;
	bbase = bfr;
	getyx(stdscr,ybase,xbase);	/* get my initial position */
	if (!(wrap  = newline)) {
		if (WRAP - xbase < size)
			size = WRAP - xbase;
		bfr[size-1] = '\0';	/* ...permit initial display/move */
	}

	ShowAt(tag = bfr);
	tag += strlen(tag);
	MoveTo(tag);			/* ...and end-of-string */

	for (;;) {
		if (errs) {
			errs = 0;
			beep();
		} else
			refresh();
		c = cmdch(Imode ? (int *)0 : &count);
		if (c == '\n' || c == '\r') {
			(void)move_end(tag,CTL(F));
			if (newline) addch('\n');
			break;
		}
		if (c == '\t') {
			toggle();
		} else if (!Imode) {	/* process scroll-mode ops */
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
	refresh();
}
