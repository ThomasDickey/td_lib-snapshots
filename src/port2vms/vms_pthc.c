#ifndef	lint
static	char	Id[] = "$Id: vms_pthc.c,v 4.0 1990/06/26 14:47:37 ste_cm Rel $";
#endif	lint

/*
 * Title:	vms_pathcat.c (VMS path concatenation)
 * Author:	T.E.Dickey
 * Created:	26 Jun 1990
 * $Log: vms_pthc.c,v $
 * Revision 4.0  1990/06/26 14:47:37  ste_cm
 * BASELINE Tue Aug 14 16:27:44 1990
 *
 *		Revision 3.1  90/06/26  14:47:37  dickey
 *		RCS_BASE
 *		
 * Function:	Forms a combined path a la 'pathcat()', but for VMS-style
 *		pathnames.
 *
 *		Assumes that 'fname' and 'dst' are distinct buffers.  The
 *		'dst' and 'dname' buffers may be the same, though.
 */

#define	STR_PTYPES
#include	"portunix.h"

#define	A_NODE	8
#define	A_DEV	4
#define	A_DIR	2
#define	A_NAME	1

char *
pathcat(dst, dname, fname)
char	*dst, *dname, *fname;
{
	auto	char	tmp[BUFSIZ],
			*s;
	auto	int	f_code;

	/* find the highest-level of object in 'fname' */
	if (s = strchr(fname, ':')) {
		f_code = (s[1] == ':') ? A_NODE : A_DEV;
	} else if (s = strrchr(fname, ':')) {
		f_code = A_DEV;
	} else if (s = strchr(fname, ']')) {
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
		if (s = strchr(tmp, ':')) {
			if (s[1] == ':')
				s += 2;
			else
				s = tmp;
		} else
			s = tmp;
		break;
	case A_DIR:	/* append after NODE or DEV (before DIR) */
		if (!(s = strchr(tmp, '['))) {
			if (s = strrchr(tmp, ':'))
				s++;
			else
				s = tmp;
		}
		break;
	case A_NAME:	/* append after NODE, DEV or DIR */
		if ((s = strchr(tmp, ']')) || (s = strrchr(tmp, ':')))
			s++;
		else
			s = tmp;
	}

	(void)strcpy(s, fname);
	return (strcpy(dst, tmp));
}

#ifdef	TEST
static
do_test(path, leaf)
char	*path, *leaf;
{
	auto	char	tmp[BUFSIZ];
	printf("%s + %s => \"%s\"\n", path, leaf, pathcat(tmp, path, leaf));
}

#define	DO_NAME	16
static
char	*
do_name(dst, inx)
char	*dst;
{
	*dst = EOS;
	if (inx & A_NODE)	(void)strcat(dst, "node::");
	if (inx & A_DEV)	(void)strcat(dst, "dev:");
	if (inx & A_DIR)	(void)strcat(dst, "[dir]");
	if (inx & A_NAME)	(void)strcat(dst, "name");
	return (dst);
}

main(argc, argv)
char	*argv[];
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
#endif
