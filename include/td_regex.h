/* $Id: td_regex.h,v 12.1 1994/05/30 00:47:55 tom Exp $ */

/*
 * SYSTEM5/BSD4.x differences between native regular-expression handling:
 */

#ifndef		_TD_REGEX_H
#define		_TD_REGEX_H

#undef REGEX_T

#if HAVE_REGEX_H_FUNCS		/* HP/UX, Linux */
#  include <regex.h>
#  define REGEX_T regex_t
#  define OLD_REGEX(expr)		regfree(&expr)
#  define NEW_REGEX(expr,pattern)	(regcomp(&expr, pattern,0) == 0)
#  define GOT_REGEX(expr,string)	(regexec(&expr, string, 0, (regmatch_t*)0, 0) == 0)
#endif

#if HAVE_REGEXPR_H_FUNCS && !defined(REGEX_T)	/* Solaris */
#  include <regexpr.h>
#  define REGEX_T char *
#  define OLD_REGEX(expr)		free(&expr)
#  define NEW_REGEX(expr,pattern)	(step(pattern, expr) != 0)
#  define GOT_REGEX(expr,string)	((expr = compile(string, NULL, NULL)) != 0)
#endif

#if HAVE_REGCMP_FUNCS && !defined(REGEX_T)	/* old SYSTEM5 */
	extern	char	*regcmp(_ar1(char *,s) _CDOTS);
	extern	char	*regex(_ar1(char *,re));
#  define REGEX_T char *
#  define OLD_REGEX(expr)		if (expr) free(expr)
#  define NEW_REGEX(expr,pattern)	((expr = regcmp(pattern,0)) != 0)
#  define GOT_REGEX(expr,string)	(regex(expr, string, 0) != 0)
#endif

#if HAVE_RE_COMP_FUNCS && !defined(REGEX_T) /* probably BSD4.x */
	extern	char	*re_comp(_ar1(char *,s)); /* returns 0 or error message */
	extern	int	re_exec(_ar1(char *,s));  /* (return > 0): match */
#  define REGEX_T char *
#  define OLD_REGEX(expr)
#  define NEW_REGEX(expr,pattern)	((expr = re_comp(pattern)) == 0)
#  define GOT_REGEX(expr,string)	(re_exec(string) != 0)
#  define BAD_REGEX(expr)		dedmsg(gbl, expr)
#endif

#ifndef REGEX_T
	/* fake with with wildcard-matching */
#  define REGEX_T char *
#  define OLD_REGEX(expr)		if (expr != 0) { free(expr); expr = 0; }
#  define NEW_REGEX(expr,pattern)	((expr = stralloc(pattern)) != 0)
#  define GOT_REGEX(expr,string)	(strwcmp(expr, string) != 0)
#endif

#ifndef BAD_REGEX
#  define BAD_REGEX(expr)		dedmsg(gbl, "illegal expression")
#endif

#endif	/* _TD_REGEX_H */
