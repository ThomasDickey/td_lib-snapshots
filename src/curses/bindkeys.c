/*
 * Title:	bindkeys.c
 * Author:	T.E.Dickey
 * Created:	02 Jul 1994
 * Modified:
 *
 * Function:	provide a key-binding facility for curses applications.
 */

#include <td_curse.h>

MODULE_ID("$Id: bindkeys.c,v 12.7 1995/12/24 22:39:10 tom Exp $")

#define KEYCODE int
#define	BINDKEYS struct BindKeys
BINDKEYS {
	BINDKEYS *child;
	BINDKEYS *sibling;
	KEYCODE  given;
	KEYCODE  result;	/* -1 for intermediate nodes */
};

int	define_key_binding(
		_arx(BINDKEYS **,tablep)
		_arx(KEYCODE *,  definition)
		_ar1(KEYCODE,    result))
		_ret

int	remove_key_binding(
		_arx(BINDKEYS *, tablep)
		_ar1(KEYCODE *,  definition))
		_ret

KEYCODE lookup_key_binding(
		_arx(BINDKEYS *, table)
		_ar1(KEYCODE *,  input))
		_ret

#define MAX_HASH 128	/* FIXME: should I use UCHAR_MAX? */

#define NOT_USED (-2)
#define NOT_DONE (-1)

/*ARGSUSED*/	def_ALLOC(BINDKEYS)

static
BINDKEYS *NewNode(
	_AR1(KEYCODE,	given))
	_DCL(KEYCODE,	given)
{
	BINDKEYS *p = ALLOC(BINDKEYS, 1);
	p->given   = given;
	p->child   = 0;
	p->sibling = 0;
	p->result  = NOT_USED;
	return p;
}

/*
 * Try to find a match for the given value in the parent's children.  Keep the
 * list sorted to avoid subtle problems.
 */
static
BINDKEYS *FillNode(
	_ARX(BINDKEYS *, parent)
	_AR1(KEYCODE,    given)
		)
	_DCL(BINDKEYS *, parent)
	_DCL(KEYCODE,    given)
{
	BINDKEYS *p, *q;
	int	found = FALSE;

	if ((p = parent->child) != 0) {
		q = 0;
		while (p != 0) {
			if (p->given == given) {
				found = TRUE;
				break;
			} else if (p->given > given) {
				break;
			}
			q = p;
			p = p->sibling;
		}
		if (!found) {
			p = NewNode(given);
			if (q != 0) {
				p->sibling = q->sibling;
				q->sibling = p;
			} else {
				parent->child = p;
			}
		}
	} else {	/* allocate first child */
		p = NewNode(given);
		parent->child = p;
	}
	return p;
}

/*
 * For the given key-binding table and result, encode a definition.  The result
 * is a key-code (i.e., a character or an index into the caller's private
 * tables).
 *
 * The table is organized so that the first level's sibling-list acts as a
 * hash table (i.e., we allocate an array).
 */
int
define_key_binding(
	_ARX(BINDKEYS **,tablep)
	_ARX(KEYCODE *,  definition)
	_AR1(KEYCODE,    result)
		)
	_DCL(BINDKEYS **,tablep)
	_DCL(KEYCODE *,  definition)
	_DCL(KEYCODE,    result)
{
	BINDKEYS *parent = *tablep;
	BINDKEYS *p;
	BINDKEYS *q;
	KEYCODE given;
	int	level = 0;

	if (parent == 0) {	/* allocate the top-level */
		BINDKEYS *list = ALLOC(BINDKEYS,MAX_HASH);
		*tablep = p = ALLOC(BINDKEYS,1);
		p->child    = list;
		p->sibling  = 0;
		p->given    = NOT_USED;
		p->result   = NOT_USED;
		for (given = 0; given < MAX_HASH; given++) {
			list[given].child   = 0;
			list[given].sibling = (given+1) < MAX_HASH
					    ? &list[(given+1)]
					    : 0;
			list[given].given   = given;
			list[given].result  = given;
		}
		parent = p;
	}

	while (*definition >= 0) {
		given = *definition++;
		if (level == 0) {
			/*
			 * If 'given' > MAX_HASH, we'll have to extend the
			 * table with a linked-list search
			 */
			if (given >= MAX_HASH) {
				p = &(parent->sibling[MAX_HASH]);
				while ((q = p->sibling) != 0) {
					if (q->given >= given)
						break;
				}
				if (q == 0 || q->given > given) {
					p->sibling = NewNode(given);
					p = p->sibling;
					p->sibling = q;
				} else {
					p = q;
				}
			} else {
				p = &(parent->sibling[given]);
			}
		} else {
			p = FillNode(parent, given);
		}

		/*
		 * This scheme doesn't use timeouts, hence providing a long
		 * definition wipes out abbreviations.
		 *
		 * FIXME: add flag to control overwrite
		 */
		if (*definition >= 0)
			p->result = NOT_DONE;

		parent = p;
		level++;
	}
	parent->result = result;
	return 0;
}

/*
 * Remove a key-binding from the given table.
 */
int
remove_key_binding(
	_ARX(BINDKEYS *, tablep)
	_AR1(KEYCODE *,  definition)
		)
	_DCL(BINDKEYS *, tablep)
	_DCL(KEYCODE *,  definition)
{
	if (lookup_key_binding(tablep, definition) >= 0) {
		/* FIXME */
		return TRUE;
	}
	return FALSE;
}

/*
 * Lookup the input in the key-binding table, returning a positive keycode on
 * success.  As long as there's an ambiguity, return -1 to keep the caller
 * looping.  (Successful keycodes are all positive).
 *
 * The caller is responsible for reallocating the pending list and for popping
 * codes from the list.
 */
KEYCODE
lookup_key_binding(
	_ARX(BINDKEYS *, table)
	_AR1(KEYCODE *,  input)
		)
	_DCL(BINDKEYS *, table)
	_DCL(KEYCODE *,  input)
{
	if (table != 0) {
		int	level = 0;
		BINDKEYS *parent = table;
		BINDKEYS *p = 0;
		while (*input >= 0) {
			KEYCODE given = *input++;
			if (level++ == 0) {
				if (given < MAX_HASH) {
					p = &(table->sibling[given]);
				} else {
					p = &(table->sibling[MAX_HASH-1]);
					while (p != 0) {
						if (p->given >= given)
							break;
						p = p->sibling;
					}
				}
			} else {
				for (p = parent->child; p != 0; p = p->sibling) {
					if (p->given >= given) {
						break;
					}
				}
			}
			if (p == 0 || p->given != given)
				break;
			table = p;
		}
		return p->result;
	}
	return NOT_DONE;
}

#ifdef TEST
_MAIN
{
	/* test by loading a termcap, making symbols from the names, and
	 * binding the keys to those symbols
	 */
	exit(EXIT_SUCCESS);
}
#endif
