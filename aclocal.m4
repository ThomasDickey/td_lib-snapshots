dnl Extended Macros that test for specific features.
dnl $Header: /users/source/archives/td_lib.vcs/RCS/aclocal.m4,v 12.47 1995/01/28 16:33:34 tom Exp $
dnl ---------------------------------------------------------------------------
dnl BELOW THIS LINE CAN BE PUT INTO "acspecific.m4", by changing "TD_" to "AC_"
dnl ---------------------------------------------------------------------------
dnl	Tests for a program given by name along the user's path, and sets a
dnl	variable to the program's directory-prefix if found.  Don't match if
dnl	the directory is ".", since we need an absolute path-reference.
define([TD_PROGRAM_PREFIX],
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
define([TD_PROGRAM_FULLPATH],
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
dnl	Adds to the include-path
dnl
dnl	Autoconf 1.11 should have provided a way to add include path options to
dnl	the cpp-tests.
dnl
define([TD_INCLUDE_PATH],
[
for p in $1
do
	if test -d $p
	then
		AC_VERBOSE(adding $p to include-path)
		INCLUDES="$INCLUDES -I$p"
		ac_cpp="${ac_cpp} -I$p"
		CFLAGS="$CFLAGS -I$p"
	fi
done
])dnl
dnl ---------------------------------------------------------------------------
dnl	Adds to the library-path
dnl
dnl	Some machines have trouble with multiple -L options.
dnl
define([TD_LIBRARY_PATH],
[
for p in $1
do
	if test -d $p
	then
		AC_VERBOSE(adding $p to library-path)
		LIBS="$LIBS -L$p"
		CFLAGS="$CFLAGS -L$p"
	fi
done
])dnl
dnl ---------------------------------------------------------------------------
dnl	Combines AC_HAVE_FUNCS logic with additional test from Kevin Buettner
dnl	that checks to see if we need a prototype for the given function.
define([TD_HAVE_FUNCS],
[
td_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS -Iinclude"
for ac_func in $1
do
changequote(,)dnl
ac_tr_func=`echo $ac_func | tr '[a-z]' '[A-Z]'`
changequote([,])dnl
AC_FUNC_CHECK(${ac_func},
AC_DEFINE(HAVE_${ac_tr_func})
AC_COMPILE_CHECK([missing "$ac_func" extern],
[#include <td_local.h>],
[
#undef $ac_func
struct zowie { int a; double b; struct zowie *c; char d; };
extern struct zowie *$ac_func(); $ac_func();
],
AC_DEFINE(NEED_${ac_tr_func})))
done
CFLAGS="$td_CFLAGS"
])dnl
dnl ---------------------------------------------------------------------------
dnl	On both Ultrix and CLIX, I find size_t defined in <stdio.h>
define([TD_SIZE_T],
[AC_CHECKING(for size_t in <sys/types.h> or <stdio.h>)
 AC_TEST_PROGRAM([
#include <sys/types.h>
#include <stdio.h>
int main() { size_t x; exit (0);}
], ,
[AC_DEFINE(size_t, unsigned)])])dnl
dnl ---------------------------------------------------------------------------
define([TD_DEV_T],
[AC_PROVIDE([$0])AC_CHECKING(for dev_t in sys/types.h)
AC_HEADER_EGREP(dev_t, sys/types.h, , AC_DEFINE(dev_t, unsigned short))])dnl
dnl ---------------------------------------------------------------------------
define([TD_INO_T],
[AC_PROVIDE([$0])AC_CHECKING(for ino_t in sys/types.h)
AC_HEADER_EGREP(ino_t, sys/types.h, , AC_DEFINE(ino_t, unsigned short))])dnl
dnl ---------------------------------------------------------------------------
define([TD_MODE_T],
[AC_PROVIDE([$0])AC_CHECKING(for mode_t in sys/types.h)
AC_HEADER_EGREP(mode_t, sys/types.h, , AC_DEFINE(mode_t, int))])dnl
dnl ---------------------------------------------------------------------------
dnl Test if "##" is substituted properly, or failing that, if /**/ can do
dnl the trick.
define([TD_ANSI_CPP],
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
define([TD_REGEX_H_FUNCS],
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
define([TD_REGEXPR_H_FUNCS],
[save_libs="$LIBS"
AC_HAVE_LIBRARY(gen, [LIBS="$LIBS -lgen"])
AC_TEST_PROGRAM([
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
], [AC_DEFINE(HAVE_REGEXPR_H_FUNCS)], [LIBS="$save_libs"])
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for the presence of regcmp/regex functions (no include-file?)
dnl Some systems (CLIX) use <pw.h> for this purpose.  CLIX requires the -lPW,
dnl but HPUX has only a broken version of that library, so we've got to
dnl try the compile with/without the library.
define([TD_REGCMP_LIBS],
[AC_TEST_PROGRAM([
#include <stdio.h>	/* need this for CLIX to define '__' macro */
#if HAVE_PW_H
#include <pw.h>		/* _must_ use prototype on CLIX */
#endif
#if HAVE_LIBGEN_H
#include <libgen.h>	/* IRIX defines it here */
#endif
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
], [td_have_regcmp_funcs=yes],
   [td_have_regcmp_funcs=no])
])dnl
dnl ---------------------------------------------------------------------------
dnl Test for the presence of regcmp/regex functions.
define([TD_REGCMP_FUNCS],
[
AC_HAVE_HEADERS(pw.h libgen.h)
TD_REGCMP_LIBS
save_libs="$LIBS"
if test $td_have_regcmp_funcs = no; then
	AC_HAVE_LIBRARY(PW)
	if test -n "${ac_have_lib}"; then
		TD_REGCMP_LIBS
	fi
fi
if test $td_have_regcmp_funcs = no; then
	LIBS="$save_libs"
else
	AC_DEFINE(HAVE_REGCMP_FUNCS)
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for the presence of re_comp/re_exec functions (no include-file?)
define([TD_RE_COMP_FUNCS],
[AC_TEST_PROGRAM([
int main() {
	extern char *re_comp();
	char *e;
	char *p = "foo";
	char *s = "foobar";
	if ((e = re_comp(p)) != 0
	 || re_exec(s) <= 0)
	 	exit(1);
	exit(0);
}
], [AC_DEFINE(HAVE_RE_COMP_FUNCS)])
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for the ensemble of include-files and functions that make up the
dnl host's regular expression parsing.
define([TD_REGEX],
[AC_CHECKING(for regular-expression library support)
TD_REGEX_H_FUNCS
TD_REGEXPR_H_FUNCS
TD_REGCMP_FUNCS
TD_RE_COMP_FUNCS
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for the ensemble of programs that are used in RCS, SCCS, VCS, CVS.
define([TD_RCS_SCCS],
[AC_CHECKING(for SCCS/RCS programs)
TD_PROGRAM_PREFIX(RCS_PATH, rcs)
TD_PROGRAM_PREFIX(SCCS_PATH, admin)dnl the SCCS tool
TD_PROGRAM_PREFIX(VCS_PATH, vcs)dnl VCS is my RCS application
TD_PROGRAM_PREFIX(CVS_PATH, cvs)dnl CVS is a layer above RCS
TD_PROGRAM_PREFIX(CMV_PATH, cmv)dnl CmVision combines RCS and SCCS archives
])dnl
dnl ---------------------------------------------------------------------------
define([TD_GMTOFF],
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
AC_COMPILE_CHECK([localzone declared], $ac_decl,
[long x = localzone;], AC_DEFINE(LOCALZONE_DECLARED))
AC_COMPILE_CHECK([timezone declared], $ac_decl,
[long x = timezone;], AC_DEFINE(TIMEZONE_DECLARED))
AC_COMPILE_CHECK(tm_gmtoff in tm, $ac_decl,
[struct tm tm; tm.tm_gmtoff;], AC_DEFINE(HAVE_TM_GMTOFF))
AC_COMPILE_CHECK(tm_zone in tm, $ac_decl,
[struct tm tm; tm.tm_isdst;], AC_DEFINE(HAVE_TM_ISDST))
])dnl
dnl ---------------------------------------------------------------------------
dnl Test if curses defines 'addchnstr()' (maybe a macro or function)
define([TD_CURSES_ADDCHNSTR],
[AC_COMPILE_CHECK([function/macro addchnstr],[#include <curses.h>
], [addchnstr(0,0)], [AC_DEFINE(HAVE_ADDCHNSTR)])
])dnl
dnl ---------------------------------------------------------------------------
dnl Test if curses defines 'chtype' (usually a 16-bit type for SysV curses).
define([TD_CURSES_CHTYPE],
[AC_CHECKING(chtype typedef)
AC_TEST_PROGRAM([#include <curses.h>
int main() { chtype foo; exit(0); }
], [AC_DEFINE(HAVE_TYPE_CHTYPE)])
])dnl
dnl ---------------------------------------------------------------------------
dnl Test if curses defines 'cbreak()' (maybe a macro or function)
define([TD_CURSES_CBREAK],
[AC_COMPILE_CHECK([function/macro cbreak],["#include <curses.h>
"], [cbreak()], [AC_DEFINE(HAVE_CBREAK)])
])dnl
dnl ---------------------------------------------------------------------------
dnl Test if curses defines 'erasechar()' (maybe a macro or function)
define([TD_CURSES_ERASECHAR],
[AC_CHECKING(function/macro erasechar)
AC_TEST_PROGRAM([#include <curses.h>
int main() { int foo = erasechar(); exit(0); }
], [AC_DEFINE(HAVE_ERASECHAR)])
])dnl
dnl ---------------------------------------------------------------------------
dnl Test if curses defines 'killchar()' (maybe a macro or function)
define([TD_CURSES_KILLCHAR],
[AC_CHECKING(function/macro killchar)
AC_TEST_PROGRAM([#include <curses.h>
int main() { int foo = killchar(); exit(0); }
], [AC_DEFINE(HAVE_KILLCHAR)])
])dnl
dnl ---------------------------------------------------------------------------
dnl Test if curses defines 'struct screen'.
dnl
dnl	If this isn't defined, we cannot build a lint library that will check
dnl	for that type, since it isn't resolved.
dnl
define([TD_STRUCT_SCREEN],
[AC_COMPILE_CHECK([definition of struct-screen/macro],["#include <curses.h>
"], [struct screen { int dummy;}], [AC_DEFINE(NEED_STRUCT_SCREEN)])
])dnl
dnl ---------------------------------------------------------------------------
dnl Test if curses defines KD, KU, etc., for cursor keys
dnl
dnl	I found at least one implementation of curses that didn't declare these
dnl	variables in the include-file, but did define them in the library.  If
dnl	they're not in the include-file, ignore them.  Otherwise, assume that
dnl	curses initializes them in 'initscr()'.
dnl
define([TD_TCAP_CURSOR],
[AC_CHECKING([termcap-cursor variables])
AC_TEST_PROGRAM([#include <curses.h>
int main() { char *d=KD, *u=KU, *r=KR, *l=KL; exit(0); }
], [AC_DEFINE(HAVE_TCAP_CURSOR)])
])dnl
dnl ---------------------------------------------------------------------------
dnl Test for interesting things about curses functions/datatypes
define([TD_CURSES_FUNCS],
[TD_CURSES_ADDCHNSTR
TD_CURSES_CHTYPE
TD_CURSES_CBREAK
TD_CURSES_ERASECHAR
TD_CURSES_KILLCHAR
TD_STRUCT_SCREEN
TD_TCAP_CURSOR
])dnl
dnl ---------------------------------------------------------------------------
dnl Test for the varieties of curses libraries.
dnl
dnl	There are two main varieties of curses: BSD and SYSV.  The latter has
dnl	more features (e.g., keypad and function key support, auxiliary
dnl	character set).  The easiest thing to test for is the presence of
dnl	'keypad()' in the curses library.  BSD curses hasn't got it.
dnl
dnl	There's a free version of SYSV curses called 'ncurses'.  If we've got
dnl	that, configure for it instead of the BSD curses.
dnl
dnl	On SunOS, we may have both BSD and SYS5 curses (the latter under /5lib
dnl	and /5include).
dnl
define([TD_CURSES_LIBS],
[
AC_PROVIDE([$0])
td_save_LIBS="${LIBS}"
dnl:if test -d /usr/5lib -a -d /usr/5include
dnl:then
dnl:	TD_INCLUDE_PATH(/usr/5include)
dnl:	TD_LIBRARY_PATH(/usr/5lib)
dnl:fi
AC_HAVE_LIBRARY(curses)
td_have_keypad=yes
case "$DEFS" in
*HAVE_LIBCURSES*)
	td_decl="#include <curses.h>
"
	AC_COMPILE_CHECK([BSD vs SYSV curses], $td_decl,
	[keypad(curscr,1)],,
	td_have_keypad=no)
	;;
esac
AC_WITH(ncurses,
[
if test $td_have_keypad = no
then
	td_save2LIBS="${LIBS}"
	LIBS="${td_save_LIBS}"
	AC_HAVE_LIBRARY(ncurses)
	case "$DEFS" in
	*HAVE_LIBNCURSES*)
		# Linux installs NCURSES's include files in a separate
		# directory to avoid confusion with the native curses.  Some
		# versions have <ncurses.h>, while newer ones have it renamed
		# to <curses.h> -- in either case it's linked, but we use the
		# definition in <td_curse.h>
		if test -d /usr/include/ncurses
		then
			TD_INCLUDE_PATH(/usr/include/ncurses)
			AC_DEFINE(HAVE_NCURSES_H)
		else
			AC_HAVE_HEADERS(ncurses.h)
		fi
		;;
	*)
		LIBS="${td_save2LIBS}"
		AC_HAVE_LIBRARY(termcap)
		;;
	esac
fi
],[AC_HAVE_LIBRARY(termcap)])
])dnl
dnl ---------------------------------------------------------------------------
dnl Test for non-Posix prototype for 'signal()'
dnl
dnl	Apollo sr10.x defines prototypes for the signal handling function that
dnl	have an extra argument (in comparison with Standard C).  Also, on the
dnl	systems that have standard prototypes, the predefined actions often
dnl	have incomplete prototypes.  This macro tries to test for these cases
dnl	so that when compiling I don't see unnecessary warning messages.
dnl
dnl	(If the compiler doesn't recognize prototypes, of course, this test
dnl	will not find anything :-)
dnl
define([TD_SIG_ARGS],
[AC_REQUIRE([AC_RETSIGTYPE])
AC_CHECKING([non-standard signal handler prototype])
AC_TEST_PROGRAM([
#include <signal.h>
RETSIGTYPE (*signal(int sig, RETSIGTYPE(*func)(int sig)))(int sig2);
RETSIGTYPE catch(int sig) { exit(1); }
main() { signal(SIGINT, catch); exit(0); }
],[AC_DEFINE(SIG_ARGS_STANDARD)],[AC_TEST_PROGRAM([
#include <signal.h>
RETSIGTYPE (*signal(int sig, RETSIGTYPE(*func)(int sig,...)))(int sig2,...);
RETSIGTYPE catch(int sig, ...) { exit(1); }
main() { signal(SIGINT, catch); exit(0); }
],[AC_DEFINE(SIG_ARGS_VARYING)])]
)
if test -n "$GCC"
then
AC_WITH(warnings,
	[
AC_CHECKING([redefinable signal handler prototype])
	# We're checking the definitions of the commonly-used predefined signal
	# macros, to see if their values are the ones that we expect.  If so,
	# we'll plug in our own definitions, that have complete prototypes.  We
	# do this when we're developing with gcc, with all warnings, and
	# shouldn't do it for other compilers, since (for example) the IRIX
	# compiler complains a lot.
		case "$DEFS" in
		*SIG_ARGS_*) # we have prototypes
AC_TEST_PROGRAM([
#include <signal.h>
#undef  NOT
#define NOT(s,d) ((long)(s) != (long)(d))
main() { exit(NOT(SIG_IGN,1) || NOT(SIG_DFL,0) || NOT(SIG_ERR,-1)); }
],[AC_DEFINE(SIG_IGN_REDEFINEABLE)])
		;;
		esac
	])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl Test for the presence of <sys/wait.h>, 'union wait', arg-type of 'wait()'.
dnl
dnl	FIXME: These tests should have been in autoconf 1.11!
dnl
dnl	Note that we cannot simply grep for 'union wait' in the wait.h file,
dnl	because some Posix systems turn this on only when a BSD variable is
dnl	defined. Since I'm trying to do without special defines, I'll live
dnl	with the default behavior of the include-file.
dnl
dnl	I do _2_ compile checks, because we may have union-wait, but the
dnl	prototype for 'wait()' may want an int.
dnl
dnl	Don't use HAVE_UNION_WAIT, because the autoconf documentation implies
dnl	that if we've got union-wait, we'll automatically use it.
dnl
define([TD_WAIT],
[AC_CHECKING(wait include/argument type)
AC_HAVE_HEADERS(wait.h)
AC_HAVE_HEADERS(sys/wait.h)td_decl='#include <sys/types.h>
'
case "$DEFS" in
*_SYS_WAIT_H*)td_decl="$td_decl
#include <sys/wait.h>
" ;;
*_WAIT_H*)td_decl="$td_decl
#include <wait.h>
" ;;
esac
AC_COMPILE_CHECK([union wait declared], $td_decl,
[union wait x],
[AC_COMPILE_CHECK([union wait used as wait-arg], $td_decl,
 [union wait x; wait(&x)],
 [AC_DEFINE(WAIT_USES_UNION)])
 ])
])dnl
dnl ---------------------------------------------------------------------------
dnl Test if the <sys/stat.h> 'stat' struct defines 'st_blocks' member.
dnl If not, assume it's some non-BSD system.
define([TD_STAT_ST_BLOCKS],
[AC_CHECKING(for .st_blocks in struct stat)
 AC_TEST_PROGRAM([
#include <sys/types.h>
#include <sys/stat.h>
int main() {exit(0);}
int t() {struct stat sb; return sb.st_blocks; }
],
[AC_DEFINE(STAT_HAS_ST_BLOCKS)])
])dnl
dnl ---------------------------------------------------------------------------
dnl Test for the presence of the 'sys_errlist[]' array if we don't have the
dnl 'strerror()' function.
define([TD_SYS_ERRLIST],
[AC_CHECKING(for sys_errlist in <errno.h>)
 AC_TEST_PROGRAM([
#include <stdio.h>
#include <errno.h>
extern char *sys_errlist[];
extern int sys_nerr;
int main() { char *x = sys_errlist[sys_nerr-1]; exit (x==0);}
],
[AC_DEFINE(HAVE_SYS_ERRLIST)])
])dnl
