/* acconfig.h
 * $Header: /users/source/archives/td_lib.vcs/RCS/acconfig.h,v 12.2 1994/05/23 23:47:43 tom Exp $
 *
 * Extended symbols for generating configuration scripts for td_lib with
 * autoconf.
 */

/* Define to `int' if <sys/types.h> doesn't define.  */
#undef dev_t

/* Define to `int' if <sys/types.h> doesn't define.  */
#undef ino_t

/* Undef if <time.h> doesn't declare 'timezone' */
#undef TIMEZONE_DECLARED

/* Undef if struct tm doesn't have .tm_gmtoff member */
#undef HAVE_TM_GMTOFF

/* Undef if struct tm doesn't have .tm_zone member */
#undef HAVE_TM_ZONE
