/*
 * Title:	txtalloc.c (text-allocator)
 * Author:	T.E.Dickey
 * Created:	29 Apr 1988
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		30 Jul 1995, split-out guts as btree.c
 *
 * Function:	Maintains a balanced binary tree of character strings, to
 *		be used in applications where we've got a lot of allocations
 *		that may be for the same data.
 */

#define	STR_PTYPES
#include <td_btree.h>

MODULE_ID("$Id: txtalloc.c,v 12.10 2010/07/03 16:24:50 tom Exp $")

static BI_NODE *
new_node(void *data)
{
    char *value = (char *) data;
    size_t need = strlen(value) + 1;
    BI_NODE *result = BI_NODE_ALLOC(need);
    memset(result, 0, BI_NODE_SIZE + need);
    strcpy(result->value.text, value);
    return result;
}

static int
cmp_node(const void *a, const void *b)
{
    return strcmp((const char *) a, (const char *) b);
}

static void
dpy_node(const void *a)
{
    PRINTF("%s", (const char *) a);
}

static BI_TREE text_tree =
{
    cmp_node,
    new_node,
    dpy_node,
    BI_NODE_NULL
};

char *
txtalloc(const char *text)
{
    return btree_find(&text_tree, text);
}

/*
 * Dummy entry for consistency
 */
/*ARGSUSED*/
void
txtfree(char *p)
{
    /*NOT IMPLEMENTED */
    (void) p;
}

/******************************************************************************/
void
free_txtalloc(void)
{
    /*NOT IMPLEMENTED */
}

/******************************************************************************/
#ifdef	TEST
_MAIN
{
    register int j;
    for (j = 1; j < argc; j++)
	(void) txtalloc(argv[j]);
    btree_dump(&text_tree);
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif
