#ifndef	lint
static	char	Id[] = "$Id: rcsload.c,v 9.5 1991/09/06 11:20:50 dickey Exp $";
#endif

/*
 * Title:	rcsload.c (load delta-tree for an RCS file)
 * Author:	T.E.Dickey
 * Created:	19 Aug 1988
 * $Log: rcsload.c,v $
 * Revision 9.5  1991/09/06 11:20:50  dickey
 * added 'readonly' arg to 'rcsopen()'
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

/* local definitions */
#define	CHUNK	31			/* one less than a power of 2 */
#define	def_doalloc RCSTREE_alloc
	/*ARGSUSED*/
	def_DOALLOC(RCSTREE)

static	int	cur_added,
		cur_deleted;
static	int	log_or_edit;

					/* buffer maintained by 'loadtext()' */
static	char	*my_buffer;
static	int	my_length;
static	unsigned my_limit;

/************************************************************************
 *	local procedures						*
 ************************************************************************/

static
loadtext(c)
{
	char	etype;		/* editing type */
	static	int	at, skip;

	if (my_buffer == 0)
		my_buffer = doalloc(my_buffer, my_limit = BUFSIZ);

	if ((c == '\n') || (c == EOS && my_length > 0)) {
#ifdef	TEST
		static	char	tag[] = ">-=";
		PRINTF("%c>'%s'\n", tag[log_or_edit+1], my_buffer);
#endif
		my_length = 0;

		if (log_or_edit > 0) {
			if (skip > 0)
				skip--;
			else if (sscanf(my_buffer, "%c%d %d",&etype,&at,&skip) == 3) {
				if (etype == 'a')
					cur_added += skip;
				else {
					if (etype == 'd')
						cur_deleted += skip;
					skip = 0;
				}
			}
		} else if (log_or_edit == 0) {
			cur_added++;
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
	if ((log_or_edit = code) >= 0) {
		cur_added =
		cur_deleted = 0;
	}
	my_length   = 0;
	return rcsparse_str(s, loadtext);
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
rcsload(archive, full, verbose)
char	*archive;			/* name of file to open		*/
int	full;				/* TRUE if we open full path	*/
int	verbose;			/* TRUE if we show messages	*/
{
	static	RCSTREE	nil;		/* empty struct, for terminator */
	RCSTREE	new,			/* current struct, for loading	*/
		*vec = 0;		/* vector of structs to return	*/
	unsigned total = 0;		/* number of items in vector	*/
	char	key[BUFSIZ],
		tmp[BUFSIZ],
		*s	= 0;
	register int j, k;
	int	delta	= 0;

	if (!rcsopen(name2rcs(archive,full), verbose, TRUE))
		return(vec);

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
	if (p)
		dofree((char *)p);
}

#ifdef	TEST
main(argc, argv)
char	*argv[];
{
	RCSTREE	*p;
	register int	j, k;

	for (j = 1; j < argc; j++) {
		if (p = rcsload(argv[j], TRUE)) {
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

failed(s)
char	*s;
{
	perror(s);
	(void)exit(1);
}
#endif
