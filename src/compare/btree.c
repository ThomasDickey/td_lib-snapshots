/*
 * Title:	btree.c (binary-tree)
 * Author:	T.E.Dickey
 * Created:	30 Jul 1995, from txtalloc.c
 * Modified:
 *
 * Function:	Maintains a balanced binary tree of unspecified nodes.
 *		The algorithm is taken from "The Art of Computer Programming
 *		-- Volume 3 -- Sorting and Searching", by Donald Knuth.
 */

#include	<td_btree.h>

MODULE_ID("$Id: btree.c,v 12.4 1995/09/04 14:00:34 tom Exp $")

#define	llink	links[0]
#define	rlink	links[1]

			/* definitions to make this simple, like Knuth */
#define	LLINK(p)	p->llink
#define	RLINK(p)	p->rlink
#define	KEY(p)		p->value.text
#define	B(p)		p->balance

#define	LINK(a,p)	p->links[(a)>0]

void *	btree_find(
	_ARX(BI_TREE *,	funcs)
	_AR1(void *,	data)
		)
	_DCL(BI_TREE *,	funcs)
	_DCL(void *,	data)
{
				/* (A1:Initialize) */
register
	BI_NODE	*t = &(funcs->head),	/* 't' => to the father of 's'	*/
		*s = RLINK(t),		/* 's' => to rebalancing point	*/
		*p = RLINK(t),		/* 'p' => down the tree		*/
		*q,
		*r;
register
	int	a;
	char	*value;

	if (p == 0) {
		RLINK(t) = p = (*funcs->allocat)(data);
		return (KEY(p));
	}
				/* (A2:Compare) */
	while ((a = (*funcs->compare)(data, value = KEY(p))) != 0) {
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
			LINK(a,p) = q = (*funcs->allocat)(data);
			value = KEY(q);

			/* (A6:Adjust balance factors) */
			/*
			 * Now the balance factors on nodes between 's' and 'q'
			 * need to be changed from zero to +/- 1.
			 */
			if ((*funcs->compare)(data, KEY(s)) < 0)
				r = p = LLINK(s);
			else
				r = p = RLINK(s);

			while (p != q) {
				if ((a = (*funcs->compare)(data, KEY(p))) != 0) {
					B(p) = (a < 0) ? -1 : 1;
					p = LINK(a,p);
				}
			}

				/* (A7:Balancing act) */
			a = ((*funcs->compare)(data, KEY(s)) < 0) ? -1 : 1;

			if (B(s) == 0) {
				/* ...the tree has grown higher	*/
				B(s) = a;
				funcs->head.llink += 1;
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

static
void	dump_nodes(
	_ARX(BI_TREE *,	funcs)
	_ARX(BI_NODE *,	p)
	_AR1(int,	level)
		)
	_DCL(BI_TREE *,	funcs)
	_DCL(BI_NODE *,	p)
	_DCL(int,	level)
{
	register int	j;

	if (p) {
		dump_nodes(funcs, LLINK(p),  level+1);
		for (j = 0; j < level; j++)
			PRINTF(". ");
		(*funcs->display)(KEY(p));
		PRINTF(" (%d)\n", B(p));
		dump_nodes(funcs, RLINK(p), level+1);
	}
}
/*
 * Invoke display function for each node
 */
void	btree_dump(
	_AR1(BI_TREE *,	funcs)
		)
	_DCL(BI_TREE *,	funcs)
{
	dump_nodes(funcs, funcs->head.rlink, 0);
}
