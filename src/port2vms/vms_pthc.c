/*
 * Title:	vms_pathcat.c (VMS path concatenation)
 * Author:	T.E.Dickey
 * Created:	26 Jun 1990
 * Modified:
 *		27 Dec 2014, coverity warnings.
 *		24 May 2010, fix typo in test-driver.
 *		07 Mar 2004, remove K&R support, indent'd.
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

MODULE_ID("$Id: vms_pthc.c,v 12.6 2014/12/27 22:42:38 tom Exp $")

#define	A_NODE	8
#define	A_DEV	4
#define	A_DIR	2
#define	A_NAME	1

char *
vms_pathcat(char *dst, char *dname, char *fname)
{
    char tmp[MAXPATHLEN], *s;
    int f_code;
    char *result = 0;

    if (strlen(dname) < sizeof(tmp)) {
	/* find the highest-level of object in 'fname' */
	if ((s = strchr(fname, ':')) != NULL) {
	    f_code = (s[1] == ':') ? A_NODE : A_DEV;
	} else if ((s = strrchr(fname, ':')) != NULL) {
	    f_code = A_DEV;
	} else if ((s = strchr(fname, ']')) != NULL) {
	    f_code = A_DIR;
	} else {
	    f_code = A_NAME;
	}

	(void) strcpy(tmp, dname);

	/* append to the next-highest level in 'dname' */
	switch (f_code) {
	case A_NODE:		/* replace the entire string */
	    s = tmp;
	    break;
	case A_DEV:		/* append after NODE */
	    if ((s = strchr(tmp, ':')) != NULL) {
		if (s[1] == ':') {
		    s += 2;
		} else {
		    s = tmp;
		}
	    } else {
		s = tmp;
	    }
	    break;
	case A_DIR:		/* append after NODE or DEV (before DIR) */
	    if (!(s = strchr(tmp, '['))) {
		if ((s = strrchr(tmp, ':')) != NULL) {
		    s++;
		} else {
		    s = tmp;
		}
	    }
	    break;
	case A_NAME:		/* append after NODE, DEV or DIR */
	    if ((s = strchr(tmp, ']')) != 0
		|| (s = strrchr(tmp, ':')) != 0) {
		s++;
	    } else {
		s = tmp;
	    }
	    break;
	}

	if ((strlen(fname) + (s - tmp) + 3) < MAXPATHLEN) {
	    (void) strcpy(s, fname);
	    (void) strcpy(dst, tmp);
	    result = dst;
	}
    }
    return result;
}

#ifdef	TEST
static void
do_test(char *path, char *leaf)
{
    char tmp[MAXPATHLEN];
    printf("%s + %s => \"%s\"\n", path, leaf, vms_pathcat(tmp, path, leaf));
}

#define	DO_NAME	16
static char *
do_name(char *dst, int inx)
{
    *dst = EOS;
    if (inx & A_NODE)
	(void) strcat(dst, "node::");
    if (inx & A_DEV)
	(void) strcat(dst, "dev:");
    if (inx & A_DIR)
	(void) strcat(dst, "[dir]");
    if (inx & A_NAME)
	(void) strcat(dst, "name");
    return (dst);
}

/*ARGSUSED*/
_MAIN
{
    int j, k;
    char tmp1[MAXPATHLEN], tmp2[MAXPATHLEN];

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
    /*NOTREACHED */
}
#endif /* TEST */
