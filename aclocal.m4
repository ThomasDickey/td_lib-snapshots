dnl Extended Macros that test for specific features.
dnl $Header: /users/source/archives/td_lib.vcs/RCS/aclocal.m4,v 12.1 1994/05/22 23:10:57 tom Exp $
define(AC_DEV_T,
[AC_PROVIDE([$0])AC_CHECKING(for dev_t in sys/types.h)
AC_HEADER_EGREP(dev_t, sys/types.h, , AC_DEFINE(dev_t, unsigned short))])dnl
dnl
define(AC_INO_T,
[AC_PROVIDE([$0])AC_CHECKING(for ino_t in sys/types.h)
AC_HEADER_EGREP(ino_t, sys/types.h, , AC_DEFINE(ino_t, unsigned short))])dnl
dnl
