#ifndef	lint
static	char	Id[] = "$Id: denode.c,v 9.0 1991/05/15 10:01:11 ste_cm Rel $";
#endif

/*
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

char	*denode (path, node, opt)
register char	*path, *node;
int	*opt;
{
register char *s = path, *t = node;

	if (opt && (*opt >= 0))
		t += *opt;
	else {
		while ((*t != '/') && *t) t++;
		if (opt) {
			*t = '\0';
			*opt = t - node;
		}
	}

	if (node < t) {
		while (*s) {
			if (node < t) {
				if (*node != *s)	break;
			} else {
				if (*s == '/')		path = s;
				break;
			}
			node++, s++;
		}
	}
	return(path);
}
