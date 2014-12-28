/*
 * Title:	rcsload.c (load delta-tree for an RCS file)
 * Author:	T.E.Dickey
 * Created:	19 Aug 1988
 * Modified:
 *		27 Dec 2014, coverity warnings.
 *		05 Jul 2010, use environment's $TMPDIR in test-driver.
 *		07 Mar 2004, remove K&R support, indent'd.
 *		30 May 1998, compile with g++
 *		29 Oct 1993, ifdef-ident
 *		27 Sep 1993, forgot to allow EOS in 'append()'.
 *		21 Sep 1993, gcc-warnings.  Fixes for uninitialized-memory with
 *			     Purify.
 *		18 Nov 1992, fixed a broken loop in 'eat_text()'.  Plugged
 *			     memory leaks.
 *		26 Oct 1992, changed interface to 'rcsread()'.
 *		07 Feb 1992, use 'rcs2time()'
 *		17 Oct 1991, added logic for computing versions of loaded text
 *			     along branches.
 *		10 Oct 1991, got this to compute versions of loaded text on the
 *			     main-trunk.
 *		07 Oct 1991, if file-contents are loaded, provide enough space
 *			     to keep newlines on each record, for compat with
 *			     file2argv/vecedit procedures.
 *		04 Oct 1991, conversion to ANSI
 *		17 Sep 1991, renamed RCSTREE to DELTREE; use prototype-macros
 *		16 Sep 1991, Construct vectors for text of file, plus deltas
 *			     (must next evaluate deltas, also load history-
 *			     text).  Load history-comment also
 *		13 Sep 1991, partial implementation of logic to load file-text
 *		06 Sep 1991, some cleanup, added code for 'author'.  Made this
 *			     compute line numbers for branches.  Corrected line-
 *			     number computation.  Lint.  Added 'readonly' arg
 *			     to 'rcsopen()'.
 *		05 Sep 1991, added 'full' argument
 *		18 Apr 1990, changed call on 'name2rcs()'
 *
 * Function:	Scans an RCS archive file, and returns an array of DELTREE
 *		structures, which represent the information about each delta.
 *
 * patch:	should permit this to load the actual delta text also on the
 *		structure, for use in an RCS viewer.  for now, simply count
 *		the number of lines, insertions and deletions.
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	"rcsdefs.h"
#include	<ctype.h>
#include	<time.h>

MODULE_ID("$Id: rcsload.c,v 12.16 2014/12/27 20:48:55 tom Exp $")

#ifdef	TEST
#define	DEBUG(s) PRINTF s;
#else
#define	DEBUG(s)
#endif

/* local definitions */
#define	CHUNK	31		/* one less than a power of 2 */

static int cur_added, cur_deleted;
static int log_or_edit;		/* -1=log, 0=text, 1=delta */

/* data used for loading text */
static char empty[] = "";
static char **load_vector;	/* one-shot pointer to line-pointers */
static char *load_buffer;	/* one-shot pointer to file-contents */
static char *load_logged;	/* one-shot pointer to log-comment   */
static char *load_last;		/* => current storage in load_buffer */
static char *last_rev = empty;	/* test-only                         */

/* buffer maintained by 'loadtext()' */
static char *my_buffer;
static size_t my_length;
static size_t my_limit;

/************************************************************************
 *	local procedures						*
 ************************************************************************/

#ifdef	TEST
static void
show_vector(const char *tag,
	    const char *rev,
	    char **v)
{
    int j;

    PRINTF("%s:%s\n", tag, rev);
    if (v != 0)
	for (j = 0; v[j]; j++)
	    PRINTF("%5d\t%s", j + 1, v[j]);
}
#define	SHOW_VECTOR(tag,rev,v)	show_vector(tag,rev,v);
#else
#define	SHOW_VECTOR(tag,rev,v)
#endif

static void
append(char *s)
{
    if (load_last != 0 && s != 0) {
	while ((*load_last = *s++) != EOS)
	    load_last++;
	load_last++;		/* skip the EOS I just copied */
    }
}

static void
loadtext(int c)
{
    static char edit_type;	/* editing type */
    static int edit_at, skip;

    if (my_buffer == 0) {
	my_buffer = doalloc(my_buffer, my_limit = BUFSIZ);
	*my_buffer = EOS;
    }

    if (c != EOS) {
	if (my_length >= my_limit - 1)
	    my_buffer = doalloc(my_buffer, my_limit += BUFSIZ);
	my_buffer[my_length++] = (char) c;
	my_buffer[my_length] = EOS;
    }

    if ((c == '\n') || (c == EOS && my_length > 0)) {
#ifdef	TEST2
	static char tag[] = ">-=";
	PRINTF("%c>'%s'", tag[log_or_edit + 1], my_buffer);
#endif
	my_length = 0;

	if (log_or_edit > 0) {
	    if (skip > 0) {
		skip--;
		edit_at++;
		append(my_buffer);
	    } else if (sscanf(my_buffer,
			      "%c%d %d",
			      &edit_type, &edit_at, &skip) == 3) {
		if (edit_type == 'a') {
		    cur_added += skip;
		    append(my_buffer);
		} else if (edit_type == 'd') {
		    cur_deleted += skip;
		    append(my_buffer);
		    skip = 0;
		} else {
#ifdef	TEST2
		    PRINTF("? unknown edit-code:%c\n",
			   edit_type);
		    exit(FAIL);
#endif
		    skip = 0;
		}
	    }
#ifdef	TEST2
	    else {
		PRINTF("? unexpected record\n");
		exit(FAIL);
	    }
#endif
	} else if (log_or_edit == 0) {
	    cur_added++;
	    append(my_buffer);
	} else if (load_last != 0) {
	    append(my_buffer);
	}
    }
    if (c == EOS)
	skip = 0;
}

/*
 * Initialize the text-buffer
 */
static char *
eat_text(char *s, int code)
{
    char *base = load_last;

    if ((log_or_edit = code) >= 0) {
	cur_added =
	    cur_deleted = 0;
    }
    my_length = 0;
    s = rcsparse_str(s, loadtext);

    if (base == 0) ;
    else if (log_or_edit >= 0) {	/* compute vector of line-pointers */
	unsigned j;
	char *p;

	for (j = 0, p = base; p != load_last; p += strlen(p) + 1, j++) ;
	load_vector = vecalloc((size_t) (++j));
	for (j = 0, p = base; p != load_last; p += strlen(p) + 1, j++)
	    load_vector[j] = p;
	load_vector[j] = 0;
	SHOW_VECTOR(log_or_edit ? "DELTA" : "FILE", last_rev, load_vector)
    } else {
	char *t, *d;

	for (t = d = base; t < load_last; s++) {
	    if (t != d)
		(void) strcpy(d, t);
	    d += strlen(d) + 1;
	    t += strlen(t) + 1;
	}
	load_logged = base;
	DEBUG(("NOTES:%s\n%s", last_rev, load_logged))
    }

    return s;
}

/*
 * Returns the (string) revision of which the delta is a branch.  If it is not
 * a branch, return a null pointer.
 */
static char *
branch_of(char *rev)
{
    char bfr[BUFSIZ];
    char *result = 0;

    if (strlen(rev) < sizeof(bfr)) {
	char *s = strcpy(bfr, rev);
	char *t = bfr;

	while (t != 0) {
	    if ((t = strchr(s, '.')) != NULL)
		if ((t = strchr(t + 1, '.')) != NULL)
		    s = t + 1;
	}
	if (s != bfr) {
	    *(--s) = EOS;
	    result = txtalloc(bfr);
	}
    }
    return result;
}

/*
 * Propagate the editing changes from ancestor to descendent
 */
static void
ripple(DELTREE * dst,
       DELTREE * src)
{
    dst->num_lines = src->num_lines
	- dst->num_deleted
	+ dst->num_added;

    if (dst->vector != 0) {
	char **script = dst->vector;
	dst->vector = vecedit(src->vector, script);
	vecfree(script);
    }
}

/*
 * Given an entry in the rcs-tree, recur back to the point at which the
 * branch leaves the trunk to propagate the line counts.
 */
static void
fill_branch(DELTREE * vector,
	    int at,
	    char *root)
{
    int j;
    int found = FALSE;

    for (j = 0; vector[j].parent; j++) {
	if (vector[j].parent == vector[at].revision) {
	    fill_branch(vector, j, root);
	    found = TRUE;
	    break;
	}
    }

    if (!found) {
	for (j = 0; vector[j].parent; j++) {
	    if (vector[j].revision == root) {
		found = TRUE;
		break;
	    }
	}
    }

    if (found)
	ripple(vector + at, vector + j);
}

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/

DELTREE *
rcsload(char *archive,		/* name of file to open             */
	int full,		/* TRUE if we open full path        */
	int load,		/* TRUE if we load file-text        */
	int verbose)		/* TRUE if we show messages */
{
    static DELTREE nil;		/* empty struct, for terminator */
    DELTREE newtree;		/* current struct, for loading  */
    DELTREE *vec = 0;		/* vector of structs to return  */
    size_t total = 0;		/* number of items in vector    */
    char key[BUFSIZ], tmp[BUFSIZ], *name, *s = 0;
    size_t j;
    int k;
    int delta = 0, code = S_FAIL;

    if (!rcsopen(name = name2rcs(archive, full), verbose, TRUE))
	return (vec);

    if (load) {
	unsigned length = 0;
	char *t;

	load_buffer = file2mem(name);
	if (load_buffer == 0)
	    return vec;
	for (t = load_buffer; *t; t++)
	    if (*t == '\n')
		length++;
	DEBUG(("%d newlines, filesize=%d\n", length, t - load_buffer))
	    length += (unsigned) (t - load_buffer) + 2;
	load_buffer = doalloc(load_buffer, (size_t) length);
    } else
	load_buffer = 0;
    load_last = load_buffer;
    load_vector = 0;
    load_logged = 0;

    k = -1;
    memset(&newtree, 0, sizeof(newtree));
    while ((s = rcsread(s, code)) != NULL) {
	s = rcsparse_id(key, s);

	switch (code = rcskeys(key)) {
	case S_HEAD:
	    s = rcsparse_num(tmp, s);
	    break;
	case S_VERS:
	    last_rev = txtalloc(key);
	    if (delta == 0) {
		newtree.revision = last_rev;
	    } else {
		k = -1;
		for (j = 0; j < total; j++)
		    if (last_rev == vec[j].revision) {
			k = (int) j;
			break;
		    }
		if ((RCS_DEBUG > 1) || (k < 0))
		    PRINTF("********   %s (%d)\n", key, k);
	    }
	    break;
	case S_NEXT:
	    s = rcsparse_num(tmp, s);
	    newtree.parent = txtalloc(tmp);
	    newtree.buffer = load_buffer;
	    load_buffer = 0;
	    vec = DOALLOC(vec, DELTREE, ((total + 1) | CHUNK) + 1);
	    vec[total++] = newtree;
	    vec[total] = nil;
	    break;
	case S_DATE:
	    s = rcsparse_num(tmp, s);
	    newtree.tstamp = rcs2time(tmp);
	    break;
	case S_AUTHOR:
	    s = rcsparse_id(tmp, s);
	    newtree.author = txtalloc(tmp);
	    break;
	case S_DESC:
	    s = rcsparse_str(s, (RcsparseStr) 0);
	    delta = 1;
	    break;
	case S_LOG:
	    s = eat_text(s, -1);
	    if (k >= 0) {
		vec[k].logged = load_logged;
		load_logged = 0;
	    }
	    break;
	case S_TEXT:
	    s = eat_text(s, (delta++ != 1));
	    if (k >= 0) {
		vec[k].num_lines = 0;
		vec[k].num_added = cur_added;
		vec[k].num_deleted = cur_deleted;
		vec[k].vector = load_vector;
		load_vector = 0;
	    }
	    break;
	}
	if (!s)
	    break;
    }
    rcsclose();

    /*
     * Revise the added/deleted numbers to a forward sense (for display)
     */
    if (total != 0) {
	/*
	 * First, do the trunk.  We must work backwards from the tip
	 * because that is the entry which holds a complete version of
	 * the file.
	 */
	vec[0].num_lines = vec[0].num_added;
	k = 0;
	while (strcmp(vec[k].parent, "")) {
	    for (j = 0; j < total; j++)
		if (vec[j].revision == vec[k].parent) {
		    ripple(vec + j, vec + k);
		    vec[k].num_added = vec[j].num_added;
		    vec[k].num_deleted = vec[j].num_deleted;
		    vec[j].num_added = vec[j].num_deleted = 0;
		    k = (int) j;
		    break;
		}
	}

	/*
	 * Process branches.  These are distinguished by the number of
	 * dots in the revision (i.e., "2*n+1").  Branches are linked
	 * backwards (i.e., the "parent" is null for the ends of the
	 * branch).
	 */
	for (j = 0; j < total; j++) {
	    if (!strcmp(vec[j].parent, "")
		&& (s = branch_of(vec[j].revision)) != 0)
		fill_branch(vec, (int) j, s);
	}
    }
    return (vec);
}

/*
 * To release storage, we simply release the vector, since the stuff allocated
 * by 'txtalloc()' is persistent.
 */
void
rcsunload(DELTREE * p)		/* vector to release */
{
    if (p != 0) {
	int j;
	for (j = 0; p[j].vector; j++) {
	    if (p[j].buffer != 0)
		dofree(p[j].buffer);
	    if (p[j].vector != 0)
		vecfree(p[j].vector);
	}
	dofree((char *) p);
    }
#ifdef	NO_LEAKS
    if (my_buffer != 0) {
	dofree(my_buffer);
	my_buffer = 0;
	my_limit = 0;
    }
#endif
}

#ifdef	TEST
static void
compare(char *name,
	char **vector,
	char *revision)
{
    char temp[BUFSIZ];
    char version[80];
    char buffer[BUFSIZ];
    FILE *fp;
    const char *tmpdir = getenv("TMPDIR");

    if (tmpdir == 0)
	tmpdir = "/tmp";

    sprintf(temp, "%s/%s", tmpdir, pathleaf(name));
    PRINTF("compare(%s) %s\n", pathleaf(name), revision);
    (void) unlink(temp);

    if (!(fp = fopen(temp, "w")))
	failed(temp);

    while (*vector) {
	(void) fputs(*vector, fp);
	vector++;
    }
    FCLOSE(fp);

    *buffer = EOS;
    catarg(buffer, strcat(strcpy(version, "-r"), revision));
    catarg(buffer, name2rcs(name, TRUE));
    catarg(buffer, temp);
    shoarg(stdout, "rcsdiff", buffer);
    execute("rcsdiff", buffer);
    (void) unlink(temp);
}

/*ARGSUSED*/
_MAIN
{
    DELTREE *p;
    int j, k;

    for (j = 1; j < argc; j++) {
	char *name = argv[j];
	if ((p = rcsload(name, TRUE, TRUE, TRUE)) != NULL) {
	    for (k = 0; p[k].tstamp; k++) {
		PRINTF("%05d/%05d  %3s => %s \t%s",
		       p[k].num_added,
		       p[k].num_deleted,
		       p[k].parent,
		       p[k].revision,
		       ctime(&p[k].tstamp));
#ifdef	TEST2
		SHOW_VECTOR("SHOW", p[k].revision, p[k].vector);
		FFLUSH(stdout);
#endif
		compare(name, p[k].vector, p[k].revision);
	    }
	    rcsunload(p);
	}
    }
#ifdef	NO_LEAKS
    free_txtalloc();
    show_alloc();
#endif
    exit(SUCCESS);
}
#endif
