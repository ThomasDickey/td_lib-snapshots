/*
 * Title:	vms_pathcat.c (VMS path concatenation)
 * Author:	T.E.Dickey
 * Created:	26 Jun 1990
 * Modified:
 *		01 Dec 1993, ifdefs, TurboC warnings.
 *		22 Sep 1993, gcc warnings
 *		20 Nov 1992, use prototypes
 *		12 Dec 1991, corrected entrypoint-name
 *		18 Oct 1991, use macro _MAIN
 *		20 May 1991, apollo sr10.3 cpp complains about endif-tags
 *		
 * Function:	Forms a combined path a la 'pathcat()', but for VMS-style
 *		pathnames.
 *
 *		Assumes that 'fname' and 'dst' are distinct buffers.  The
 *		'dst' and 'dname' buffers may be the same, though.
 */

#define	STR_PTYPES
#include	"port2vms.h"

MODULE_ID("$Id: vms_pthc.c,v 12.2 1994/08/21 18:39:51 tom Exp $")

#define	A_NODE	8
#define	A_DEV	4
#define	A_DIR	2
#define	A_NAME	1

char *	vms_pathcat(
	_ARX(char *,	dst)
	_ARX(char *,	dname)
	_AR1(char *,	fname)
		)
	_DCL(char *,	dst)
	_DCL(char *,	dname)
	_DCL(char *,	fname)
{
	auto	char	tmp[BUFSIZ],
			*s;
	auto	int	f_code;

	/* find the highest-level of object in 'fname' */
	if ((s = strchr(fname, ':')) != NULL) {
		f_code = (s[1] == ':') ? A_NODE : A_DEV;
	} else if ((s = strrchr(fname, ':')) != NULL) {
		f_code = A_DEV;
	} else if ((s = strchr(fname, ']')) != NULL) {
		f_code = A_DIR;
	} else
		f_code = A_NAME;

	(void)strcpy(tmp, dname);

	/* append to the next-highest level in 'dname' */
	switch (f_code) {
	case A_NODE:	/* replace the entire string */
		s = tmp;
		break;
	case A_DEV:	/* append after NODE */
		if ((s = strchr(tmp, ':')) != NULL) {
			if (s[1] == ':')
				s += 2;
			else
				s = tmp;
		} else
			s = tmp;
		break;
	case A_DIR:	/* append after NODE or DEV (before DIR) */
		if (!(s = strchr(tmp, '['))) {
			if ((s = strrchr(tmp, ':')) != NULL)
				s++;
			else
				s = tmp;
		}
		break;
	case A_NAME:	/* append after NODE, DEV or DIR */
		if ((s = strchr(tmp, ']')) != 0
		 || (s = strrchr(tmp, ':')) != 0)
			s++;
		else
			s = tmp;
	}

	(void)strcpy(s, fname);
	return (strcpy(dst, tmp));
}

#ifdef	TEST
static
void	do_test(
	_ARX(char *,	path)
	_AR1(char *,	leaf)
		)
	_DCL(char *,	path)
	_DCL(char *,	leaf)
{
	auto	char	tmp[BUFSIZ];
	printf("%s + %s => \"%s\"\n", path, leaf, vms_pathcat(tmp, path, leaf));
}

#define	DO_NAME	16
static
char *	do_name(
	_ARX(char *,	dst)
	_AR1(int,	inx)
		)
	_DCL(char *,	dst)
	_DCL(int,	inx)
{
	*dst = EOS;
	if (inx & A_NODE)	(void)strcat(dst, "node::");
	if (inx & A_DEV)	(void)strcat(dst, "dev:");
	if (inx & A_DIR)	(void)strcat(dst, "[dir]");
	if (inx & A_NAME)	(void)strcat(dst, "name");
	return (dst);
}

/*ARGSUSED*/
_MAIN
{
	auto	int	j, k;
	auto	char	tmp1[BUFSIZ],
			tmp2[BUFSIZ];

	if (argc > 1) {
		for (j = 1; j < argc; j++) {
			do_test(argv[1], argv[j]);
		}
	} else {
		for (j = 1; j < DO_NAME; j++)
			for (k = 1; k < DO_NAME; k++) {
				strucpy(tmp2, do_name(tmp2, k));
				do_test(do_name(tmp1, j), tmp2);
			}
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif	/* TEST */
