/* acconfig.h
 * $Header: /users/source/archives/td_lib.vcs/RCS/acconfig.h,v 12.9 1994/05/30 23:16:01 tom Exp $
 *
 * Extended symbols for generating configuration scripts for td_lib with
 * autoconf.
 */

/* Define to `int' if <sys/types.h> doesn't define.  */
#undef dev_t

/* Define to `int' if <sys/types.h> doesn't define.  */
#undef ino_t

/* Define if preprocessor does "##" concatenation */
#undef HAVE_NEW_TOKEN_SPLICE

/* Define if preprocessor does comment-splicing concatenation */
#undef HAVE_OLD_TOKEN_SPLICE

/* Define if preprocessor uses leading '#' to change macro arg to quotes */
#undef HAVE_NEW_TOKEN_QUOTE

/* Define if preprocessor substitutes macro arguments inside quotes */
#undef HAVE_OLD_TOKEN_QUOTE

/* Define if we have <regex.h> with regexec/regcomp/regfree interface */
#undef HAVE_REGEX_H_FUNCS

/* Define if we have <regexpr.h> with compile/step interface */
#undef HAVE_REGEXPR_H_FUNCS

/* Define if we have regcmp/regex interface */
#undef HAVE_REGCMP_FUNCS

/* Define if we have re_comp/re_exec interface */
#undef HAVE_RE_COMP_FUNCS

/* Define if <time.h> does declare 'timezone' */
#undef TIMEZONE_DECLARED

/* Define if struct tm has .tm_gmtoff member */
#undef HAVE_TM_GMTOFF

/* Define if struct tm has .tm_isdst member */
#undef HAVE_TM_ISDST

/* Define if curses defines the function/macro 'erasechar()' */
#undef HAVE_ERASECHAR

/* Define if curses defines the function/macro 'killchar()' */
#undef HAVE_KILLCHAR

/* Define if curses defines the type 'chtype' */
#undef HAVE_TYPE_CHTYPE

/* Define to path of "vi" */
#undef DEFAULT_EDITOR

/* Define to path of "view" */
#undef DEFAULT_BROWSE

/* Define to path of "more" */
#undef DEFAULT_PAGER

/* Define if we find "rcs" in our path */
#undef RCS_PATH

/* Define if we find "admin" in our path */
#undef SCCS_PATH

/* Define if we find "vcs" in our path */
#undef VCS_PATH

/* Define if we find "cvs" in our path */
#undef CVS_PATH

/* end of local acconfig.h */
