#ifndef	lint
static	char	Id[] = "$Id: rcs_dir.c,v 9.0 1991/05/15 10:04:43 ste_cm Rel $";
#endif

/*
 * Title:	rcs_dir.c (returns name of RCS-directory)
 * Author:	T.E.Dickey
 * Created:	02 Sep 1988
 * $Log: rcs_dir.c,v $
 * Revision 9.0  1991/05/15 10:04:43  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  10:04:43  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  88/09/02  08:48:16  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  88/09/02  08:48:16  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  88/09/02  08:48:16  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  88/09/02  08:48:16  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  88/09/02  08:48:16  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/09/02  08:48:16  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/09/02  08:48:16  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.2  88/09/02  08:48:16  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	Encapsulates the name of the RCS directory, permitting the
 *		user to override our default by assigning an environment
 *		variable "RCS_DIR".
 */

extern	char	*getenv();

char *
rcs_dir()
{
	static	char	*name;
	if (name == 0)	name = getenv("RCS_DIR");
	if (name == 0)	name = "RCS";
	return (name);
}
