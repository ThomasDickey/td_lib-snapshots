#ifndef	lint
static	char	Id[] = "$Id: viewfile.c,v 12.0 1991/10/04 07:56:37 ste_cm Rel $";
#endif

/*
 * Title:	view_file.c (create window to view file)
 * Author:	T.E.Dickey
 * Created:	23 Aug 1989
 * Modified:
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

#define		CUR_PTYPES
#include	"ptypes.h"

view_file(
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
		resetty();
		code = execute(editor, fname);
		rawterm();
		touchwin(curscr);
		(void)wrefresh(curscr);
	}
	return (code);
}
