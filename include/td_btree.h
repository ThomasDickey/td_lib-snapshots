/* $Id: td_btree.h,v 12.2 1995/07/30 20:55:58 tom Exp $ */

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
	char	 value[1];	/* we'll return pointer to this data */
	};

#define BI_TREE struct _bi_tree
	BI_TREE	{
	int	(*compare) (_arx(void *,a) _ar1(void *,b));
	BI_NODE*(*allocat) (_ar1(void *,a));
	void	(*display) (_ar1(void *,a));
	BI_NODE	head;		/* root data, on end to ease initialization */
	};

	void *	btree_find(
		_arx(BI_TREE *,	funcs)
		_ar1(void *,	data)
			)
		_dcl(BI_TREE *,	funcs)
		_dcl(void *,	data)
		_ret

	void	btree_dump(
		_ar1(BI_TREE *,	funcs)
			)
		_dcl(BI_TREE *,	funcs)
		_nul

#endif /* TD_BTREE_H */
