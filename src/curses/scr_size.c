#ifndef	lint
static	char	Id[] = "$Id: scr_size.c,v 5.0 1989/10/04 11:44:47 ste_cm Rel $";
#endif	lint

/*
 * Author:	T.E.Dickey
 * Title:	scr_size.c (obtain screen size)
 * Created:	27 Jul 1988
 * $Log: scr_size.c,v $
 * Revision 5.0  1989/10/04 11:44:47  ste_cm
 * BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *
 *		Revision 4.1  89/10/04  11:44:47  dickey
 *		modified (in SR10.1 conversion) so that the TEST-program
 *		generated from this is the same as the 'scr_size' program
 *		I use in running 'rterm' to the sun's
 *		
 *		Revision 4.0  88/07/27  13:24:40  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
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
 * Function:	inquire to see the size of VT100 window
 *
 * Returns:	A negative code if any error is found, 0 for normal (termcap)
 *		usage, and a positive number for system-dependent sizing (i.e.,
 *		an Apollo screen).
 */

#ifdef	apollo
#ifdef	__STDC__
#include <apollo/base.h>
#else
#include </sys/ins/base.ins.c>
#endif	__STDC__
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
#include <stdio.h>
main()
{
	int	lc[2];
	int	code;

	if ((code = scr_size(lc)) >= 0) {
		fprintf(stderr, "%s %d lines, %d columns\n",
			code ? "Termcap" : "Size:",
			lc[0], lc[1]);
	} else
		fprintf(stderr, "?? cannot determine screen size\n");
	if (code <= 0)
		sleep(3);	/* in case we were in a pad */
}
#endif	TEST
