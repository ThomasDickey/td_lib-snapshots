#ifndef	lint
static	char	Id[] = "$Id: rcsload.c,v 6.0 1989/10/04 12:04:36 ste_cm Rel $";
#endif	lint

/*
 * Title:	rcsload.c (load delta-tree for an RCS file)
 * Author:	T.E.Dickey
 * Created:	19 Aug 1988
 * $Log: rcsload.c,v $
 * Revision 6.0  1989/10/04 12:04:36  ste_cm
 * BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *
 *		Revision 5.0  89/10/04  12:04:36  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.1  89/10/04  12:04:36  dickey
 *		lint (apollo SR10.1)
 *		
 *		Revision 4.0  89/06/09  13:39:42  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  89/06/09  13:39:42  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.1  89/06/09  13:39:42  dickey
 *		use 'dofree()'
 *		
 *		Revision 2.0  88/09/13  12:46:37  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.4  88/09/13  12:46:37  dickey
 *		sccs2rcs keywords
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

#include	<stdio.h>
#include	<ctype.h>
extern	long	packdate();
extern	char	*ctime();
extern	char	*name2rcs();
extern	char	*rcsread(), *rcsparse_id(), *rcsparse_num(), *rcsparse_str();
extern	char	*txtalloc();

/* local definitions */
#define	CHUNK	31			/* one less than a power of 2 */
#define	def_doalloc RCSTREE_alloc
	/*ARGSUSED*/
	def_DOALLOC(RCSTREE)

static	int	cur_added,
		cur_deleted;
static	int	log_or_edit;

/************************************************************************
 *	local procedures						*
 ************************************************************************/

static
loadtext(c)
{
	static	char	bfr[BUFSIZ];
	static	int	len;
	char	etype;		/* editing type */
	static	int	at, skip;

	if ((c == '\n') || (c == EOS && len > 0)) {
#ifdef	TEST
		static	char	tag[] = ">-=";
		PRINTF("%c>'%s'\n", tag[log_or_edit+1], bfr);
#endif	TEST
		len = 0;

		if (log_or_edit > 0) {
			if (skip > 0)
				skip--;
			else if (sscanf(bfr, "%c%d %d",&etype,&at,&skip) == 3) {
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
	} else if ((c != EOS) && (len < sizeof(bfr)-1)) {
		bfr[len++] = c;
		bfr[len]   = EOS;
	}
	if (c == EOS)
		skip = 0;
}

/*
 * Return a pointer to buffer if the delta's revision is a branch of the
 * specified number of levels.
 */
static
char *
branch(p,n)
RCSTREE	*p;
{
	char	bfr[BUFSIZ];
	register char	*s;

	n = n + n;
	(void)strcpy(bfr, p->revision);
	while (n-- > 0) {
		if (s = strrchr(bfr, '.'))
			*s = EOS;
		else
			return (s);
	}
	s = (strrchr(bfr, '.') != 0) ? txtalloc(bfr) : 0;
	return (s);
}

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/

RCSTREE *
rcsload(archive, verbose)
char	*archive;			/* name of file to open		*/
int	verbose;			/* TRUE if we show messages	*/
{
	static	RCSTREE	nil;		/* empty struct, for terminator */
	RCSTREE	new,			/* current struct, for loading	*/
		*vec = 0;		/* vector of structs to return	*/
	unsigned total = 0;		/* number of items in vector	*/
	char	key[BUFSIZ],
		tmp[BUFSIZ],
		*s	= 0;
	register int j, k, n;
	int	delta	= 0;
	int	yd, md, dd, ht, mt, st;

	if (!rcsopen(name2rcs(archive), -verbose))
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
#ifdef	TEST
				PRINTF("********   %s (%d)\n", key, k);
#endif	TEST
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
			if (sscanf(tmp, FMT_DATE,
				&yd, &md, &dd, &ht, &mt, &st) == 6) {
				newzone(5,0,FALSE);
				new.tstamp = packdate(1900+yd, md,dd, ht,mt,st);
				oldzone();
			}
			break;
		case S_DESC:
			s = rcsparse_str(s, NULL_FUNC);
			delta = 1;
			break;
		case S_LOG:
			log_or_edit = -1;
			s = rcsparse_str(s, loadtext);
			break;
		case S_TEXT:
			log_or_edit = (delta++ == 1) ? 0 : 1;
			cur_added =
			cur_deleted = 0;
			s = rcsparse_str(s, loadtext);
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
						   + vec[k].num_deleted
						   - vec[k].num_added;
				k = j;
				break;
			}
		}

		/*
		 * Process branches.  These are distinguished by the number of
		 * dots in the revision (i.e., "2*n+1").
		 *
		 * patch: no code to support branches yet (88/08/19); must
		 *	work out how to handle it.  generate an error message
		 *	if any branch is found.
		 */
		for (n = 1; n < 2; n++) {
			for (j = 0; j < total; j++) {
				if (strcmp(vec[j].parent,""))	continue;
				if (s = branch(&vec[j],n)) {
					FPRINTF(stderr, "%s: branch %s\n",
						archive, vec[j].revision);
				}
			}
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
#endif	TEST
