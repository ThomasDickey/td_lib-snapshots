dnl Extended Macros that test for specific features.
dnl $Header: /users/source/archives/td_lib.vcs/RCS/aclocal.m4,v 12.6 1994/05/28 23:43:46 tom Exp $
dnl ---------------------------------------------------------------------------
dnl BELOW THIS LINE CAN BE PUT INTO "acspecific.m4"
dnl ---------------------------------------------------------------------------
define([AC_DEV_T],
[AC_PROVIDE([$0])AC_CHECKING(for dev_t in sys/types.h)
AC_HEADER_EGREP(dev_t, sys/types.h, , AC_DEFINE(dev_t, unsigned short))])dnl
dnl ---------------------------------------------------------------------------
define([AC_INO_T],
[AC_PROVIDE([$0])AC_CHECKING(for ino_t in sys/types.h)
AC_HEADER_EGREP(ino_t, sys/types.h, , AC_DEFINE(ino_t, unsigned short))])dnl
dnl ---------------------------------------------------------------------------
dnl Test if "##" is substituted properly, or failing that, if /**/ can do
dnl the trick.
define([AC_ANSI_CPP],
[AC_CHECKING(for ANSI CPP token-splicing)
AC_TEST_PROGRAM([
#define cat(a,b) a##b
int main() { cat(lo,ng) x; char *y = "a" "b"; exit (0);}
],
[AC_DEFINE(HAVE_NEW_TOKEN_SPLICE) ansi_cpp=1], ansi_cpp=0)
if test $ansi_cpp = 0; then
AC_TEST_PROGRAM([
#define cat(a,b) a/**/b
int main() { cat(lo,ng) x; exit (0);}
],
[AC_DEFINE(HAVE_OLD_TOKEN_SPLICE)])
fi
])
dnl ---------------------------------------------------------------------------
define([AC_GMTOFF],
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
[struct tm tm; tm.tm_isdst;], AC_DEFINE(HAVE_TM_ISDST))
])dnl
