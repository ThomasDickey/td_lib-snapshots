#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: on_winch.c,v 12.2 1994/06/27 23:53:58 tom Exp $";
#endif

/*
 * Title:	on_winch.c (on SIGWINCH)
 * Author:	T.E.Dickey
 * Created:	27 Jun 1994
 * Modified:
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

#define CUR_PTYPES	/* this is part of my curses extensions */
#define SIG_PTYPES
#include "ptypes.h"

#ifdef SIGWINCH
#define	ON_WINCH struct OnWinch
	ON_WINCH {
	ON_WINCH	*next;
	void		(*func)ARGS((void));
	};

/*ARGSUSED*/def_ALLOC(ON_WINCH)

static	ON_WINCH	*list;
static	int		disable_this;
static	int		caught_this;

static
void	handle_resize (_AR0)
{
	(void)signal(SIGWINCH, SIG_IGN);
	if (list != 0 && resizewin()) {
		register ON_WINCH *p;
		for (p = list; p != 0; p = p->next) {
			caught_this = FALSE;
			(*p->func)();
		}
		refresh();
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
static
SIGNAL_FUNC(catch_winch)
{
	caught_this = TRUE;
	if (!disable_this)
		handle_resize();
	(void)signal(SIGWINCH, catch_winch);
}

void	on_winch(
	_FN1(void,	func,(void))
		)
	_DCL(void,	(*func)())
{
	register ON_WINCH *p, *q;

	disable_this = TRUE;
	if (func != 0) {	/* add function to end of our list */
		p = ALLOC(ON_WINCH,1);
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
	} else {		/* remove function on the end of list */
		if ((p = list) != 0) {
			for (q = 0; p->next != 0; q = p, p = p->next)
				;
			if (q != 0)
				q->next = 0;
			else
				list = 0;
			dofree((char *)p);
		}
	}
	disable_this = FALSE;
	if (caught_this)
		handle_resize();
	(void)signal(SIGWINCH, catch_winch);
}
#endif	/* SIGWINCH */
