/*
 * Title:	view_file.c (create window to view file)
 * Author:	T.E.Dickey
 * Created:	23 Aug 1989
 * Modified:
 *		01 Dec 2019, use executev()
 *		07 Mar 2004, remove K&R support, indent'd.
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

#define STR_PTYPES
#include	"td_curse.h"

MODULE_ID("$Id: viewfile.c,v 12.11 2019/12/17 23:38:28 tom Exp $")

#ifdef	SYS_UNIX

int
view_file(char *fname, int readonly)
{
    int code;
    const char *editor = dftenv(readonly ? "view" : "vi", "EDITOR");

    if ((code = padedit(fname, readonly, editor)) < 0) {
	/* give up: put it in the current process's window */
	size_t need = strlen(editor);
	char **argv = calloc(need + 2, sizeof(char *));
	char *buffer = stralloc(editor);

	if (argv != NULL && buffer != NULL) {
	    int argc = (int) strlen(buffer);
	    argc = bldarg(argc, argv, buffer);

	    argv[argc++] = fname;
	    argv[argc] = NULL;

	    cookterm();
	    code = executev(argv);
	    rawterm();
	    touchwin(curscr);
	    (void) wrefresh(curscr);
	}

	free(argv);
	free(buffer);
    }
    return (code);
}

#endif /* SYS_UNIX */

/******************************************************************************/
#ifdef	TEST
_MAIN
{
    (void) argc;
    (void) argv;
    exit(EXIT_FAILURE);
    /*NOTREACHED */
}
#endif /* TEST */
