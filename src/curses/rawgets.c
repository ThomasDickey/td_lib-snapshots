#ifndef	lint
static	char	Id[] = "$Id: rawgets.c,v 11.10 1992/08/17 13:17:56 dickey Exp $";
#endif

/*
 * Author:	T.E.Dickey
 * Title:	rawgets.c (raw-mode 'gets()')
 * Created:	29 Sep 1987 (from 'fl.c')
 * Modified:
 *		17 Aug 1992, if 'fast_q' is non-null, start edit in scroll-mode
 *		10 Aug 1992, allow window-arg to be null, for replaying scripts
 *			     to buffers that are not visible.
 *		06 Aug 1992, added command/logging arguments.
 *		05 Aug 1992, highlight in insert-mode when wrap is disabled.
 *			     Added fast-quit and erase-word features.
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
 *		directory editor to enter new text.
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
#include	"dyn_string.h"

#define	SHIFT	5

#define	to_home(c)	(((c) == CTL('B')))
#define	to_left(c)	(((c) == '\b') || ((c) == ARO_LEFT))
#define	to_right(c)	(((c) == '\f') || ((c) == ARO_RIGHT))
#define	to_end(c)	(((c) == CTL('F')))

static	WINDOW	*Z;		/* window we use in this module */
static	char	**Prefix;	/* insert/scrolling prefix, if any */
static	DYN	*history;	/* record of keystrokes if logging active */
static	int	xbase,	ybase,	/* base-position of 'bfr[]' */
		xlast,		/* last usable column in screen */
		shift,		/* amount shifted in no-wrap mode */
		wrap,		/* if we echo newline, assume wrappable */
		errs,		/* flag for error/illegal char */
		Imode;		/* insert:1, scroll:0 */
static	char	*bbase;		/* 'bfr[]' copy */

/*
 * Clear the remainder of the current line to the 'xlast' position.  Don't
 * use wclrtoeol() when wrapping, since xlast may not be on the end.
 */
static
void	ClearIt(_AR0)
{
	if (Z) {
		if (wrap) {
			register int	x;

			for (x = Z->_curx; x < xlast; x++)
				(void)waddch(Z,' ');
		} else {
			(void)wclrtoeol(Z);
		}
	}
}

static
void	ShowAll(_AR0)
{
	if (Z) {
		(void)wmove(Z, ybase, xbase);
		ShowAt(bbase+shift);
	}
}

/*
 * Position the cursor at the given index in the string.  This permits us to
 * ignore details such as wraparound, and whether the cursor is at the end of
 * string.
 */
static
int	MoveTo(
	_AR1(char *,	new))
	_DCL(char *,	new)
{
	if (Z) {
		register int	y = ybase,
				x = xbase,
				z = new-(bbase+shift),
				original = shift;

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
		if (shift != original)
			ShowAll();

		(void)wmove(Z,y,x);
	}
}

/*
 * Repaint the string starting at a given position
 */
static
int	ShowAt(
	_AR1(char *,	at))
	_DCL(char *,	at)
{
	if (Z) {
		register int	y,x, row, col, len, max;

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
}

/*
 * Insert a character in the screen and into 'bfr[]' at the given position.
 */
static
void	InsertAt(
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
char *	DeleteBefore(
	_ARX(char *,	at)
	_AR1(int,	count)
		)
	_DCL(char *,	at)
	_DCL(int,	count)
{
	if (at > bbase) {
		register char	*d = at,
				*s = at;
		register int	old, new, x;

		while (count-- > 0) {
			at--;
			if (--d == bbase)
				break;
		}

		while (*d++ = *s++);

		if (Z) {
			old = Z->_cury;
			MoveTo(at);
			ShowAt(at);
			getyx(Z, new, x);

			while (old > new) {
				(void)wmove(Z,old,0);
				ClearIt();
				old--;
			}
			(void)wmove(Z,new,x);
		}
	} else
		errs++;

	return(at);
}

/*
 * Delete the words before the given pointer
 */
static
char *	DeleteWordBefore(
	_ARX(char *,	at)
	_AR1(int,	count)
		)
	_DCL(char *,	at)
	_DCL(int,	count)
{
	register char	*s;
	register int	found;

	while ((at > bbase) && (count-- > 0)) {
		for (s = at-1, found = 0; s >= bbase; s--) {
			if (isspace(*s)) {
				if (found) {
					s++;	/* point to first nonblank */
					break;
				}
			} else
				found++;
		}
		at = DeleteBefore(at, at - s);
	}
	return (at);
}

/*
 * Show the insert/scroll prefix
 */
static
void	ShowPrefix(_AR0)
{
	if (Z && Prefix) {
		register char	*prefix = Prefix[Imode];

		(void)wstandend(Z);
		(void)wmove(Z, ybase, xbase-strlen(prefix));
		while (*prefix)
			(void)waddch(Z,*prefix++);
	}
}

/*
 * Toggle the insert/scroll mode, and show the state of this flag by
 * overwriting the ":" position of the prompt which is written before
 * calling this procedure.
 */
static
void	ToggleMode(_AR0)
{
	Imode = !Imode;

	if (Z) {
		register int	y,x;

		getyx(Z,y,x);
		ShowPrefix();

		if (!wrap) {
			if (!Imode)
				(void)wstandout(Z);
			ShowAll();
		}

		(void)wmove(Z,y,x);
	}
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
	if (to_home(c))		at = bbase;
	else if (to_end(c))	at = bbase + strlen(bbase);
	else			errs++;

	if (!errs)		MoveTo(at);
	return (at);
}

/************************************************************************
 *	main procedure							*
 ************************************************************************/

int	wrawgets (
	_ARX(WINDOW *,	win)
	_ARX(char *,	bfr)		/* in/out buffer */
	_ARX(char **,	pref)
	_ARX(int,	size)		/* maximum length of 'bfr' */
	_ARX(int,	newline)	/* force newline-echo on completion */
	_ARX(int,	fast_q)		/* nonnull: extra quit character */
	_ARX(char **,	command)	/* nonnull: read inputs */
	_AR1(int,	logging)	/* nonnull: write inputs */
		)
	_DCL(WINDOW *,	win)
	_DCL(char *,	bfr)
	_DCL(char **,	pref)
	_DCL(int,	size)
	_DCL(int,	newline)
	_DCL(int,	fast_q)
	_DCL(char **,	command)
	_DCL(int,	logging)
{
	register char	*tag;
	register int	c,
			EraseChar = erasechar(),
			EraseWord = eraseword(),
			EraseLine = killchar();
	auto	 int	count;
	static	 DYN	*saved;

	saved = dyn_copy(saved, bfr);
	if (logging)
		dyn_init(&history, 1);

	Prefix = pref;
	wrap  = newline;
	Imode = 1;
	errs  = 0;
	bbase = tag = bfr;
	shift = 0;

	if (Z = win) {
		getyx(Z,ybase,xbase);		/* get my initial position */
		ShowPrefix();
		(void)wmove(Z,ybase,xbase);
		xlast = xbase + size;
		if (xlast >= Z->_maxx)
			xlast = Z->_maxx - 1;

		if (!wrap)
			while (strlen(bfr) > (shift + xlast - xbase))
				shift += SHIFT;
		if (fast_q)
			ToggleMode();
		else
			ShowAll();

	} else {
		xbase =
		ybase = 0;
		xlast = 80;
	}

	tag += strlen(tag);
	MoveTo(tag);			/* ...and end-of-string */

	for (;;) {
		if (errs) {
			errs = 0;
			beep();
		}

		if (command && *command && **command) {
			if (Imode || !isdigit(**command))
				count = 1;
			else {
				register char	*s = *command;
				count = 0;
				while (isdigit(*s))
					count = (count * 10) + (*s++ - '0');
				*command = s;
			}
			c = decode_logch(command, (int *)0);
		} else {
			if (Z)
				(void)wrefresh(Z);
			count = 1;
			c = cmdch(Imode ? (int *)0 : &count);
		}
		if (c == EOS)
			continue;

		/*
		 * Record every character that we can infer.
		 */
		if (logging) {
			if (count != 1) {
				char	temp[20];
				FORMAT(temp, "%d", count);
				history = dyn_append(history, temp);
			}
			history = dyn_append_c(history, c);
		}

		/*
		 * We return only one of three types of thing:
		 *	up/down arrow,
		 *	kill-character
		 *	or return/newline
		 * so that we can interlock this with a history-mechanism.
		 */
		if ((c == '\n') || (c == '\r')) {
			MoveTo(bbase + strlen(bbase));
			if (Z && newline)
				(void)waddch(Z,'\n');
			break;
		}
		if ((c == ARO_DOWN) || (c == ARO_UP))
			break;

		if (c == '\t') {
			ToggleMode();
		} else if (!Imode || !isascii(c)) {
			/* process scroll-mode ops */

			if ((c == fast_q)
			 || ((fast_q != EOS) && (c == 'q'))) {
				(void)strcpy(bfr, dyn_string(saved));
				break;
			}

			if (to_left(c)) {
				if (tag > bfr) {
				char	*s = tag;
					while (count-- > 0)
						if (--s == bfr)
							break;
					MoveTo(tag = s);
				} else
					errs++;
			} else if (to_right(c)) {
				if (*tag) {
				char	*s = tag;
					while (count-- > 0)
						if (*(++s) == '\0')
							break;
					MoveTo(tag = s);
				} else
					errs++;
			} else if (c == EraseChar) {
				tag = DeleteBefore(tag,count);
			} else if (c == EraseWord) {
				tag = DeleteWordBefore(tag,count);
			} else if (c == EraseLine) {
				tag = DeleteBefore(tag, tag - bfr);
			} else
				tag = move_end(tag,c);
		} else {	/* process insert-mode ops */
			if (c == EraseChar) {
				tag = DeleteBefore(tag,1);
			} else if (c == EraseWord) {
				tag = DeleteWordBefore(tag,count);
			} else if (c == EraseLine) {
				count = strlen(bfr);
				(void)DeleteBefore(bfr+count, count);
				break;
			} else if (isprint(c)) {
				if (tag-bfr < size-3)
					InsertAt(tag++,c);
				else	errs++;
			} else
				tag = move_end(tag,c);
		}
	}

	if (Z) {
		if (!wrap && !Imode) {
			(void)wstandend(Z);
			ShowAll();
		}
		(void)wrefresh(Z);
	}

	return (c);	/* returns character which terminated this call */
}

/*
 * Returns the logging history from the last call on 'wrawgets()'
 */
char *	rawgets_log(_AR0)
{
	return dyn_string(history);
}

/************************************************************************
 *	test procedure							*
 ************************************************************************/
#ifdef	TEST
_MAIN
{
	register int	j	= 0;
	auto	 int	wrap	= ((argc > 1) && !strcmp(argv[1], "-w"));
	auto	 char	bfr[BUFSIZ];
	static	 char	**pref[] = { "> " : "^ "};

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
		rawgets(bfr, pref, sizeof(bfr), wrap, 'q');
	}
	endwin();
}
#endif	/* TEST */
