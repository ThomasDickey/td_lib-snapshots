/*
 * Title:	denode.c
 * Author:	T.E.Dickey
 * Created:	22 Jan 1987
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		03 Oct 1991, converted to ANSI
 *
 * Function:	Remove a leading node-name from a path.
 *
 * Arguments:
 *	path - pathname to scan
 *	node - string derived from 'getcwd()'.  If this does not begin with a
 *		'/', we assume that a nodename is in front.
 *	opt -	option/length of 'node':
 *		>= 0, is the length of 'node' from a previous call.
 *		= -1, tells this procedure to find the length of the nodename,
 *		      and to put a null on the end (so we can use strlen to set
 *		      'opt' for successive calls).
 *		< -1, tells this procedure to find the length of the nodename,
 *		      but not to modify 'node[]'.
 *
 * Returns:	A pointer into the 'path' string, past any leading nodename.
 */

#include "ptypes.h"

MODULE_ID("$Id: denode.c,v 12.4 1993/11/28 23:36:40 tom Exp $")

char *	denode (
	_ARX(register char *,	path)
	_ARX(register char *,	node)
	_AR1(int *,		opt)
		)
	_DCL(register char *,	path)
	_DCL(register char *,	node)
	_DCL(int *,		opt)
{
	register char *s = path, *t = node;

	if (opt && (*opt >= 0)) {
		t += *opt;
	} else {
		register int n = 0;
		while (!isSlash(node[n]) && node[n] != EOS)
			n++;
		if (opt) {
			node[n] = EOS;
			*opt = n;
		}
		t += n;
	}

	if (node < t) {
		while (*s) {
			if (node < t) {
				if (*node != *s)	break;
			} else {
				if (isSlash(*s))	path = s;
				break;
			}
			node++, s++;
		}
	}
	return(path);
}
