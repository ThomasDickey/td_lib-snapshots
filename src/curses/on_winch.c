/*
 * Title:	on_winch.c (on SIGWINCH)
 * Author:	T.E.Dickey
 * Created:	27 Jun 1994
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		23 Jul 1994, retain 'saved_winch' to use for HP/UX curses.
 *
 * Function:	Maintains a list of functions to invoke when a SIGWINCH
 *		occurs.  The SIGWINCH signal tells an application when the
 *		window size has changed, e.g, when a user resizes the xterm
 *		within which the process resides.
 *
 *		The functions are called in FIFO order, so that the display
 *		updates in the order in which the functions are called.
 *
 *		Invoking 'on_winch()' with a null argument removes the most
 *		recently attached function.
 */

#define CUR_PTYPES		/* this is part of my curses extensions */
#define SIG_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: on_winch.c,v 12.9 2010/07/04 17:47:36 tom Exp $")

#ifdef SIGWINCH
#define	ON_WINCH struct OnWinch
ON_WINCH {
    ON_WINCH *next;
    void (*func) (void);
};

static RETSIGTYPE catch_winch(SIGNAL_args);

static DCL_SIGNAL(saved_winch);
static ON_WINCH *list;
static int disable_this;
static int caught_this;

/*
 * The 'signal' function returns the last function associated with the given
 * signal.  We assume that if there's another function than ours, that it is
 * the signal handler of either the application, or of a library function such
 * as in HP/UX curses which does display resizing.
 *
 * This scheme will not work if the application sets/clears the signal handler
 * within the calls bracketed by this module.  That is, we assume that HP/UX
 * curses sets its signal handler (only one!) in 'initscr()', and doesn't do
 * anything fancy (like have multiple signal handlers).
 */
static void
set_handler(RETSIGTYPE (*new_handler) (SIGNAL_ARGS))
{
    DCL_SIGNAL(old_handler);

    old_handler = signal(SIGWINCH, new_handler);

    if (old_handler != SIG_DFL
	&& old_handler != SIG_IGN
	&& old_handler != SIG_ERR
	&& old_handler != catch_winch) {
	saved_winch = old_handler;
    }
}

/*
 * Handle the resize by calling the application's resize handler (if any), and
 * then the handlers registered with this module.
 */
static void
handle_resize(void)
{
    if (saved_winch != 0)
	(*saved_winch) (SIGWINCH);

    set_handler(SIG_IGN);
    if (list != 0 && resizewin()) {
	ON_WINCH *p;
	for (p = list; p != 0; p = p->next) {
	    caught_this = FALSE;
	    (*p->func) ();
	}
	refresh();
	caught_this = FALSE;
    }
}

/*
 * Each time we get the window-resized signal, update via the functions that
 * are on the list.  If the list happens to be empty, record this, so that when
 * the list is (re)populated, we can try to display the screen properly.
 *
 * This all assumes that 'on_winch()' is called only in places where it's all
 * right to recompute the display, e.g., while waiting for input.
 */
/*ARGSUSED*/
static
SIGNAL_FUNC(catch_winch)
{
    (void) sig;

    caught_this = TRUE;
    if (!disable_this)
	handle_resize();
    set_handler(catch_winch);
}

void
on_winch(void (*func) (void))
{
    ON_WINCH *p, *q;

    disable_this = TRUE;
    if (func != 0) {		/* add function to end of our list */
	p = ALLOC(ON_WINCH, 1);
	q = list;

	p->next = 0;
	p->func = func;

	if (q != 0) {
	    while (q->next != 0)
		q = q->next;
	    q->next = p;
	} else {
	    list = p;
	}
    } else {			/* remove function on the end of list */
	if ((p = list) != 0) {
	    for (q = 0; p->next != 0; q = p, p = p->next) ;
	    if (q != 0)
		q->next = 0;
	    else
		list = 0;
	    dofree((char *) p);
	}
    }
    disable_this = FALSE;
    if (caught_this)
	handle_resize();
    set_handler(catch_winch);
}

/* This function is called before/after invoking shell processes, to force this
 * module to ignore interrupts.  We do that because some systems (e.g., Linux)
 * get confused when there's more than one application in an xterm that has
 * SIGWINCH enabled (I get a "Level 3 reset").
 *
 * Disabling the interrupt in this way means that 'ded' has to query the screen
 * size when it exits from a subprocess that invokes 'vile', for example.
 */
void
enable_winch(int enabled)
{
    if ((disable_this = !enabled))
	set_handler(SIG_IGN);
    else
	set_handler(catch_winch);
}
#endif /* SIGWINCH */
