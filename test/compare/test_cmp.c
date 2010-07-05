#ifndef	NO_IDENT
static const char II[] = "$Id: test_cmp.c,v 12.5 2010/07/05 15:12:52 tom Exp $";
#endif /*lint */

/*
 * Title:	test_scomp.c
 * Author:	T.E.Dickey
 * Created:	05 Jul 1989 (for 'ddif' tests)
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		21 Sep 1993, gcc-warnings
 *
 * Function:	This module acts as a test-driver for the comparison
 *		utilities 'scomp' and 'm2comp'.
 */

static int c_opt;

extern char *ctime(time_t * t);

static char **
load(const char *name, int *num, const char *tag)
{
    char **v;
    Stat_t sb;

    if (stat(name, &sb) < 0)
	failed(name);
    if ((sb.st_mode & S_IFMT) != S_IFREG) {
	FPRINTF(stderr, "%s: not a file\n", name);
	exit(FAIL);
    }
    if ((*num = file2argv(name, &v)) < 0)
	failed(name);
    if (c_opt)
	PRINTF("%s %s\t%s", tag, name, ctime(&sb.st_mtime));
    return (v);
}

static void
freefile(char **vec)
{
    free(vec[0]);
    vecfree(vec);
}

static
SCOMP_MATCH(s_match)
{
    return (strcmp(*p1, *p2) == 0);
}

static
SCOMP_REPORT(s_report)
{
    int both = (lo_1 <= hi_1) && (lo_2 <= hi_2);

    if (lo_1 >= hi_1)
	PRINTF("%d", 1 + hi_1);
    else
	PRINTF("%d,%d", 1 + lo_1, 1 + hi_1);
    PRINTF("%c", both ? 'c' : (lo_1 <= hi_1 ? 'd' : 'a'));
    if (lo_2 >= hi_2)
	PRINTF("%d", 1 + hi_2);
    else
	PRINTF("%d,%d", 1 + lo_2, 1 + hi_2);
    PRINTF("\n");
    while (lo_1 <= hi_1)
	PRINTF("< %s", v1[lo_1++]);
    if (both)
	PRINTF("---\n");
    while (lo_2 <= hi_2)
	PRINTF("> %s", v2[lo_2++]);
}

static int
argv_last(SCOMP_TYPE v,
	  int now,
	  int want)
{
    while (v[now] != 0 && now < want)
	now++;
    return (now);
}

static
SCOMP_REPORT(s_context)
{
    int both = (lo_1 <= hi_1) && (lo_2 <= hi_2);
    int first, last;
    char mark;
    int j;

    PRINTF("***************\n");	/* 15-'*' */

    first = lo_1 - (c_opt - 1);
    if (first < 0)
	first = 0;
    last = argv_last(v1, hi_1, hi_1 + c_opt + 1);
    mark = both ? '!' : '-';

    PRINTF("*** %d,%d\n", first, last);
    for (j = first - 1; j < last; j++) {
	if (j >= 0)
	    PRINTF("%c %s", (j < lo_1 || j > hi_1) ? ' ' : mark, v1[j]);
    }

    first = lo_2 - (c_opt - 1);
    if (first < 0)
	first = 0;
    last = argv_last(v2, hi_2, hi_2 + c_opt + 1);
    mark = both ? '!' : '+';

    PRINTF("\n");
    PRINTF("--- %d,%d -----\n", first, last);
    for (j = first - 1; j < last; j++) {
	if (j >= 0)
	    PRINTF("%c %s", (j < lo_2 || j > hi_2) ? ' ' : mark, v2[j]);
    }
}

_MAIN
{
    char buffer[BUFSIZ];
    void (*func) (SCOMP_REPORT_ARGS(p1, p2)) = s_report;

    setbuf(stdout, buffer);
    if ((argc > 1) && !strncmp(argv[1], "-c", 2)) {
	func = s_context;
	if (sscanf(argv[1], "-c%d", &c_opt) != 1 || (c_opt <= 0))
	    c_opt = 3;
	argc--, argv++;
    }
    if (argc > 2) {
	int n1, n2;
	char **v1 = load(argv[1], &n1, "***");
	char **v2 = load(argv[2], &n2, "---");
	SCOMP((SCOMP_TYPE) v1, n1, (SCOMP_TYPE) v2, n2,
	      sizeof(char *), s_match, func);
	freefile(v1);
	freefile(v2);
    }
    exit(SUCCESS);
    /*NOTREACHED */
}
