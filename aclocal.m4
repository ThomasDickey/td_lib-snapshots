dnl Extended Macros that test for specific features.
dnl $Header: /users/source/archives/td_lib.vcs/RCS/aclocal.m4,v 12.2 1994/05/23 23:38:41 tom Exp $
define(AC_DEV_T,
[AC_PROVIDE([$0])AC_CHECKING(for dev_t in sys/types.h)
AC_HEADER_EGREP(dev_t, sys/types.h, , AC_DEFINE(dev_t, unsigned short))])dnl
dnl
define(AC_INO_T,
[AC_PROVIDE([$0])AC_CHECKING(for ino_t in sys/types.h)
AC_HEADER_EGREP(ino_t, sys/types.h, , AC_DEFINE(ino_t, unsigned short))])dnl
dnl
define(AC_GMTOFF,
[AC_REQUIRE([AC_STRUCT_TM])ac_decl='#include <sys/types.h>
'
case "$DEFS" in
  *TM_IN_SYS_TIME*) ac_decl="$ac_decl
#include <sys/time.h>
" ;;
  *) ac_decl="$ac_decl
#include <time.h>
" ;;
esac
AC_COMPILE_CHECK([timezone declared], $ac_decl,
[long x = timezone;], AC_DEFINE(TIMEZONE_DECLARED), ac_no_timezone=1)
AC_COMPILE_CHECK(tm_gmtoff in tm, $ac_decl,
[struct tm tm; tm.tm_gmtoff;], AC_DEFINE(HAVE_TM_GMTOFF))
AC_COMPILE_CHECK(tm_zone in tm, $ac_decl,
[struct tm tm; tm.tm_zone;], AC_DEFINE(HAVE_TM_ZONE))
])dnl
