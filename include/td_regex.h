/* $Id: td_regex.h,v 12.14 2017/11/22 00:27:33 tom Exp $ */

/*
 * SYSTEM5/BSD4.x differences between native regular-expression handling:
 */

#ifndef		TD_REGEX_H
#define		TD_REGEX_H

#undef REGEX_T

#ifndef TD_ERE_REGEX
#define TD_ERE_REGEX 0
#endif

#if defined(HAVE_REGEX_H_FUNCS)		/* POSIX (seen first in HP/UX, Linux) */
#  include <regex.h>
#  define REGEX_T regex_t
#  define OLD_REGEX(expr)		regfree(&expr)
#  define NEW_REGEX(expr,pattern)	(regcomp(&expr, pattern, TD_ERE_REGEX) == 0)
#  define GOT_REGEX(expr,string)	(regexec(&expr, string, 0, (regmatch_t*)0, 0) == 0)
#endif

#if defined(HAVE_REGEXPR_H_FUNCS) && !defined(REGEX_T)	/* Solaris */
#  include <regexpr.h>
#  define REGEX_T char *
#  define OLD_REGEX(expr)		free(expr)
#  define NEW_REGEX(expr,pattern)	((expr = compile(pattern, (char *)0, (char *)0)) != 0)
#  define GOT_REGEX(expr,string)	(step(string, expr) != 0)
#endif

#if defined(HAVE_REGCMP_FUNCS) && !defined(REGEX_T)	/* old SYSTEM5 */
#  if defined(HAVE_PW_H) && defined(HAVE_LIBPW)
#    undef index
#    define index CLIX_index	/* CLIX has conflict here */
#    include <pw.h>
#    undef index
#  else
#    if defined(HAVE_LIBGEN_H)		/* IRIX */
#      include <libgen.h>
#    else
	extern	char	*regcmp(char * s, ...);
	extern	char	*regex(char * re);
#    endif
#  endif
#  define REGEX_T char *
#  define OLD_REGEX(expr)		if (expr) free(expr)
#  define NEW_REGEX(expr,pattern)	((expr = regcmp(pattern, (char *)0)) != 0)
#  define GOT_REGEX(expr,string)	(regex(expr, string,  (char *)0) != 0)
#endif

#if defined(HAVE_RE_COMP_FUNCS) && !defined(REGEX_T) /* probably BSD4.x */
					/*  IRIX defines in <unistd.h> */
	extern	char	*re_comp(const char * s); /* returns 0 or error message */
	extern	int	re_exec(const char * s);  /* (return > 0): match */
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
#  define GOT_REGEX(expr,string)	(strwcmp(expr, string) == 0)
#endif

#ifndef BAD_REGEX
#  define BAD_REGEX(expr)		dedmsg(gbl, "illegal expression")
#endif

#endif	/* TD_REGEX_H */
