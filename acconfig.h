/* acconfig.h
 * $Header: /users/source/archives/td_lib.vcs/RCS/acconfig.h,v 12.3 1994/05/28 00:43:54 tom Exp $
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

/* Define if <time.h> does declare 'timezone' */
#undef TIMEZONE_DECLARED

/* Define if struct tm has .tm_gmtoff member */
#undef HAVE_TM_GMTOFF

/* Define if struct tm has .tm_isdst member */
#undef HAVE_TM_ISDST

/* end of local acconfig.h */
