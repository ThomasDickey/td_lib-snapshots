/*
 * Title:	vms_relpath.c (convert VMS path to relative-form)
 * Author:	T.E.Dickey
 * Created:	26 Jun 1990
 * Modified:
 *		01 Dec 1993, ifdefs, TurboC warnings.
 *		22 Sep 1993, gcc warnings
 *		20 Nov 1992, use prototypes
 *		18 Oct 1991, use macro _MAIN
 *		20 May 1991, apollo sr10.3 cpp complains about endif-tags
 *		27 Jun 1990, use 'strlcpy()' to ensure that all pathnames are
 *			     lowercased
 *		
 * Function:	Converts a pathname from (presumably absolute form) to relative
 *		form.
 */

#define	STR_PTYPES
#include	"port2vms.h"

MODULE_ID("$Id: vms_rpth.c,v 12.2 1994/08/21 18:39:51 tom Exp $")

static
char *	after_leaf(
	_AR1(char *,	path))
	_DCL(char *,	path)
{
	while (*path) {
		if (*path == '.' || *path == ']')
			break;
		path++;
	}
	return (path);
}

char *	vms_relpath(
	_ARX(char *,	dst)
	_ARX(char *,	cwd)
	_AR1(char *,	src)
		)
	_DCL(char *,	dst)
	_DCL(char *,	cwd)
	_DCL(char *,	src)
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
	||  strncmp(src,cwd, j = (size_t)(s-src)) )
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
		if ((j = (size_t)(s-src)) <= 1)
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
static
void	do_test(
	_ARX(char *,	s)
	_AR1(char *,	wd)
		)
	_DCL(char *,	s)
	_DCL(char *,	wd)
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
		FPRINTF(stderr, "Testing...\n");
		for (j = 0; j < SIZEOF(tbl); j++) {
			for (k = 0; k < SIZEOF(tbl); k++) {
				do_test(strcpy(src, tbl[k]), tbl[j]);
				do_test(strcat(src, "name"), tbl[j]);
			}
		}
		FFLUSH(stdout);
		FPRINTF(stderr, "...done\n");
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
