#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/cm_funcs/RCS/sccs_dir.c,v 7.0 1988/09/02 08:48:57 ste_cm Rel $";
#endif	lint

/*
 * Title:	sccs_dir.c (returns name of SCCS-directory)
 * Author:	T.E.Dickey
 * Created:	02 Sep 1988
 * $Log: sccs_dir.c,v $
 * Revision 7.0  1988/09/02 08:48:57  ste_cm
 * BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *
 *		Revision 6.0  88/09/02  08:48:57  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  88/09/02  08:48:57  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  88/09/02  08:48:57  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/09/02  08:48:57  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/09/02  08:48:57  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.2  88/09/02  08:48:57  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	Encapsulates the name of the SCCS directory, permitting the
 *		user to override our default by assigning an environment
 *		variable "SCCS_DIR".
 */

extern	char	*getenv();

char *
sccs_dir()
{
	static	char	*name;
	if (name == 0)	name = getenv("SCCS_DIR");
	if (name == 0)	name = "sccs";
	return (name);
}
