#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: txtalloc.c,v 12.2 1993/10/29 17:35:23 dickey Exp $";
#endif

/*
 * Title:	txtalloc.c (text-allocator)
 * Author:	T.E.Dickey
 * Created:	29 Apr 1988
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		16 Nov 1992, Added 'free_txtalloc()' for debugging memory leaks.
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		04 Oct 1989, lint (apollo SR10.1)
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
NODE *	new_NODE(
	_AR1(char *,	text))
	_DCL(char *,	text)
{
	register NODE	*p = MYALLOC(NODE,
				sizeof(NODE) + (unsigned)strlen(text));
	(void)strcpy(KEY(p),text);
	LLINK(p) =
	RLINK(p) = 0;
	B(p)     = 0;
	return (p);
}

char *	txtalloc(
	_AR1(char *,	text))
	_DCL(char *,	text)
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
	while ((a = strcmp(text, value = KEY(p))) != 0) {
				/* (A3,A4: move left/right accordingly)	*/
		if ((q = LINK(a,p)) != NULL) {
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
				if ((a = strcmp(text, KEY(p))) != 0) {
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
void	txtfree(
	_AR1(char *,	p))
	_DCL(char *,	p)
{
	/* patch */
}

/******************************************************************************/
static
void	free_NODE(
	_AR1(NODE *,	p))
	_DCL(NODE *,	p)
{
	if (p != 0) {
		free_NODE(p->llink);
		free_NODE(p->rlink);
		dofree((char *)p);
	}
}

void	free_txtalloc(_AR0)
{
	free_NODE(head.rlink);
}

/******************************************************************************/
#ifdef	TEST
void	txtdump(
	_arx(NODE *,	p)
	_ar1(int,	level));

void	txtdump(
	_ARX(NODE *,	p)
	_AR1(int,	level)
		)
	_DCL(NODE *,	p)
	_DCL(int,	level)
{
	register int	j;

	if (p) {
		txtdump(LLINK(p),  level+1);
		for (j = 0; j < level; j++)
			PRINTF(". ");
		PRINTF("%s (%d)\n", KEY(p), B(p));
		txtdump(RLINK(p), level+1);
	}
}

_MAIN
{
	register int	j;
	for (j = 1; j < argc; j++)
		(void)txtalloc(argv[j]);
	txtdump(head.rlink,0);
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
