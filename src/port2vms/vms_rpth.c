#ifndef	lint
static	char	Id[] = "$Id: vms_rpth.c,v 7.0 1991/10/18 15:35:50 ste_cm Rel $";
#endif

/*
 * Title:	vms_relpath.c (convert VMS path to relative-form)
 * Author:	T.E.Dickey
 * Created:	26 Jun 1990
 * $Log: vms_rpth.c,v $
 * Revision 7.0  1991/10/18 15:35:50  ste_cm
 * BASELINE Thu Jul 16 16:14:02 1992 -- CM_TOOLS #11
 *
 *		Revision 6.0  91/10/18  15:35:50  ste_cm
 *		BASELINE Mon Oct 21 13:09:39 1991 -- adapt to CM_TOOLS #10
 *		
 *		Revision 5.1  91/10/18  15:35:50  dickey
 *		use macro _MAIN
 *		
 *		Revision 5.0  91/05/20  17:18:26  ste_cm
 *		BASELINE Tue Jun 11 16:44:53 1991 -- apollo sr10.3
 *		
 *		Revision 4.1  91/05/20  17:18:26  dickey
 *		apollo sr10.3 cpp complains about endif-tags
 *		
 *		Revision 4.0  90/06/27  09:59:09  ste_cm
 *		BASELINE Tue Aug 14 16:27:44 1990
 *		
 *		Revision 3.2  90/06/27  09:59:09  dickey
 *		use 'strlcpy()' to ensure that all pathnames are lowercased
 *		
 *		Revision 3.1  90/06/27  08:29:24  dickey
 *		RCS_BASE
 *		
 * Function:	Converts a pathname from (presumably absolute form) to relative
 *		form.
 */

#define	STR_PTYPES
#include	"portunix.h"
extern	char	*strlcpy();

static
char	*
after_leaf(path)
char	*path;
{
	while (*path) {
		if (*path == '.' || *path == ']')
			break;
		path++;
	}
	return (path);
}

char	*
vms_relpath(dst, cwd, src)
char	*dst, *cwd, *src;
{
	auto	char	current[BUFSIZ];
	auto	char	source[BUFSIZ];
	auto	size_t	j;
	auto	char	*s, *d;
	auto	int	matched;

	src = strlcpy(source, src);	/* dst may be the same as src */
	if (cwd == 0) {		/* if cwd not given, get the actual path */
		cwd = getwd(current);
#ifdef	unix
		cwd = name2vms(current, strcat(cwd, "/"));
#endif
	} else
		cwd = strlcpy(current, cwd);

	/* We must have a directory-path in 'src' to change, as well as obtain
	 * a working-directory. Further, VMS pathnames always contain at least
	 * one colon (following the device), and a pair of square brackets
	 * (delineating the pathname).
	 *
	 * If any of the syntax checks fail, or if the src-path is not on the
	 * same node and device as the cwd-path, we cannot compute a relative
	 * pathname, and simply copy the src-path to the dst-argument.
	 */
	if (cwd == 0
	||  (s = strrchr(src, ':')) == 0
	||  (d = strrchr(cwd, ':')) == 0
	||  (s+1 != strchr(src, '['))
	||  (d+1 != strchr(cwd, '['))
	||  (s-src != d-cwd)
	||  strncmp(src,cwd, j = (s-src)) )
		return (strcpy(dst, src));

	/* If we made it past that, 'j' contains the distance to the '[' in
	 * each path, past the "node::dev:", if any.
	 */
	src += j + 1;
	cwd += j + 1;
	*dst = EOS;

	/* Skip over the portion of the pathnames in 'src[]' and 'cwd[]' which
	 * are common to each.
	 */
	for (matched = 0;;) {
		s = after_leaf(src+1);
		d = after_leaf(cwd+1);
		if ((j = (s-src)) <= 1)
			continue;
		if (s-src != d-cwd)
			break;
		if (strncmp(src+1,cwd+1,j-1))
			break;
		matched++;
		src = s;
		cwd = d;
		if (*s != '.' || *d != '.')
			break;
	}

	/*
	 * If the paths are a complete mismatch, still make this relative.
	 */
	if (!matched) {
		src++;		/* skip over '[' */
		cwd++;
		(void)strcpy(dst, (*cwd == '.' || *src == '.') ? "[-" : "[-.");
	}

	/*
	 * Any remaining path-levels in 'cwd[]' result in '-' marks appended
	 * to the destination.
	 */
	while (*cwd == '.') {
		if (*dst == EOS)
			(void)strcpy(dst, "[");
		(void)strcat(dst, "-");
		cwd = after_leaf(++cwd);
	}

	/*
	 * Append the remainder of 'src' to the destination argument. This is
	 * the result that we return.
	 */
	if (*dst == EOS)
		(void)strcpy(dst, "[");
	if (!strcmp(dst, "[") && *src == ']' && src[1] != EOS) {
		*dst = EOS;
		src++;
	}
	return (strcat(dst, src));
}

#ifdef	TEST
do_test(s,wd)
char	*s;
char	*wd;
{
	auto	char	dst[BUFSIZ];
	if (wd == 0) wd = "(getwd)";
	printf("%-24s %-24s => %-24s\n", wd, s, vms_relpath(dst, wd, s));
}

/*ARGSUSED*/
_MAIN
{
	static	char	*tbl[] = {
		 "dev:[1st]"
		,"dev:[aaa]"
		,"dev:[1st.2nd]"
		,"dev:[1st.bbb]"
		,"dev:[1st.2nd.3rd]"
		,"dev:[1st.2nd.ccc]"
		};
	auto	char	src[BUFSIZ];
	register int	j, k;

	if (argc > 1) {
		for (j = 1; j < argc; j++)
			do_test(argv[j], (char *)0);
	} else {
		fprintf(stderr, "Testing...\n");
		for (j = 0; j < sizeof(tbl)/sizeof(tbl[0]); j++) {
			for (k = 0; k < sizeof(tbl)/sizeof(tbl[0]); k++) {
				do_test(strcpy(src, tbl[k]), tbl[j]);
				do_test(strcat(src, "name"), tbl[j]);
			}
		}
		fprintf(stderr, "...done\n");
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
