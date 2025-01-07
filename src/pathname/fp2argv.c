/*
 * Title:	fp2argv (file-pointer to argv-list)
 * Author:	T.E.Dickey
 * Created:	18 Jul 1988
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		17 Nov 1992, modified _FNX macro.
 *		25 Feb 1992, allow input-records to be arbitrarily wide.
 *		04 Oct 1991, conversion to ANSI
 *		20 Jun 1991, added trace-arg for 'ded'
 *
 * Function:	Reads a file via standard I/O and allocates an argv-like
 *		structure containing the file in memory.
 *
 * Arguments:	fp	the file pointer via which to read
 *		argv_	the address of the pointer to the resulting structure.
 *
 * Returns:	the number of lines allocated.
 */

#include	"ptypes.h"
#include	"dyn_str.h"

MODULE_ID("$Id: fp2argv.c,v 12.9 2025/01/07 00:30:52 tom Exp $")

#define	CHUNK	32

static char *
get_line(FILE *fp)
{
    static DYN *bfr;
    static char tmp[] = "?";

    dyn_init(&bfr, (size_t) BUFSIZ);

    do {
	tmp[0] = (char) fgetc(fp);
	if (feof(fp))
	    break;
	(void) dyn_append(bfr, tmp);
	if (tmp[0] == '\n')
	    break;
    } while (!ferror(fp));

    return dyn_length(bfr) ? dyn_string(bfr) : NULL;
}

int
fp2argv(FILE *fp,
	char ***argv_,
	void (*trace) (char *s))
{
    char **vec = NULL;
    int lines = 0;
    unsigned have = 0;
    char *buffer;

    while ((buffer = get_line(fp)) != NULL) {
	unsigned need = (unsigned) (++lines | (CHUNK - 1)) + 1;
	if (need != have) {
	    vec = DOALLOC(vec, char *, need);
	    have = need;
	}
	vec[lines - 1] = stralloc(buffer);
	if (trace != NULL)
	    (*trace) (buffer);
    }
    if (lines == 0)
	vec = DOALLOC(vec, char *, 1);
    vec[lines] = NULL;
    *argv_ = vec;
    return (lines);
}

#ifdef	TEST
_MAIN
{
    char **list;
    int j;

    (void) argc;
    (void) argv;

    fp2argv(stdin, &list, 0);
    for (j = 0; list[j]; j++)
	PRINTF("%d:\t%s", j + 1, list[j]);
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif
