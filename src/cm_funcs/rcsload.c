#ifndef	lint
static	char	Id[] = "$Id: rcsload.c,v 9.6 1991/09/13 08:56:28 dickey Exp $";
#endif

/*
 * Title:	rcsload.c (load delta-tree for an RCS file)
 * Author:	T.E.Dickey
 * Created:	19 Aug 1988
 * $Log: rcsload.c,v $
 * Revision 9.6  1991/09/13 08:56:28  dickey
 * partial implementation of logic to load file-text
 *
 *		Revision 9.5  91/09/06  12:06:55  dickey
 *		added 'readonly' arg to 'rcsopen()'
 *		
 *		Revision 9.4  91/09/06  10:32:16  dickey
 *		lint
 *		
 *		Revision 9.3  91/09/06  10:28:42  dickey
 *		made this compute line numbers for branches.
 *		corrected line-number computation
 *		
 *		Revision 9.2  91/09/06  08:35:54  dickey
 *		some cleanup, added code for 'author'
 *		
 *		Revision 9.1  91/09/05  11:37:15  dickey
 *		added 'full' argument
 *		
 *		Revision 9.0  91/05/15  09:40:58  ste_cm
 *		BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *		
 *		18 Apr 1990, changed call on 'name2rcs()'
 *		
 *
 * Function:	Scans an RCS archive file, and returns an array of RCSTREE
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
extern	long	packdate();
extern	char	*ctime();
extern	char	*txtalloc();
extern	off_t	filesize();

/* local definitions */
#define	CHUNK	31			/* one less than a power of 2 */
#define	def_doalloc RCSTREE_alloc
	/*ARGSUSED*/
	def_DOALLOC(RCSTREE)

static	int	cur_added,
		cur_deleted;
static	int	log_or_edit;		/* -1=log, 0=text, 1=delta */

					/* data used for loading text */
static	int	load_flag;
static	char	*load_buffer,
		*load_last;

					/* buffer maintained by 'loadtext()' */
static	char	*my_buffer;
static	int	my_length;
static	unsigned my_limit;

/************************************************************************
 *	local procedures						*
 ************************************************************************/

static
append_text(s)
char	*s;
{
	if (load_flag && s != 0) {
		while (*load_last++ = *s++);
	}
}

/*
 * This leaves each record in the file with the editing-code prefixed to the
 * line-contents.
 */
static
append_delta(c, line, s)
char	*s;
{
	if (load_flag) {
		char	temp[80];
		FORMAT(temp, "%c%d", c, line);
		append_text(s);
		append_text(s);
	}
}

static
loadtext(c)
{
	static	char	edit_type;		/* editing type */
	static	int	edit_at, skip;

	if (my_buffer == 0)
		my_buffer = doalloc(my_buffer, my_limit = BUFSIZ);

	if ((c == '\n') || (c == EOS && my_length > 0)) {
#ifdef	TEST
		static	char	tag[] = ">-=";
		PRINTF("%c>'%s'\n", tag[log_or_edit+1], my_buffer);
#endif
		my_length = 0;

		if (log_or_edit > 0) {
			if (skip > 0) {
				skip--;
				edit_at++;
				append_delta(edit_type, edit_at, my_buffer);
			} else if (sscanf(my_buffer,
					"%c%d %d",
					&edit_type, &edit_at, &skip) == 3) {
				if (edit_type == 'a')
					cur_added += skip;
				else if (edit_type == 'd') {
					cur_deleted += skip;
					append_delta(edit_type, edit_at,
						strchr(my_buffer, ' ')+1);
					skip = 0;
				} else	{
#ifdef	TEST
					PRINTF("? unknown edit-code:%c\n",
						edit_type);
					exit(FAIL);
#endif
					skip = 0;
				}
			}
#ifdef	TEST
			else {
				PRINTF("? unexpected record\n");
				exit(FAIL);
			}
#endif
		} else if (log_or_edit == 0) {
			cur_added++;
			append_text(my_buffer);
		}
	} else if (c != EOS) {
		if (my_length >= my_limit-1)
			my_buffer = doalloc(my_buffer, my_limit += BUFSIZ);
		my_buffer[my_length++] = c;
		my_buffer[my_length] = EOS;
	}
	if (c == EOS)
		skip = 0;
}

/*
 * Initialize the text-buffer
 */
static
char	*
eat_text(s, code)
char	*s;
int	code;
{
	char	*base = load_last;

	if ((log_or_edit = code) >= 0) {
		cur_added =
		cur_deleted = 0;
	}
	my_length   = 0;
	s = rcsparse_str(s, loadtext);

	/* compute vector of line-pointers */
	if (base != 0 && log_or_edit >= 0) {
		register int	j;
		register char	*p;
		int	count = 0;
		for (j = 0, p = base; p != load_last; p += strlen(p)+1, j++)
#ifdef	TEST
			PRINTF("%5d:\t%s\n", j+1, p)
#endif
			;
		count = j;
	}

	return s;
}

/*
 * Parse a date
 */
static
time_t
eat_date(arg)
char	*arg;
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
branch_of(rev)
char *rev;
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
fill_branch(vector, at, root)
RCSTREE *vector;
int	at;
char	*root;
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

RCSTREE *
rcsload(archive, full, load, verbose)
char	*archive;			/* name of file to open		*/
int	full;				/* TRUE if we open full path	*/
int	load;				/* TRUE if we load file-text	*/
int	verbose;			/* TRUE if we show messages	*/
{
	static	RCSTREE	nil;		/* empty struct, for terminator */
	RCSTREE	new,			/* current struct, for loading	*/
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

	if (load_flag = load) {
		off_t	length = filesize(name);
		if (length < 0)
			return vec;
		load_buffer = doalloc((char *)0, (unsigned)length);
	} else
		load_buffer = 0;
	load_last = load_buffer;

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
			new.buffer = load_buffer;
			load_buffer = 0;
			vec = DOALLOC(vec,RCSTREE,((total+1)|CHUNK)+1);
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
			break;
		case S_TEXT:
			s = eat_text(s, (delta++ != 1));
			if (k >= 0) {
				vec[k].num_lines   = 0;
				vec[k].num_added   = cur_added;
				vec[k].num_deleted = cur_deleted;
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
rcsunload(p)
RCSTREE	*p;			/* vector to release */
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
main(argc, argv)
char	*argv[];
{
	RCSTREE	*p;
	register int	j, k;

	for (j = 1; j < argc; j++) {
		if (p = rcsload(argv[j], TRUE, TRUE, TRUE)) {
			for (k = 0; p[k].tstamp; k++)
				PRINTF("%05d/%05d  %3s => %s \t%s",
					p[k].num_added,
					p[k].num_deleted,
					p[k].parent,
					p[k].revision,
					ctime(&p[k].tstamp));
			rcsunload(p);
		}
	}
}
#endif
