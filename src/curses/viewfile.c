/*
 * Title:	view_file.c (create window to view file)
 * Author:	T.E.Dickey
 * Created:	23 Aug 1989
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *
 * Function:	From a process running in 'curses', builds another window
 *		(preferably not in the same process-window) to view a file.
 *
 * Arguments:	fname	- name of file to write the display to
 *		readonly - true if we don't need to write on file (and can
 *			abandon a subprocess).
 *
 * Returns:	-1 if error occurs, 0 if success
 */

#include	"td_curse.h"

MODULE_ID("$Id: viewfile.c,v 12.6 2001/05/15 01:00:00 tom Exp $")

#ifdef	SYS_UNIX

int	view_file(
	_ARX(char *,	fname)
	_AR1(int,	readonly)
		)
	_DCL(char *,	fname)
	_DCL(int,	readonly)
{
	auto	int	code;
	auto	char	*editor = dftenv(readonly ? "view" : "vi", "EDITOR");

	if ((code = padedit(fname, readonly, editor)) < 0) {
		/* give up: put it in the current process's window */
		cookterm();
		code = execute(editor, fname);
		rawterm();
		touchwin(curscr);
		(void)wrefresh(curscr);
	}
	return (code);
}

#endif	/* SYS_UNIX */
