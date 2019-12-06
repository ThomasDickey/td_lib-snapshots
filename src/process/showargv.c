/*
 * Title:	showargv.c (display argv-array)
 * Created:	04 Dec 2019
 * Modified:
 *		05 Dec 2019, add show_argv2(), for cases where verb differs
 */

#define CUR_PTYPES
#include	"ptypes.h"

MODULE_ID("$Id: showargv.c,v 12.2 2019/12/05 10:07:19 tom Exp $")

/*
 * Function:	Displays a command + argument-string, using argv array.
 *
 * Arguments:	fp	- file-pointer to which to write result
 *		argv	- list of arguments, terminated by NULL
 *		compat	- true to add a trailing blank like "shoarg()"
 */
static void
show_argv_x(FILE *fp,
	    const char *verb,
	    char **argv,
	    int compat)
{
    int ch;
    int first = TRUE;
    const char *arg;

    fputc('%', fp);
    while ((arg = *argv++) != NULL) {
	if (first)
	    arg = verb;
	first = FALSE;
	fputc(' ', fp);
	while ((ch = UCH(*arg++)) != EOS) {
	    if (valid_shell_char(ch)) {
		if (ch == ' ' || ch == '\\')
		    fputc('\\', fp);
		fputc(ch, fp);
	    } else if (ch < ' ') {
		fputc('^', fp);
		fputc(ch | '@', fp);
	    } else if (ch == 127) {
		fputc('^', fp);
		fputc('?', fp);
	    } else {
		fprintf(fp, "\\%03o", ch);
	    }
	}
    }
    if (compat)
	fputc(' ', fp);		/* compat with shoarg */
    fputc('\n', fp);
}

void
show_argv(FILE *fp,
	  char **argv)
{
    show_argv_x(fp, argv[0], argv, TRUE);
}

void
show_argv2(FILE *fp,
	   const char *verb,
	   char **argv)
{
    show_argv_x(fp, verb, argv, TRUE);
}

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
