#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/curses/RCS/scr_size.c,v 4.0 1988/07/27 13:24:40 ste_cm Rel $";
#endif	lint

/*
 * Author:	T.E.Dickey
 * Title:	scr_size.c (obtain screen size)
 * Created:	27 Jul 1988
 * $Log: scr_size.c,v $
 * Revision 4.0  1988/07/27 13:24:40  ste_cm
 * BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *
 *		Revision 3.0  88/07/27  13:24:40  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/07/27  13:24:40  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.2  88/07/27  13:24:40  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	(Apollo only) inquire to see if the VT100 window has changed
 *		size since curses was initialized.  If so, adjust stdscr and
 *		curscr to match.
 *
 * Returns:	A negative code if any error is found, 0 for normal (termcap)
 *		usage, and a positive number for system-dependent sizing (i.e.,
 *		an Apollo screen).
 */

#ifdef	apollo
#include </sys/ins/base.ins.c>
#endif	apollo
extern	char	*getenv();

#define	my_LINES	retval[0]
#define	my_COLS		retval[1]

scr_size(retval)
int	*retval;
{
	char	i_blk[1024];

	/*
	 * Test for an Apollo screen first, so that we can distinguish between
	 * Apollo's vt100 emulator (return=1) and X-windows (return=0).  If
	 * we reversed this, the Apollo would always go from pad to vt100
	 * and give a value.  Testing Apollo first lets us verify if we are in
	 * a vt100 emulator running under the Display Manager.
	 */
#ifdef	apollo
	extern	void	vte_$inq_screen_size();
	stream_$id_t	id	= ios_$stdout;
	status_$t	st;

	struct	{
		short	height;
		short	width;
		} screen_size;

	vte_$inq_screen_size(&id, &screen_size, &st);
	if (st.all == status_$ok) {
		my_LINES = screen_size.height;
		my_COLS  = screen_size.width;
		return (1);
	}
#endif	apollo

	/*
	 * If we can get the size from termcap, let's believe it.
	 * patch: There doesn't seem to be any way we can get window-resizing
	 * info under X11.2 on Apollo SR9.7
	 */
	if (tgetent(i_blk,getenv("TERM")) >= 0) {
		my_LINES = tgetnum("li");
		my_COLS  = tgetnum("co");
		return (0);
	}
	return (-1);
}

#ifdef	TEST
main()
{
	int	lc[2];
	int	code;

	if ((code = scr_size(lc)) >= 0)
		printf("%d lines, %d columns (code=%d)\n", lc[0], lc[1], code);
	else
		printf("?? cannot determine screen size\n");
	sleep(3);		/* in case we were in a pad */
}
#endif	TEST
