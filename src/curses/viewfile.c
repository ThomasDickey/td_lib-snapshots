#ifndef	lint
static	char	Id[] = "$Id: viewfile.c,v 9.0 1991/05/15 09:58:34 ste_cm Rel $";
#endif

/*
 * Title:	view_file.c (create window to view file)
 * Author:	T.E.Dickey
 * Created:	23 Aug 1989
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
extern	char	*dftenv();

view_file(fname, readonly)
char	*fname;
int	readonly;
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
