#ifndef	lint
static	char	Id[] = "$Id: ftype.c,v 4.1 1989/09/14 15:00:47 dickey Exp $";
#endif	lint

/*
 * Title:	ftype.c (file-type/suffix)
 * Author:	T.E.Dickey
 * Created:	13 Sep 1988
 * $Log: ftype.c,v $
 * Revision 4.1  1989/09/14 15:00:47  dickey
 * made ftype.c portable to vms
 *
 *		Revision 4.0  88/09/13  06:46:16  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/09/13  06:46:16  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/09/13  06:46:16  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.2  88/09/13  06:46:16  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	returns a pointer to the left-most "."-suffix of the leaf of
 *		the given pathname.
 */

extern	char	*strchr();
extern	char	*strrchr();

#ifdef	vms
#define	PATH_END	']'
#else	unix
#define	PATH_END	'/'
#endif	vms/unix

char *
ftype(path)
char *path;
{
	register char	*s,
			*t;

	if ((s = strrchr(path, PATH_END)) == 0)	/* find leaf */
		s = path;
	else	
		s++;
	if ((t = strchr(s, '.')) == 0)		/* ...and suffix in leaf */
		t = s + strlen(s);
	return (t);
}
