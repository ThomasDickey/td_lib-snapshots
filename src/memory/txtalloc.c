#ifndef	lint
static	char	Id[] = "$Id: txtalloc.c,v 9.0 1991/05/15 09:56:43 ste_cm Rel $";
#endif

/*
 * Title:	txtalloc.c (text-allocator)
 * Author:	T.E.Dickey
 * Created:	29 Apr 1988
 * $Log: txtalloc.c,v $
 * Revision 9.0  1991/05/15 09:56:43  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  09:56:43  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  89/10/04  13:04:16  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  89/10/04  13:04:16  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/10/04  13:04:16  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/10/04  13:04:16  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.1  89/10/04  13:04:16  dickey
 *		lint (apollo SR10.1)
 *		
 *		Revision 4.0  88/05/17  09:23:59  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/05/17  09:23:59  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/05/17  09:23:59  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.5  88/05/17  09:23:59  dickey
 *		sccs2rcs keywords
 *		
 *		16 May 1988, perform a single 'doalloc()' call for each node
 *		10 May 1988, added dummy 'txtfree()'
 *
 * Function:	Maintains a balanced binary tree of character strings.
 *		The algorithm is taken from "The Art of Computer Programming
 *		-- Volume 3 -- Sorting and Searching", by Donald Knuth.
 */

#define	STR_PTYPES
#include	"ptypes.h"

typedef	struct	_node	{
	struct	_node	*links[2];
	char		balance;	/* holds 0, -1, +1 */
	char		value[1];
	} NODE;

#define	llink	links[0]
#define	rlink	links[1]

			/* definitions to make this simple, like Knuth */
#define	LLINK(p)	p->llink
#define	RLINK(p)	p->rlink
#define	KEY(p)		p->value
#define	B(p)		p->balance

#define	LINK(a,p)	p->links[(a)>0]

#ifdef	lint
#define	MYALLOC(c,n)	(c *)0
#else
#define	MYALLOC(c,n)	(c *)doalloc((char *)0, (n))
#endif

static	NODE	head;

static
NODE *
new_NODE(text)
char	*text;
{
register
NODE	*p = MYALLOC(NODE, sizeof(NODE) + strlen(text));
	(void)strcpy(KEY(p),text);
	LLINK(p) =
	RLINK(p) = 0;
	B(p)     = 0;
	return (p);
}

char *
txtalloc(text)
char	*text;
{
				/* (A1:Initialize) */
register
NODE	*t	= &head,	/* 't' points to the father of 's'	*/
	*s	= RLINK(t),	/* 's' points to rebalancing point	*/
	*p	= RLINK(t),	/* 'p' moves down the tree		*/
	*q,
	*r;
register
int	a;
char	*value;

	if (p == 0) {
		RLINK(t) = p = new_NODE(text);
		return (KEY(p));
	}
				/* (A2:Compare) */
	while (a = strcmp(text, value = KEY(p))) {
				/* (A3,A4: move left/right accordingly)	*/
		if (q = LINK(a,p)) {
			if (B(q)) {
				t = p;
				s = q;
			}
			p = q;
			/* ...continue comparing */
		} else {
			/* (A5:Insert) */
			LINK(a,p) = q = new_NODE(text);
			value = KEY(q);

			/* (A6:Adjust balance factors) */
			/*
			 * Now the balance factors on nodes between 's' and 'q'
			 * need to be changed from zero to +/- 1.
			 */
			if (strcmp(text, KEY(s)) < 0)
				r = p = LLINK(s);
			else
				r = p = RLINK(s);

			while (p != q) {
				if (a = strcmp(text, KEY(p))) {
					B(p) = (a < 0) ? -1 : 1;
					p = LINK(a,p);
				}
			}

				/* (A7:Balancing act) */
			a = (strcmp(text, KEY(s)) < 0) ? -1 : 1;

			if (B(s) == 0) {
				/* ...the tree has grown higher	*/
				B(s) = a;
				head.llink++;
			} else if (B(s) == -a) {
				/* ...the tree has gotten more balanced	*/
				B(s) = 0;
			} else if (B(s) == a) {
				/* ...the tree has gotten out of balance */
				if (B(r) == a) {
					/* (A8:Single rotation) */
					p          = r;
					LINK(a,s)  = LINK(-a,r);
					LINK(-a,r) = s;

					B(s) = B(r) = 0;
				} else if (B(r) == -a) {
					/* (A9: Double rotation) */
					p          = LINK(-a,r);
					LINK(-a,r) = LINK(a,p);
					LINK(a,p)  = r;
					LINK(a,s)  = LINK(-a,p);
					LINK(-a,p) = s;

					if (B(p) == a)
						{ B(s) = -a; B(r) = 0;	}
					else if (B(p) == 0)
						{ B(s) =     B(r) = 0;  }
					else if (B(p) == -a)
						{ B(s) = 0;  B(r) = a;  }

					B(p) = 0;
				}
				/* A10:Finishing touch */
				t->links[(s == RLINK(t))] = p;
			}
			break;
		}
	}
	return (value);
}

/*
 * Dummy entry for consistency
 */
/*ARGSUSED*/
txtfree(p)
char	*p;
{
}

#ifdef	TEST
txtdump(p, level)
NODE	*p;
{
int	j;

	if (p) {
		txtdump(LLINK(p),  level+1);
		for (j = 0; j < level; j++)
			printf(". ");
		printf("%s (%d)\n", KEY(p), B(p));
		txtdump(RLINK(p), level+1);
	}
}

main(argc, argv)
char	*argv[];
{
int	j;
	for (j = 1; j < argc; j++)
		(void)txtalloc(argv[j]);
	txtdump(head.rlink,0);
	/*NOTREACHED*/
	exit(0);
}

failed(s)
char	*s;
{
	perror(s);
	exit(1);
}
#endif
