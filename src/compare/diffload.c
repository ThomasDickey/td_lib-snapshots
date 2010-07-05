/*
 * Title:	diffload.c (load delta-tree from difference of files)
 * Author:	T.E.Dickey
 * Created:	25 Feb 1992
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *
 * Function:	Loads one or more files and returns an array of DELTREE
 *		structures, which represent the successive differences between
 *		them.
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	"deltree.h"
#include	"dyn_str.h"
#include	<ctype.h>
#include	<time.h>

MODULE_ID("$Id: diffload.c,v 12.8 2010/07/04 15:28:35 tom Exp $")

#ifdef	TEST
#define	DEBUG(s) PRINTF s;
#else
#define	DEBUG(s)
#endif

/************************************************************************
 *	local procedures						*
 ************************************************************************/

static void
load_file(DELTREE * p, char *name)
{
    p->num_lines =
	p->num_added = file2argv(name, &(p->vector));
    p->buffer = p->vector[0];
}

/*
 */
static void
load_diff(DELTREE * p, char *name1, char *name2)
{
    static const char *verb = "diff -n ";
    static DYN *cmd;

    FILE *pp;
    char tmp1[MAXPATHLEN * 2 + 1];
    char tmp2[MAXPATHLEN * 2 + 1];
    void (*trace) (char *s) = 0;

    (void) name2s(tmp1, sizeof(tmp1), name1, 1);
    (void) name2s(tmp2, sizeof(tmp2), name2, 1);

    dyn_init(&cmd, strlen(verb) + 2 + strlen(tmp1) + strlen(tmp2));
    APPEND(cmd, verb);
    APPEND(cmd, tmp2);
    APPEND(cmd, " ");
    APPEND(cmd, tmp1);

    if ((pp = popen(dyn_string(cmd), "r")) != 0) {
	char **script;
	int length = fp2argv(pp, &script, trace), n;

	(void) pclose(pp);

	p->vector = vecedit(p[-1].vector, script);
	p->buffer = script[0];
	p->num_lines = veclen(p->vector);

	for (n = 0; n < length; n++) {
	    int line, count;
	    char code;

	    if (sscanf(script[n], "%c%d %d\n",
		       &code, &line, &count) == 3) {
		if (code == 'a') {
		    p->num_added += count;
		    n += count;
		} else if (code == 'd')
		    p->num_deleted += count;
	    }
	}
	vecfree(script);
    }
}

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/

DELTREE *
diffload(char **names,		/* names of file(s) to open */
	 int load,		/* TRUE if we load file-text        */
	 int verbose)		/* TRUE if we show messages */
{
    Stat_t sb;
    int total = veclen(names);
    DELTREE *vec = ALLOC(DELTREE, (unsigned) total + 1);
    static DELTREE nil;

    int j;

    (void) load;
    (void) verbose;

    for (j = 0; j < total; j++) {
	vec[j] = nil;
	if (stat_file(names[j], &sb) < 0)
	    break;		/* patch? */

	vec[j].tstamp = sb.st_mtime;
	vec[j].author = uid2s(sb.st_uid);
	vec[j].revision = txtalloc(names[j]);
	vec[j].parent = (j > 0) ? names[j - 1] : txtalloc("");

	if (j == 0) {
	    load_file(vec + j, names[j]);
	} else {
	    load_diff(vec + j, names[j], names[j - 1]);
	}
    }
    vec[j] = nil;

    return vec;
}

/*
 * To release storage, we simply release the vector, since the stuff allocated
 * by 'txtalloc()' is persistent.
 */
void
diffunload(DELTREE * p)		/* vector to release */
{
    if (p != 0) {
	int j;
	for (j = 0; p[j].vector; j++) {
	    if (p[j].buffer != 0)
		free(p[j].buffer);
	    if (p[j].vector != 0)
		vecfree(p[j].vector);
	}
	dofree((char *) p);
    }
}

#ifdef	TEST
static void
compare(char **vector, char *revision)
{
    char temp[BUFSIZ], buffer[BUFSIZ];
    FILE *fp;

    if (!(fp = fopen(tmpnam(temp), "w")))
	failed(temp);

    while (*vector) {
	(void) fputs(*vector, fp);
	vector++;
    }
    FCLOSE(fp);

    *buffer = EOS;
    catarg(buffer, revision);
    shoarg(stdout, "diff", buffer);	/* don't show the temp-name */
    catarg(buffer, temp);
    execute("diff", buffer);
    (void) unlink(temp);
}

/*ARGSUSED*/
_MAIN
{
    DELTREE *p = diffload(argv + 1, TRUE, TRUE);
    int j, k;

    if (p != 0) {
	for (k = 0; p[k].tstamp; k++) {
	    PRINTF("%05d/%05d  %3s => %s \t%s",
		   p[k].num_added,
		   p[k].num_deleted,
		   p[k].parent,
		   p[k].revision,
		   ctime(&p[k].tstamp));
	    if (p[k].vector != 0)
		for (j = 0; p[k].vector[j]; j++)
		    PRINTF("%5d\t%s", j + 1, p[k].vector[j]);
	    compare(p[k].vector, p[k].revision);
	}
	diffunload(p);
    }
    exit(SUCCESS);
}
#endif
