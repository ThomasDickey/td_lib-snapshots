/* $Id: td_btree.h,v 12.3 1995/09/04 14:06:47 tom Exp $ */

/*
 * TD_LIB binary-tree functions
 */

#ifndef		TD_BTREE_H
#define		TD_BTREE_H

#ifndef		PTYPES_H
#include	"ptypes.h"
#endif

#define BI_NODE struct _bi_node
	BI_NODE	{
	BI_NODE	*links[2];
	char	 balance;	/* holds 0, -1, +1 */
	union	{		/* force worst-cast alignment */
		char	text[1];
		int	num;
	}	value;		/* we'll return pointer to this data */
	};

	/* cf: offsetof */
#define BI_NODE_SIZE ((size_t) &((BI_NODE *)0)->value.text)

#ifdef lint
#define	BI_NODE_ALLOC(size) (BI_NODE *)(size)
#else
#define	BI_NODE_ALLOC(size) (BI_NODE *)doalloc((char *)0, size + BI_NODE_SIZE)
#endif

#define BI_TREE struct _bi_tree
	BI_TREE	{
	int	(*compare) (_arx(void *,a) _ar1(void *,b));
	BI_NODE*(*allocat) (_ar1(void *,a));
	void	(*display) (_ar1(void *,a));
	BI_NODE	head;		/* root data, on end to ease initialization */
	};

	void *	btree_find(
		_arx(BI_TREE *,	tree)
		_ar1(void *,	data)
			)
		_dcl(BI_TREE *,	tree)
		_dcl(void *,	data)
		_ret

	void	btree_dump(
		_ar1(BI_TREE *,	tree)
			)
		_dcl(BI_TREE *,	tree)
		_nul

#endif /* TD_BTREE_H */
