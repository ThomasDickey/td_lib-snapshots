/* $Id: td_btree.h,v 12.7 2004/03/07 21:46:31 tom Exp $ */

/*
 * TD_LIB binary-tree functions
 */

#ifndef		TD_BTREE_H
#define		TD_BTREE_H

#ifndef		PTYPES_H
#include	<ptypes.h>
#endif

#define BI_NODE struct _bi_node
	BI_NODE	{
	BI_NODE	*links[2];
	char	 balance;	/* holds 0, -1, +1 */
	union	{		/* force worst-cast alignment */
		char	text[1];
		char	*pointer;
		int	num;
	}	value;		/* we'll return pointer to this data */
	};
#define BI_NODE_NULL {{0,0}, 0, {{0}}}

	/* cf: offsetof */
#define BI_NODE_SIZE ((size_t) ((BI_NODE *)0)->value.text)

#define	BI_NODE_ALLOC(size) (BI_NODE *)doalloc((char *)0, size + BI_NODE_SIZE)

#define BI_TREE struct _bi_tree
	BI_TREE	{
	int	(*compare) (void * a, void * b);
	BI_NODE*(*allocat) (void * a);
	void	(*display) (void * a);
	BI_NODE	head;		/* root data, on end to ease initialization */
	};

	void *	btree_find(
		BI_TREE *	tree,
		void *	data
			)
		;

	void	btree_dump(
		BI_TREE *	tree
			)
		;

#endif /* TD_BTREE_H */
