#ifndef	lint
static	char	II[] = "$Id: test_cmp.c,v 11.1 1992/11/19 15:16:06 dickey Exp $";
#endif/*lint*/

/*
 * Title:	test_scomp.c
 * Author:	T.E.Dickey
 * Created:	05 Jul 1989 (for 'ddif' tests)
 * Modified:
 *
 * Function:	This module acts as a test-driver for the comparison
 *		utilities 'scomp' and 'm2comp'.
 */

#define	STR_PTYPES
#include "ptypes.h"

static	int	c_opt;

static
char **	load(
	_ARX(char *,	name)
	_ARX(int *,	num)
	_AR1(char *,	tag)
		)
	_DCL(char *,	name)
	_DCL(int *,	num)
	_DCL(char *,	tag)
{
	extern	char *	ctime();
	auto	char **	v;
	auto	STAT	sb;

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


static
SCOMP_MATCH(s_match)
{
	return (strcmp(p1,p2) == 0);
}

static
SCOMP_REPORT(s_report)
{
	auto	int	both	= (lo_1 <= hi_1) && (lo_2 <= hi_2);

	if (lo_1 >= hi_1)	PRINTF("%d",    1+hi_1);
	else			PRINTF("%d,%d", 1+lo_1, 1+hi_1);
	PRINTF("%c", both ? 'c' : (lo_1 <= hi_1 ? 'd' : 'a'));
	if (lo_2 >= hi_2)	PRINTF("%d",    1+hi_2);
	else			PRINTF("%d,%d", 1+lo_2, 1+hi_2);
	PRINTF("\n");
	while (lo_1 <= hi_1)	PRINTF("< %s", v1[lo_1++]);
	if (both)		PRINTF("---\n");
	while (lo_2 <= hi_2)	PRINTF("> %s", v2[lo_2++]);
}

static
int	argv_last(
	_ARX(char **,	v)
	_ARX(int,	now)
	_AR1(int,	want)
		)
	_DCL(char **,	v)
	_DCL(int,	now)
	_DCL(int,	want)
{
	while (v[now] != 0 && now < want)
		now++;
	return (now);
}

static
SCOMP_REPORT(s_context)
{
	auto	int	both	= (lo_1 <= hi_1) && (lo_2 <= hi_2);
	auto	int	first, last;
	auto	char	mark;
	register int j;

	PRINTF("***************\n");	/* 15-'*' */

	first = lo_1 - (c_opt - 1);
	if (first < 0)	first = 0;
	last  = argv_last(v1, hi_1, hi_1 + c_opt + 1);
	mark = both ? '!' : '-';

	PRINTF("*** %d,%d\n", first, last);
	for (j = first-1; j < last; j++) {
		if (j >= 0)
		PRINTF("%c %s", (j < lo_1 || j > hi_1) ? ' ' : mark, v1[j]);
	}

	first = lo_2 - (c_opt - 1);
	if (first < 0)	first = 0;
	last  = argv_last(v2, hi_2, hi_2 + c_opt + 1);
	mark = both ? '!' : '+';

	PRINTF("\n");
	PRINTF("--- %d,%d -----\n", first, last);
	for (j = first-1; j < last; j++) {
		if (j >= 0)
		PRINTF("%c %s", (j < lo_2 || j > hi_2) ? ' ' : mark, v2[j]);
	}
}

_MAIN
{
	auto	char	buffer[BUFSIZ];
	auto	int	(*func)()	= s_report;

	setbuf(stdout, buffer);
	if ((argc > 1) && !strncmp(argv[1], "-c", 2)) {
		func = s_context;
		if (sscanf(argv[1], "-c%d", &c_opt) != 1 || (c_opt <= 0))
			c_opt = 3;
		argc--,argv++;
	}
	if (argc > 2) {
		auto	int	n1, n2;
		auto	char **	v1 = load(argv[1], &n1, "***");
		auto	char **	v2 = load(argv[2], &n2, "---");
		SCOMP((SCOMP_TYPE)v1, n1, (SCOMP_TYPE)v2, n2,
			sizeof(char *), s_match, func);
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
