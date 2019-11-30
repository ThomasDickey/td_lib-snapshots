/* $Id: td_btree.h,v 12.9 2019/11/30 01:44:04 tom Exp $ */
/* *INDENT-OFF* */

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
	int	(*compare) (const void * a, const void * b);
	BI_NODE*(*allocat) (const void * a);
	void	(*display) (const void * a);
	BI_NODE	head;		/* root data, on end to ease initialization */
	};

	void *	btree_find(
		BI_TREE *	tree,
		const void *	data
			)
		;

	void	btree_dump(
		BI_TREE *	tree
			)
		;

#endif /* TD_BTREE_H */
/* *INDENT-ON* */
