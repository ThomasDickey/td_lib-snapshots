/* acconfig.h
 * $Header: /users/source/archives/td_lib.vcs/RCS/acconfig.h,v 12.5 1994/05/29 00:50:54 tom Exp $
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

/* Define if preprocessor does "/**/" concatenation */
#undef HAVE_OLD_TOKEN_SPLICE

/* Define if preprocessor uses leading '#' to change macro arg to quotes */
#undef HAVE_NEW_TOKEN_QUOTE

/* Define if preprocessor substitutes macro arguments inside quotes */
#undef HAVE_OLD_TOKEN_QUOTE

/* Define if we have <regex.h> with regexec/regcomp/regfree interface */
#undef HAVE_REGEX_H_FUNCS

/* Define if <time.h> does declare 'timezone' */
#undef TIMEZONE_DECLARED

/* Define if struct tm has .tm_gmtoff member */
#undef HAVE_TM_GMTOFF

/* Define if struct tm has .tm_isdst member */
#undef HAVE_TM_ISDST

/* end of local acconfig.h */
