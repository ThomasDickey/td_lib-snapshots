#ifndef	lint
static	char	Id[] = "$Id: rcsload.c,v 9.12 1991/10/07 13:21:04 dickey Exp $";
#endif

/*
 * Title:	rcsload.c (load delta-tree for an RCS file)
 * Author:	T.E.Dickey
 * Created:	19 Aug 1988
 * Modified:
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

#ifdef	TEST
#define	DEBUG(s) PRINTF s;
#else
#define	DEBUG(s)
#endif

/* local definitions */
#define	CHUNK	31			/* one less than a power of 2 */
#define	def_doalloc DELTREE_alloc
	/*ARGSUSED*/
	def_DOALLOC(DELTREE)

static	int	cur_added,
		cur_deleted;
static	int	log_or_edit;		/* -1=log, 0=text, 1=delta */

					/* data used for loading text */
static	char	**load_vector,		/* one-shot pointer to line-pointers */
		*load_buffer,		/* one-shot pointer to file-contents */
		*load_logged,		/* one-shot pointer to log-comment   */
		*load_last;		/* => current storage in load_buffer */

					/* buffer maintained by 'loadtext()' */
static	char	*my_buffer;
static	int	my_length;
static	unsigned my_limit;

/************************************************************************
 *	local procedures						*
 ************************************************************************/

#ifdef	TEST
static
show_vector(
_AR1(char **,v))
_DCL(char **,v)
{
	register int	j;

	PRINTF("TEXT:\n");
	if (v != 0)
		for (j = 0; v[j]; j++)
			PRINTF("%5d\t%s", j+1, v[j]);
}
#define	SHOW_VECTOR(v)	show_vector(v)
#else
#define	SHOW_VECTOR(v)
#endif

static
append(
_AR1(char *,	s))
_DCL(char *,	s)
{
	if (load_last != 0 && s != 0) {
		while (*load_last++ = *s++);
	}
}

static
loadtext(
_AR1(int,	c))
_DCL(int,	c)
{
	static	char	edit_type;		/* editing type */
	static	int	edit_at, skip;

	if (my_buffer == 0)
		my_buffer = doalloc(my_buffer, my_limit = BUFSIZ);

	if (c != EOS) {
		if (my_length >= my_limit-1)
			my_buffer = doalloc(my_buffer, my_limit += BUFSIZ);
		my_buffer[my_length++] = c;
		my_buffer[my_length] = EOS;
	}

	if ((c == '\n') || (c == EOS && my_length > 0)) {
#ifdef	TEST2
		static	char	tag[] = ">-=";
		PRINTF("%c>'%s'", tag[log_or_edit+1], my_buffer);
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
				} else	{
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
static
char	*
eat_text(
_ARX(char *,	s)
_AR1(int,	code)
	)
_DCL(char *,	s)
_DCL(int,	code)
{
	char	*base = load_last;

	if ((log_or_edit = code) >= 0) {
		cur_added =
		cur_deleted = 0;
	}
	my_length   = 0;
	s = rcsparse_str(s, loadtext);

	if (base == 0)
		;
	else if (log_or_edit >= 0) { /* compute vector of line-pointers */
		register unsigned j;
		register char	*p;

		for (j = 0, p = base; p != load_last; p += strlen(p)+1, j++)
			;
		load_vector = vecalloc(++j);
		for (j = 0, p = base; p != load_last; p += strlen(p)+1, j++)
			load_vector[j] = p;
		load_vector[j] = 0;
		SHOW_VECTOR(load_vector);
	} else {
		register char	*p;

		for (p = base; ; *p++ = '\n') {
			p += strlen(p);
			if ((p+1) >= load_last)
				break;
		}
		load_logged = base;
		DEBUG(("NOTES:\n%s", load_logged))
	}

	return s;
}

/*
 * Parse a date
 */
static
time_t
eat_date(
_AR1(char *,	arg))
_DCL(char *,	arg)
{
	time_t	value = 0;
	int	yd, md, dd, ht, mt, st;

	if (sscanf(arg, FMT_DATE,
		&yd, &md, &dd, &ht, &mt, &st) == 6) {
		newzone(5,0,FALSE);
		value = packdate(1900+yd, md,dd, ht,mt,st);
		oldzone();
	} else
		PRINTF("?? date \"%s\"\n", arg);
	return value;
}

/*
 * Returns the (string) revision of which the delta is a branch.  If it is not
 * a branch, return a null pointer.
 */
static
char *
branch_of(
_AR1(char *,	rev))
_DCL(char *,	rev)
{
	char	bfr[BUFSIZ];
	register char	*s = strcpy(bfr, rev), *t = bfr;

	while (t != 0) {
		if (t = strchr(s, '.'))
			if (t = strchr(t + 1, '.'))
				s = t + 1;
	}
	if (s != bfr) {
		*(--s) = EOS;
		return txtalloc(bfr);
	}
	return 0;
}

/*
 * Given an entry in the rcs-tree, recur back to the point at which the
 * branch leaves the trunk to propagate the line counts.
 */
static
fill_branch(
_ARX(DELTREE *,	vector)
_ARX(int,	at)
_AR1(char *,	root)
	)
_DCL(DELTREE *,	vector)
_DCL(int,	at)
_DCL(char *,	root)
{
	register int	j;
	int	found	= FALSE;

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

	if (found) {
		vector[at].num_lines = vector[j].num_lines
				    + vector[at].num_added
				    - vector[at].num_deleted;
	}
}

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/

DELTREE *
rcsload(
_ARX(char *,	archive)		/* name of file to open		*/
_ARX(int,	full)			/* TRUE if we open full path	*/
_ARX(int,	load)			/* TRUE if we load file-text	*/
_AR1(int,	verbose)		/* TRUE if we show messages	*/
	)
_DCL(char *,	archive)
_DCL(int,	full)
_DCL(int,	load)
_DCL(int,	verbose)
{
	static	DELTREE	nil;		/* empty struct, for terminator */
	DELTREE	new,			/* current struct, for loading	*/
		*vec = 0;		/* vector of structs to return	*/
	unsigned total = 0;		/* number of items in vector	*/
	char	key[BUFSIZ],
		tmp[BUFSIZ],
		*name,
		*s	= 0;
	register int j, k;
	int	delta	= 0;

	if (!rcsopen(name = name2rcs(archive,full), verbose, TRUE))
		return(vec);

	if (load) {
		unsigned length = 0;
		load_buffer = file2mem(name);
		if (load_buffer == 0)
			return vec;
		for (s = load_buffer; *s; s++)
			if (*s == '\n')
				length++;
		DEBUG(("%d newlines, filesize=%d\n", length, s - load_buffer))
		length += (s - load_buffer);
		load_buffer = doalloc(load_buffer, length);
	} else
		load_buffer = 0;
	load_last = load_buffer;
	load_vector = 0;
	load_logged = 0;

	k = -1;
	while (s = rcsread(s)) {
		s = rcsparse_id(key, s);

		switch (rcskeys(key)) {
		case S_HEAD:
			s = rcsparse_num(tmp, s);
			break;
		case S_COMMENT:
			s = rcsparse_str(s, NULL_FUNC);
			break;
		case S_VERS:
			if (delta == 0) {
				new.revision = txtalloc(key);
			} else {
				k = -1;
				for (j = 0; j < total; j++)
					if (!strcmp(key, vec[j].revision)) {
						k = j;
						break;
					}
				if ((RCS_DEBUG > 1) || (k < 0))
					PRINTF("********   %s (%d)\n", key, k);
			}
			break;
		case S_NEXT:
			s = rcsparse_num(tmp,s);
			new.parent = txtalloc(tmp);
			new.buffer = load_buffer;	load_buffer = 0;
			vec = DOALLOC(vec,DELTREE,((total+1)|CHUNK)+1);
			vec[total++] = new;
			vec[total+1] = nil;
			break;
		case S_DATE:
			s = rcsparse_num(tmp, s);
			new.tstamp = eat_date(tmp);
			break;
		case S_AUTHOR:
			s = rcsparse_id(tmp, s);
			new.author = txtalloc(tmp);
			break;
		case S_DESC:
			s = rcsparse_str(s, NULL_FUNC);
			delta = 1;
			break;
		case S_LOG:
			s = eat_text(s, -1);
			if (k >= 0) {
				vec[k].logged      = load_logged;
				load_logged = 0;
			}
			break;
		case S_TEXT:
			s = eat_text(s, (delta++ != 1));
			if (k >= 0) {
				vec[k].num_lines   = 0;
				vec[k].num_added   = cur_added;
				vec[k].num_deleted = cur_deleted;
				vec[k].vector      = load_vector;
				load_vector = 0;
			}
			break;
		}
		if (!s)	break;
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
				vec[k].num_added   = vec[j].num_added;
				vec[k].num_deleted = vec[j].num_deleted;
				vec[j].num_added   = vec[j].num_deleted = 0;
				vec[j].num_lines   = vec[k].num_lines
						   - vec[k].num_deleted
						   + vec[k].num_added;
				if (vec[j].vector != 0) {
					char	**script = vec[j].vector;
					vec[j].vector =
						vecedit(vec[k].vector, script);
					vecfree(script);
				}
				k = j;
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
				fill_branch(vec,j,s);
		}
	}
	return (vec);
}

/*
 * To release storage, we simply release the vector, since the stuff allocated
 * by 'txtalloc()' is persistent.
 */
rcsunload(
_AR1(DELTREE *,	p))			/* vector to release */
_DCL(DELTREE *,	p)
{
	if (p != 0) {
		register int	j;
		for (j = 0; p[j].vector; j++) {
			if (p[j].buffer != 0)	free(p[j].buffer);
			if (p[j].vector != 0)	vecfree(p[j].vector);
		}
		dofree((char *)p);
	}
}

#ifdef	TEST
static
compare(
_ARX(char *,	name)
_ARX(char **,	vector)
_AR1(char *,	revision)
	)
_DCL(char *,	name)
_DCL(char **,	vector)
_DCL(char *,	revision)
{
	extern	char	*mktemp();

	char	temp[BUFSIZ],
		version[80],
		buffer[BUFSIZ];
	FILE	*fp;

	strcat(strcpy(temp, "/usr/tmp/"), name);
	printf("compare(%s) %s\n", temp, revision);
	(void)unlink(temp);

	if (!(fp = fopen(temp, "w")))
		failed(temp);

	while (*vector) {
		fputs(*vector, fp);
		vector++;
	}
	FCLOSE(fp);

	*buffer = EOS;
	catarg(buffer, strcat(strcpy(version, "-r"), revision));
	catarg(buffer, name2rcs(name,TRUE));
	catarg(buffer, temp);
	shoarg(stdout, "rcsdiff", buffer);
	execute("rcsdiff", buffer);
	(void)unlink(temp);
}

_MAIN
{
	DELTREE	*p;
	register int	j, k;

	for (j = 1; j < argc; j++) {
		char	*name = argv[j];
		if (p = rcsload(name, TRUE, TRUE, TRUE)) {
			for (k = 0; p[k].tstamp; k++) {
				PRINTF("%05d/%05d  %3s => %s \t%s",
					p[k].num_added,
					p[k].num_deleted,
					p[k].parent,
					p[k].revision,
					ctime(&p[k].tstamp));
				SHOW_VECTOR(p[k].vector);
				compare(name, p[k].vector, p[k].revision);
			}
			rcsunload(p);
		}
	}
	exit(SUCCESS);
}
#endif
