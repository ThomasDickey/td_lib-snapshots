/*
 * Title:	txtalloc.c (text-allocator)
 * Author:	T.E.Dickey
 * Created:	29 Apr 1988
 * Modified:
 *		30 Jul 1995, split-out guts as btree.c
 *
 * Function:	Maintains a balanced binary tree of character strings, to
 *		be used in applications where we've got a lot of allocations
 *		that may be for the same data.
 */

#define	STR_PTYPES
#include <td_btree.h>

MODULE_ID("$Id: txtalloc.c,v 12.7 1995/09/14 15:16:58 tom Exp $")

static
BI_NODE	*new_node (
	_AR1(void *,	data))
	_DCL(void *,	data)
{
	char *value = (char *)data;
	size_t need = strlen(value) + 1;
	BI_NODE *result = BI_NODE_ALLOC(need);
	memset(result, 0, BI_NODE_SIZE + need);
	strcpy(result->value.text, value);
	return result;
}

static
int	cmp_node (
	_ARX(void *,	a)
	_AR1(void *,	b)
		)
	_DCL(void *,	a)
	_DCL(void *,	b)
{
	return strcmp((char *)a, (char *)b);
}

static
void	dpy_node (
	_AR1(void *,	a))
	_DCL(void *,	a)
{
	PRINTF("%s", (char *)a);
}

static	BI_TREE	text_tree = {
	cmp_node,
	new_node,
	dpy_node
	};

char *	txtalloc(
	_AR1(char *,	text))
	_DCL(char *,	text)
{
	return btree_find(&text_tree, text);
}

/*
 * Dummy entry for consistency
 */
/*ARGSUSED*/
void	txtfree(
	_AR1(char *,	p))
	_DCL(char *,	p)
{
	/*NOT IMPLEMENTED*/
}

/******************************************************************************/
void	free_txtalloc(_AR0)
{
	/*NOT IMPLEMENTED*/
}

/******************************************************************************/
#ifdef	TEST
_MAIN
{
	register int	j;
	for (j = 1; j < argc; j++)
		(void)txtalloc(argv[j]);
	btree_dump(&text_tree);
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
