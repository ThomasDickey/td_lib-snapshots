dnl Extended Macros that test for specific features.
dnl $Header: /users/source/archives/td_lib.vcs/RCS/aclocal.m4,v 12.12 1994/05/30 23:11:09 tom Exp $
dnl ---------------------------------------------------------------------------
dnl BELOW THIS LINE CAN BE PUT INTO "acspecific.m4"
dnl ---------------------------------------------------------------------------
dnl Tests for a program given by name along the user's path, and sets a variable
dnl to the program's directory-prefix if found.  Don't match if the directory is
dnl ".", since we need an absolute path-reference.
define([AC_PROGRAM_PREFIX],
[if test -z "[$]$1"; then
  # Extract the first word of `$2', so it can be a program name with args.
  set ac_dummy $2; ac_word=[$]2
  AC_CHECKING([for $ac_word])
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:"
  for ac_dir in $PATH; do
    test -z "$ac_dir" && ac_dir=.
    if test "$ac_dir" != "." -a -f $ac_dir/$ac_word -a -x $ac_dir/$ac_word; then
      $1="$ac_dir"
      break
    fi
  done
  IFS="$ac_save_ifs"
fi
ifelse([$3],,, [test -z "[$]$1" && $1="$3"])
test -n "[$]$1" && AC_DEFINE_UNQUOTED($1,[\"$]$1[\"])
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for one or more programs given by name along the user's path, and
dnl sets a variable to the program's full-path if found.
define([AC_PROGRAM_FULLPATH],
[if test -z "[$]$1"; then
  set -- $2;
  while test [$]# != 0
  do
    # autoconf gets confused when I add this to the while-statement
    if test -n "[$]$1"; then
      break
    fi
    ac_word=[$]1
    AC_CHECKING([for "$ac_word"])
    case [$]1 in
    -*) ;;
    *)
      if test -f "$ac_word" -a ! -f "./$ac_word" -a -x "$ac_word"; then
        $1="$ac_word"
      else
        IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:"
        for ac_dir in $PATH; do
          test -z "$ac_dir" && ac_dir=.
          if test "$ac_dir" != "." -a -f $ac_dir/$ac_word -a -x $ac_dir/$ac_word; then
            $1="$ac_dir/$ac_word"
            break
          fi
        done
        IFS="$ac_save_ifs"
      fi
      ;;
    esac
    shift
  done
fi
# append options, if any
if test -n "[$]$1"; then
  while test [$]# != 0
  do
    case [$]1 in
    -[*]) $1="[$]$1 [$]1";;
    [*])  set -- end;;
    esac
    shift
  done
  AC_DEFINE_UNQUOTED($1,[\"$]$1[\"])
fi
])dnl
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
[AC_CHECKING(for ANSI CPP token-splicing/quoting)
AC_TEST_PROGRAM([
#define cat(a,b) a##b
int main() { cat(lo,ng) x; char *y = "a" "b"; exit (0);}
],
[AC_DEFINE(HAVE_NEW_TOKEN_SPLICE)])
AC_TEST_PROGRAM([
#define quote(name) #name
int main() { char *y = quote(a); exit (*y != 'a');}
],
[AC_DEFINE(HAVE_NEW_TOKEN_QUOTE)])
AC_TEST_PROGRAM([
#define cat(a,b) a/**/b
int main() { cat(lo,ng) x; exit (0);}
],
[AC_DEFINE(HAVE_OLD_TOKEN_SPLICE)])
AC_TEST_PROGRAM([
#define quote(name) "name"
int main() { char *y = quote(a); exit (*y != 'a');}
],
[AC_DEFINE(HAVE_OLD_TOKEN_QUOTE)])
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for the <regex.h> include-file, and the functions associated with it.
define([AC_REGEX_H_FUNCS],
[AC_TEST_PROGRAM([
#include <sys/types.h>
#include <regex.h>
int main() {
	regex_t e;
	char *p = "foo";
	char *s = "foobar";
	if (regcomp(&e, p, 0) != 0
	 || regexec(&e, s, 0, (regmatch_t*)0, 0) < 0)
	 	exit(1);
	regfree(&e);
	exit(0);
}
], [AC_DEFINE(HAVE_REGEX_H_FUNCS)])
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for the <regexpr.h> include-file, and the functions associated with it.
define([AC_REGEXPR_H_FUNCS],
[AC_TEST_PROGRAM([
#include <sys/types.h>
#include <regexpr.h>
int main() {
	char *e;
	char *p = "foo";
	char *s = "foobar";
	if ((e = compile(p, NULL, NULL)) == 0
	 || step(s, e) == 0)
	 	exit(1);
	free(e);
	exit(0);
}
], [AC_DEFINE(HAVE_REGEXPR_H_FUNCS)])
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for the presence of regcmp/regex functions (no include-file?)
define([AC_REGCMP_FUNCS],
[AC_TEST_PROGRAM([
int main() {
	char *e;
	char *p = "foo";
	char *s = "foobar";
	if ((e = regcmp(p, 0)) == 0
	 || regex(e, s, 0) == 0)
	 	exit(1);
	free(e);
	exit(0);
}
], [AC_DEFINE(HAVE_REGCMP_FUNCS)])
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for the presence of re_comp/re_exec functions (no include-file?)
define([AC_RE_COMP_FUNCS],
[AC_TEST_PROGRAM([
int main() {
	char *p = "foo";
	char *s = "foobar";
	if ((e = re_comp(p, 0)) != 0
	 || regex(s) == 0)
	 	exit(1);
	exit(0);
}
], [AC_DEFINE(HAVE_RE_COMP_FUNCS)])
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for the ensemble of include-files and functions that make up the
dnl host's regular expression parsing.
define([AC_REGEX],
[AC_CHECKING(for regular-expression library support)
AC_REGEX_H_FUNCS
AC_REGEXPR_H_FUNCS
AC_REGCMP_FUNCS
AC_RE_COMP_FUNCS
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for the ensemble of programs that are used in RCS, SCCS, VCS, CVS.
define([AC_RCS_SCCS],
[AC_CHECKING(for SCCS/RCS programs)
AC_PROGRAM_PREFIX(RCS_PATH, rcs)
AC_PROGRAM_PREFIX(SCCS_PATH, admin)
AC_PROGRAM_PREFIX(VCS_PATH, vcs)
AC_PROGRAM_PREFIX(CVS_PATH, cvs)
])dnl
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
dnl ---------------------------------------------------------------------------
dnl Test if curses defines 'chtype' (usually a 16-bit type for SysV curses).
define([AC_CURSES_CHTYPE],
[AC_CHECKING(chtype typedef)
AC_TEST_PROGRAM([#include <curses.h>
int main() { chtype foo; exit(0); }
], [AC_DEFINE(HAVE_TYPE_CHTYPE)])
])dnl
dnl ---------------------------------------------------------------------------
dnl Test if curses defines 'erasechar()' (maybe a macro or function)
define([AC_CURSES_ERASECHAR],
[AC_CHECKING(function/macro erasechar)
AC_TEST_PROGRAM([#include <curses.h>
int main() { int foo = erasechar(); exit(0); }
], [AC_DEFINE(HAVE_ERASECHAR)])
])dnl
dnl ---------------------------------------------------------------------------
dnl Test if curses defines 'killchar()' (maybe a macro or function)
define([AC_CURSES_KILLCHAR],
[AC_CHECKING(function/macro killchar)
AC_TEST_PROGRAM([#include <curses.h>
int main() { int foo = killchar(); exit(0); }
], [AC_DEFINE(HAVE_KILLCHAR)])
])dnl
dnl ---------------------------------------------------------------------------
dnl Test for interesting things about curses
define([AC_CURSES],
[AC_HAVE_LIBRARY(curses)
AC_HAVE_FUNCS(beep)
AC_HAVE_FUNCS(keypad)
AC_CURSES_CHTYPE
AC_CURSES_ERASECHAR
AC_CURSES_KILLCHAR
])dnl
