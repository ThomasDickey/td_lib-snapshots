dnl Extended Macros that test for specific features.
dnl $Header: /users/source/archives/td_lib.vcs/RCS/aclocal.m4,v 12.75 1996/12/27 20:13:14 tom Exp $
dnl vi:set ts=4:
dnl ---------------------------------------------------------------------------
dnl BELOW THIS LINE CAN BE PUT INTO "acspecific.m4", without change
dnl ---------------------------------------------------------------------------
dnl A conventional existence-check for 'lstat' won't work with the Linux
dnl version of gcc 2.7.0, since the symbol is defined only within <sys/stat.h>
dnl as an inline function.
dnl
dnl So much for portability.
AC_DEFUN([AC_FUNC_LSTAT],
[
AC_MSG_CHECKING(for lstat)
AC_CACHE_VAL(ac_cv_func_lstat,[
AC_TRY_LINK([
#include <sys/types.h>
#include <sys/stat.h>],
	[lstat(".", (struct stat *)0)],
	[ac_cv_func_lstat=yes],
	[ac_cv_func_lstat=no])
	])
AC_MSG_RESULT($ac_cv_func_lstat )
if test $ac_cv_func_lstat = yes; then
	AC_DEFINE(HAVE_LSTAT)
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl BELOW THIS LINE CAN BE PUT INTO "acspecific.m4", by changing "TD_" to "AC_"
dnl and "td_" to "ac_".
dnl ---------------------------------------------------------------------------
dnl Test if "##" is substituted properly, or failing that, if /**/ can do
dnl the trick.  We can test concatenation with a compile, but quoting has to
dnl be tested by running a program.
AC_DEFUN([TD_ANSI_CPP],
[
AC_MSG_CHECKING(for ANSI CPP token-splicing)
AC_CACHE_VAL(td_cv_ansi_splice,[
	td_cv_ansi_splice=unknown
	AC_TRY_COMPILE(
		[#define cat(a,b) a##b],
		[cat(lo,ng) x; char *y = "a" "b"],
		[td_cv_ansi_splice=new],
		[AC_TRY_COMPILE(
			[#define cat(a,b) a/**/b],
			[cat(lo,ng) x],
			[td_cv_ansi_splice=old])
		])
	])

AC_MSG_RESULT(${td_cv_ansi_splice}-style)
if test $td_cv_ansi_splice = new; then
	AC_DEFINE(HAVE_NEW_TOKEN_SPLICE)
elif test $td_cv_ansi_splice = old; then
	AC_DEFINE(HAVE_OLD_TOKEN_SPLICE)
fi

AC_MSG_CHECKING(for ANSI CPP token-quoting)
AC_CACHE_VAL(td_cv_ansi_quote,[
	AC_TRY_RUN([#define quote(name) #name
		int main() { char *y = quote(a); exit (*y != 'a');} ],
		[td_cv_ansi_quote=new],
		[td_cv_ansi_quote=unknown],
		[td_cv_ansi_quote=unknown])
	if test $td_cv_ansi_quote = unknown; then
		AC_TRY_RUN([#define quote(name) "name"
			int main() { char *y = quote(a); exit (*y != 'a');} ],
			[td_cv_ansi_quote=old],
			[td_cv_ansi_quote=unknown],
			[td_cv_ansi_quote=unknown])
		fi
	])

AC_MSG_RESULT(${td_cv_ansi_quote}-style)
if test $td_cv_ansi_quote = new; then
	AC_DEFINE(HAVE_NEW_TOKEN_QUOTE)
elif test $td_cv_ansi_quote = old; then
	AC_DEFINE(HAVE_OLD_TOKEN_QUOTE)
fi

])dnl
dnl ---------------------------------------------------------------------------
dnl Test if curses defines 'chtype' (usually a 16-bit type for SysV curses).
AC_DEFUN([TD_CURSES_CHTYPE],
[
AC_MSG_CHECKING(for chtype typedef)
AC_CACHE_VAL(td_cv_chtype_decl,[
	AC_TRY_COMPILE([#include <curses.h>],
		[chtype foo],
		[td_cv_chtype_decl=yes],
		[td_cv_chtype_decl=no])])
AC_MSG_RESULT($td_cv_chtype_decl)
test $td_cv_chtype_decl = yes && AC_DEFINE(HAVE_TYPE_CHTYPE)
])dnl
dnl ---------------------------------------------------------------------------
dnl Test for curses data/types
AC_DEFUN([TD_CURSES_DATA],
[
TD_CURSES_CHTYPE
TD_CURSES_STYLE
TD_STRUCT_SCREEN
TD_TCAP_CURSOR
])dnl
dnl ---------------------------------------------------------------------------
dnl Curses-functions are a little complicated, since a lot of them are macros.
AC_DEFUN([TD_CURSES_FUNCS],
[
for td_func in $1
do
	TD_UPPERCASE($td_func,td_tr_func)
	AC_MSG_CHECKING(for ${td_func})
	TD_MSG_LOG(${td_func})
	AC_CACHE_VAL(td_cv_func_$td_func,[
		eval td_result='$ac_cv_func_'$td_func
		if test ".$td_result" != ".no"; then
			AC_TRY_LINK([#include <curses.h>],
			[
#ifndef ${td_func}
long foo = (long)(&${td_func});
#endif
			],
			[td_result=yes],
			[td_result=no])
		fi
		eval 'td_cv_func_'$td_func'=$td_result'
	])
	# use the computed/retrieved cache-value:
	eval 'td_result=$td_cv_func_'$td_func
	AC_MSG_RESULT($td_result)
	if test $td_result != no; then
		AC_DEFINE_UNQUOTED(HAVE_${td_tr_func})
	fi
done
])dnl
dnl ---------------------------------------------------------------------------
dnl Test for the existence of SysVr4 mouse support in curses. If we've not got
dnl it, we'll simulate the interface (for xterm, at least).
AC_DEFUN([TD_CURSES_MOUSE],
[
AC_MSG_CHECKING(for curses mouse-support)
AC_CACHE_VAL(td_cv_curses_mouse,[
AC_TRY_LINK([#include <curses.h>],[
	int x, y;
	getmouse();
	request_mouse_pos();
	wmouse_position(stdscr, &x, &y);
	mouse_on(TRUE);
	mouse_off(FALSE);
	mouse_set(0);],
	[td_cv_curses_mouse=yes],
	[td_cv_curses_mouse=no])
])
AC_MSG_RESULT($td_cv_curses_mouse)
test $td_cv_curses_mouse = yes && AC_DEFINE(CURSES_HAS_MOUSE)
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
AC_DEFUN([TD_CURSES_LIBS],
[
AC_PROVIDE([$0])
if test $WithNcurses = yes; then
	TD_MSG_LOG(ncurses library, by option)
	AC_CHECK_LIB(ncurses,initscr)
else
	# Check if we've already got curses functions in our library list
	# (e.g., if the user is overriding this test by setting $LIBS).
	TD_MSG_LOG(curses library, by default)
	AC_TRY_LINK([#include <curses.h>],
		[initscr()],
		[case "$LIBS" in
		 *libncurses.*|*-lncurses*)
		 	ac_cv_lib_ncurses_initscr=predefined
		 	ac_cv_lib_curses_initscr=no
		 	;;
		 *)
		 	ac_cv_lib_ncurses_initscr=no
		 	ac_cv_lib_curses_initscr=yes
		 	;;
		 esac],
		[ac_cv_lib_ncurses_initscr=unknown])
fi
TD_MSG_LOG(ncurses state $ac_cv_lib_ncurses_initscr)
if test $ac_cv_lib_ncurses_initscr = yes; then
	# Linux installs NCURSES's include files in a separate directory to avoid
	# confusion with the native curses.  NCURSES has its own termcap support.
	TD_INCLUDE_PATH(/usr/include/ncurses /usr/local/include/ncurses)
	ac_cv_lib_curses_initscr=yes
elif test $ac_cv_lib_ncurses_initscr = unknown; then
	# The curses library often depends on the termcap library, so we've checked
	# for it first.  We could make a more complicated test to ensure that we
	# don't add the termcap library, but some functions use it anyway if it's
	# there.
	AC_CHECK_LIB(termcap, tgetent)
	AC_CHECK_LIB(curses, initscr)
fi
# The main distinction between bsd- and sysv-curses is that the latter has a
# keypad function.
AC_MSG_CHECKING(BSD vs SYSV curses)
TD_MSG_LOG(curses-type BSD/SYSV)
AC_CACHE_VAL(td_cv_curses_type,[
	td_cv_curses_type=unknown
	if test .$ac_cv_lib_curses_initscr = .yes; then
		AC_TRY_LINK([#include <curses.h>],
			[keypad(curscr,1)],
			[td_cv_curses_type=sysv],
			[td_cv_curses_type=bsd])
	fi
])
AC_MSG_RESULT($td_cv_curses_type)
])dnl
dnl ---------------------------------------------------------------------------
dnl See which (if any!) of the styles of curses' line/row struct fit.
AC_DEFUN([TD_CURSES_STYLE],
[
AC_MSG_CHECKING(for curses struct-style)
AC_CACHE_VAL(td_cv_curses_style,[
	td_cv_curses_style=unknown
	save_CFLAGS="$CFLAGS"
	CFLAGS="-I./include $CFLAGS"
	for td_type in ncurses sysv bsd bsd44
	do
		TD_UPPERCASE($td_type,td_tr_type)
		TD_MSG_LOG($td_tr_type curses-struct)
		AC_TRY_COMPILE([
#define	TESTING_CONFIG_H 1
#define CURSES_LIKE_$td_tr_type 1
#include <td_curse.h>],
			[long test = (long)&(CursesLine(curscr,0))],
			[td_cv_curses_style=$td_type; break])
	done
	CFLAGS="$save_CFLAGS"
])
AC_MSG_RESULT($td_cv_curses_style)
TD_UPPERCASE($td_cv_curses_style,td_tr_type)
AC_DEFINE_UNQUOTED(CURSES_LIKE_${td_tr_type})
])dnl
dnl ---------------------------------------------------------------------------
AC_DEFUN([TD_GMTOFF],
[
td_decl='#include <sys/types.h>
'
if test $ac_cv_header_sys_time_h = yes; then
	td_decl="$td_decl
#include <sys/time.h>
"
else
	td_decl="$td_decl
#include <time.h>
"
fi

AC_STRUCT_TM
AC_MSG_CHECKING(for localzone declared)
AC_CACHE_VAL(td_cv_decl_localzone,[
	AC_TRY_COMPILE([$td_decl],
		[long x = localzone],
		[td_cv_decl_localzone=yes],
		[td_cv_decl_localzone=no])])
AC_MSG_RESULT($td_cv_decl_localzone)
test $td_cv_decl_localzone = yes && AC_DEFINE(LOCALZONE_DECLARED)

AC_MSG_CHECKING(for timezone declared)
AC_CACHE_VAL(td_cv_decl_timezone,[
	AC_TRY_COMPILE($td_decl,
		[long x = timezone],
		[td_cv_decl_timezone=yes],
		[td_cv_decl_timezone=no])])
AC_MSG_RESULT($td_cv_decl_timezone)
if test $td_cv_decl_timezone = yes; then
	AC_DEFINE(TIMEZONE_DECLARED)

	AC_MSG_CHECKING(for daylight declared)
	AC_CACHE_VAL(td_cv_decl_daylight,[
		AC_TRY_COMPILE($td_decl,
			[long x = daylight],
			[td_cv_decl_daylight=yes],
			[td_cv_decl_daylight=no])])
	AC_MSG_RESULT($td_cv_decl_daylight)
	test $td_cv_decl_daylight = yes && AC_DEFINE(DAYLIGHT_DECLARED)
fi

AC_MSG_CHECKING(for .tm_gmtoff in struct tm)
AC_CACHE_VAL(td_cv_tm_gmtoff_decl,[
	AC_TRY_COMPILE([$td_decl],
		[struct tm tm; tm.tm_gmtoff],
		[td_cv_tm_gmtoff_decl=yes],
		[td_cv_tm_gmtoff_decl=no])])
AC_MSG_RESULT($td_cv_tm_gmtoff_decl)
test $td_cv_tm_gmtoff_decl = yes && AC_DEFINE(HAVE_TM_GMTOFF)

AC_MSG_CHECKING(for .tm_isdst in struct tm)
AC_CACHE_VAL(td_cv_tm_isdst_decl,[
	AC_TRY_COMPILE([$td_decl],
		[struct tm tm; tm.tm_isdst],
		[td_cv_tm_isdst_decl=yes],
		[td_cv_tm_isdst_decl=no])])
AC_MSG_RESULT($td_cv_tm_isdst_decl)
test $td_cv_tm_isdst_decl = yes && AC_DEFINE(HAVE_TM_ISDST)

AC_MSG_CHECKING(for .tm_zone in struct tm)
AC_CACHE_VAL(td_cv_tm_zone_decl,[
	AC_TRY_COMPILE([$td_decl],
		[struct tm tm; tm.tm_zone],
		[td_cv_tm_zone_decl=yes],
		[td_cv_tm_zone_decl=no])])
AC_MSG_RESULT($td_cv_tm_zone_decl)
test $td_cv_tm_zone_decl = yes && AC_DEFINE(HAVE_TM_ZONE)
])dnl
dnl ---------------------------------------------------------------------------
dnl	Combines AC_HAVE_FUNCS logic with additional test from Kevin Buettner
dnl	that checks to see if we need a prototype for the given function.
dnl
dnl The prototype-check is only turned on when we're configuring to compile
dnl with warnings.
dnl
AC_DEFUN([TD_HAVE_FUNCS],
[
td_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS -Iinclude"
for td_func in $1
do
TD_UPPERCASE($td_func,td_tr_func)
AC_MSG_CHECKING(for ${td_func})
TD_MSG_LOG(${td_func})
AC_CACHE_VAL(td_cv_func_$td_func,[
	# Test for prior functional test (e.g., vfork).
eval td_result='$ac_cv_func_'$td_func
if test ".$td_result" != ".no"; then
	# GCC won't reliably fail on prototype mismatches unless we make all
	# warnings into errors.  Of course, _this_ assumes that the config is
	# otherwise ok.
AC_TRY_LINK([#undef ${td_func}],[${td_func}();],[
if test $WithPrototypes != yes; then
	td_result=yes
elif test $WithWarnings = yes; then
	if test $ac_cv_prog_gcc = yes; then
		CFLAGS="$CFLAGS -Werror"
	fi
	AC_TRY_COMPILE([
#undef  HAVE_${td_tr_func}
#define HAVE_${td_tr_func} 1
#include <td_local.h>],
[
#undef $td_func
	struct zowie { int a; double b; struct zowie *c; char d; };
	extern struct zowie *$td_func(); $td_func() ],
	[td_result=undeclared],
	[td_result=declared])
else
	td_result=yes
fi
],
[td_result=no])
fi
eval 'td_cv_func_'$td_func'=$td_result'
])
# use the computed/retrieved cache-value:
eval 'td_result=$td_cv_func_'$td_func
AC_MSG_RESULT($td_result)
if test $td_result != no; then
	AC_DEFINE_UNQUOTED(HAVE_${td_tr_func})
	if test $td_result = undeclared; then
		AC_DEFINE_UNQUOTED(NEED_${td_tr_func})
	fi
fi
done
CFLAGS="$td_CFLAGS"
])dnl
dnl ---------------------------------------------------------------------------
dnl	Adds to the include-path
dnl
dnl	Autoconf 1.11 should have provided a way to add include path options to
dnl	the cpp-tests.
dnl
AC_DEFUN([TD_INCLUDE_PATH],
[
for td_path in $1
do
	td_result=""
	AC_MSG_CHECKING(for directory $td_path)
	if test -d $td_path
	then
		INCLUDES="$INCLUDES -I$td_path"
		ac_cpp="${ac_cpp} -I$td_path"
		CFLAGS="$CFLAGS -I$td_path"
		td_result="$td_result $td_path"
	fi
	if test -z "$td_result"; then
		td_result=no
	else
		td_result=yes
	fi
	AC_MSG_RESULT($td_result)
done
])dnl
dnl ---------------------------------------------------------------------------
dnl	Adds to the library-path
dnl
dnl	Some machines have trouble with multiple -L options.
dnl
AC_DEFUN([TD_LIBRARY_PATH],
[
for td_path in $1
do
	if test -d $td_path
	then
		AC_VERBOSE(adding $td_path to library-path)
		LIBS="$LIBS -L$td_path"
		CFLAGS="$CFLAGS -L$td_path"
	fi
done
])dnl
dnl ---------------------------------------------------------------------------
dnl Some 'make' programs support $(MAKEFLAGS), some $(MFLAGS), to pass 'make'
dnl options to lower-levels.  It's very useful for "make -n" -- if we have it.
dnl (GNU 'make' does both :-)
AC_DEFUN([TD_MAKEFLAGS],
[
AC_MSG_CHECKING([for makeflags variable])
AC_CACHE_VAL(td_cv_makeflags,[
	td_cv_makeflags=''
	for td_option in '$(MFLAGS)' '-$(MAKEFLAGS)'
	do
		cat >td_lib.tmp <<TD_EOF
all :
	echo '.$td_option'
TD_EOF
		set td_result=`${MAKE-make} -f td_lib.tmp 2>/dev/null`
		if test "$td_result" != "."
		then
			td_cv_makeflags=$td_option
			break
		fi
	done
	rm -f td_lib.tmp
	])
AC_MSG_RESULT($td_cv_makeflags)
AC_SUBST(td_cv_makeflags)
]
)dnl
dnl ---------------------------------------------------------------------------
AC_DEFUN([TD_MSG_LOG],
echo "(line __oline__) testing $* ..." 1>&5
)dnl
dnl ---------------------------------------------------------------------------
dnl Tests for one or more programs given by name along the user's path, and
dnl sets a variable to the program's full-path if found.
AC_DEFUN([TD_PROGRAM_FULLPATH],
[
AC_MSG_CHECKING(full path of $1)
AC_CACHE_VAL(td_cv_$1,[
	td_cv_$1="[$]$1"
	if test -z "[$]td_cv_$1"; then
		set -- $2;
		while test [$]# != 0; do
			td_word=[$]1
			case [$]1 in
			-*)
				;;
			*)
				if test -f "$td_word" && test ! -f "./$td_word" && test -x "$td_word"; then
					td_cv_$1="$td_word"
				else
					IFS="${IFS= 	}"; td_save_ifs="$IFS"; IFS="${IFS}:"
					for td_dir in $PATH; do
						test -z "$td_dir" && td_dir=.
						if test "$td_dir" != "." && test -f $td_dir/$td_word && test -x $td_dir/$td_word; then
							td_cv_$1="$td_dir/$td_word"
							break
						fi
					done
					IFS="$td_save_ifs"
				fi
				if test -n "[$]td_cv_$1"; then
					shift
					break
				fi
				;;
			esac
			shift
		done
	fi
	# append options, if any
	if test -n "[$]td_cv_$1"; then
		while test [$]# != 0; do
			case [$]1 in
			-[*]) td_cv_$1="[$]td_cv_$1 [$]1";;
			[*])  set -- end;;
			esac
			shift
		done
	fi
])
if test -n "[$]td_cv_$1"; then
	AC_DEFINE_UNQUOTED($1,"[$]td_cv_$1")
  AC_MSG_RESULT("[$]td_cv_$1")
else
  AC_MSG_RESULT((not found))
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl	Tests for a program given by name along the user's path, and sets a
dnl	variable to the program's directory-prefix if found.  Don't match if
dnl	the directory is ".", since we need an absolute path-reference.
AC_DEFUN([TD_PROGRAM_PREFIX],
[
# Extract the first word of `$2', so it can be a program name with args.
set td_dummy $2; td_word=[$]2
AC_MSG_CHECKING(for $td_word prefix ($1))
AC_CACHE_VAL([td_cv_$1],[
	# allow import from environment-variable
	td_cv_$1="[$]$1"
	if test -z "[$]td_cv_$1"; then
		IFS="${IFS= 	}"; td_save_ifs="$IFS"; IFS="${IFS}:"
		for td_dir in $PATH; do
			test -z "$td_dir" && td_dir=.
			if test "$td_dir" != "." && test -f $td_dir/$td_word && test -x $td_dir/$td_word; then
				td_cv_$1="$td_dir"
				break
			fi
		done
		IFS="$td_save_ifs"
	fi
	ifelse([$3],,, [test -z "[$]td_cv_$1" && td_cv_$1="$3"])
])
if test -n "[$]td_cv_$1"; then
  AC_DEFINE_UNQUOTED($1,"[$]td_cv_$1")
  AC_MSG_RESULT("[$]td_cv_$1")
else
  AC_MSG_RESULT((not found))
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for the ensemble of programs that are used in RCS, SCCS, VCS, CVS.
dnl We'll have to assume that the related utilities all reside in the same
dnl directory.
AC_DEFUN([TD_RCS_SCCS],
[TD_PROGRAM_PREFIX(RCS_PATH, rcs)
TD_PROGRAM_PREFIX(SCCS_PATH, admin)dnl the SCCS tool
TD_PROGRAM_PREFIX(VCS_PATH, vcs)dnl VCS is my RCS application
TD_PROGRAM_PREFIX(CVS_PATH, cvs)dnl CVS is a layer above RCS
TD_PROGRAM_PREFIX(CMV_PATH, cmv)dnl CmVision combines RCS and SCCS archives
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for the presence of re_comp/re_exec functions (no include-file?)
AC_DEFUN([TD_RE_COMP_FUNCS],
[
AC_MSG_CHECKING(re_comp/re_exec functions)
AC_CACHE_VAL(td_cv_RE_COMP_func,[
	AC_TRY_RUN([
		int main() {
		extern char *re_comp();
		char *e;
		char *p = "foo";
		char *s = "foobar";
		if ((e = (char *)re_comp(p)) != 0
		 || re_exec(s) <= 0)
		 	exit(1);
		exit(0);
	}
	],
	[td_cv_RE_COMP_func=yes],
	[td_cv_RE_COMP_func=no],
	[td_cv_RE_COMP_func=unknown])
	])
AC_MSG_RESULT($td_cv_RE_COMP_func)
test $td_cv_RE_COMP_func = yes && AC_DEFINE(HAVE_RE_COMP_FUNCS)
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for the ensemble of include-files and functions that make up the
dnl host's regular expression parsing.
dnl
dnl Only use REGCMP functions if no other is available, to avoid spurious
dnl match on HP/UX with its broken -lPW.
AC_DEFUN([TD_REGEX],
[
TD_REGEX_H_FUNCS
TD_REGEXPR_H_FUNCS
TD_RE_COMP_FUNCS
if	test $td_cv_REGEX_H = no && \
	test $td_cv_REGEXPR_H = no && \
	test $td_cv_RE_COMP_func = no ; then
	TD_REGCMP_FUNCS
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for the <regex.h> include-file, and the functions associated with it.
AC_DEFUN([TD_REGEX_H_FUNCS],
[
AC_MSG_CHECKING(regcomp/regexec functions)
AC_CACHE_VAL(td_cv_REGEX_H,[
	AC_TRY_RUN([
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
		} ],
		[td_cv_REGEX_H=yes],
		[td_cv_REGEX_H=no],
		[td_cv_REGEX_H=unknown])
	])
AC_MSG_RESULT($td_cv_REGEX_H)
test $td_cv_REGEX_H = yes && AC_DEFINE(HAVE_REGEX_H_FUNCS)
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for the <regexpr.h> include-file, and the functions associated with it.
AC_DEFUN([TD_REGEXPR_H_FUNCS],
[
td_save_libs="$LIBS"
AC_CHECK_LIB(gen, compile)
AC_MSG_CHECKING(compile/step functions)
AC_CACHE_VAL(td_cv_REGEXPR_H,[
	AC_TRY_RUN([
#include <sys/types.h>
#include <regexpr.h>
		int main() {
			char *e;
			char *p = "foo";
			char *s = "foobar";
			if ((e = (char *)compile(p, (char *)0, (char *)0)) == 0
			 || step(s, e) == 0)
		 		exit(1);
			free(e);
			exit(0);
		} ],
		[td_cv_REGEXPR_H=yes],
		[td_cv_REGEXPR_H=no],
		[td_cv_REGEXPR_H=unknown])
	])
AC_MSG_RESULT($td_cv_REGEXPR_H)
if test $td_cv_REGEXPR_H = yes; then
	AC_DEFINE(HAVE_REGEXPR_H_FUNCS)
else
	LIBS="$td_save_libs"
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl Test for the presence of regcmp/regex functions.
AC_DEFUN([TD_REGCMP_FUNCS],
[
AC_HAVE_HEADERS(pw.h libgen.h)
AC_MSG_CHECKING(regcmp/regex functions)
AC_CACHE_VAL(td_cv_REGCMP_func,[
	TD_REGCMP_LIBS
	if test $td_cv_REGCMP_func = no; then
		td_save_LIBS="$LIBS"
		LIBS="-lPW $LIBS"
		TD_REGCMP_LIBS
		LIBS="${td_save_LIBS}"
		test $td_cv_REGCMP_func = yes && td_cv_REGCMP_func="yes-lPW"
	fi
])
AC_MSG_RESULT($td_cv_REGCMP_func)
if test $td_cv_REGCMP_func != no; then
	AC_DEFINE(HAVE_REGCMP_FUNCS)
	test $td_cv_REGCMP_func != yes && AC_CHECK_LIB(PW, regcmp)
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for the presence of regcmp/regex functions (no include-file?)
dnl Some systems (CLIX) use <pw.h> for this purpose.  CLIX requires the -lPW,
dnl but HPUX has only a broken version of that library, so we've got to
dnl try the compile with/without the library.
dnl
dnl By the way: CLIX's PW library has a conflict with the curses library's
dnl "move()" function.
AC_DEFUN([TD_REGCMP_LIBS],
[
AC_TRY_RUN([
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
		if ((e = (char *)regcmp(p, 0)) == 0
		 || regex(e, s, 0) == 0)
		 	exit(1);
		free(e);
		exit(0);
	} ],
	[td_cv_REGCMP_func=yes],
	[td_cv_REGCMP_func=no],
	[td_cv_REGCMP_func=unknown])
])dnl
dnl ---------------------------------------------------------------------------
dnl Test for non-Posix prototype for 'signal()'
dnl
dnl	Apollo sr10.x defines prototypes for the signal handling function that
dnl	have an extra argument (in comparison with Standard C).  Also, on the
dnl	systems that have standard prototypes, the predefined actions often
dnl	have incomplete prototypes.  This macro tries to test for these cases
dnl	so that when compiling I don't see unnecessary warning messages from
dnl	gcc.
dnl
dnl	(If the compiler doesn't recognize prototypes, of course, this test
dnl	will not find anything :-)
dnl
AC_DEFUN([TD_SIG_ARGS],
[
AC_REQUIRE([AC_TYPE_SIGNAL])
AC_MSG_CHECKING(for non-standard signal handler prototype)
AC_CACHE_VAL(td_cv_sig_args,[
	AC_TRY_RUN([
#include <signal.h>
		RETSIGTYPE (*signal(int sig, RETSIGTYPE(*func)(int sig)))(int sig2);
		RETSIGTYPE catch(int sig) { exit(1); }
		main() { signal(SIGINT, catch); exit(0); } ],
		[td_cv_sig_args=STANDARD],
		[AC_TRY_RUN([
#include <signal.h>
			RETSIGTYPE (*signal(int sig, RETSIGTYPE(*func)(int sig,...)))(int sig2,...);
			RETSIGTYPE catch(int sig, ...) { exit(1); }
			main() { signal(SIGINT, catch); exit(0); } ],
			[td_cv_sig_args=VARYING],
			[td_cv_sig_args=UNKNOWN],
			[td_cv_sig_args=UNKNOWN])
		],
		[td_cv_sig_args=UNKNOWN])
	])
AC_MSG_RESULT($td_cv_sig_args)
AC_DEFINE_UNQUOTED(SIG_ARGS_$td_cv_sig_args)

if test -n "$GCC" && test $WithWarnings = yes
then
	AC_MSG_CHECKING(redefinable signal handler prototype)
	AC_CACHE_VAL(td_cv_sigs_redef,[
		td_cv_sigs_redef=no
		# We're checking the definitions of the commonly-used predefined signal
		# macros, to see if their values are the ones that we expect.  If so,
		# we'll plug in our own definitions, that have complete prototypes.  We
		# do this when we're developing with gcc, with all warnings, and
		# shouldn't do it for other compilers, since (for example) the IRIX
		# compiler complains a lot.
		if test $td_cv_sig_args != UNKNOWN; then	# we have prototypes
			AC_TRY_RUN([
#include <signal.h>
#undef  NOT
#define NOT(s,d) ((long)(s) != (long)(d))
				main() { exit(NOT(SIG_IGN,1) || NOT(SIG_DFL,0) || NOT(SIG_ERR,-1)); } ],
				[td_cv_sigs_redef=yes],
				[td_cv_sigs_redef=no],
				[td_cv_sigs_redef=unknown])
		fi
		])
		AC_MSG_RESULT($td_cv_sigs_redef)
		test $td_cv_sigs_redef = yes && AC_DEFINE(SIG_IGN_REDEFINEABLE)
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl	On both Ultrix and CLIX, I find size_t defined in <stdio.h>
AC_DEFUN([TD_SIZE_T],
[
AC_MSG_CHECKING(for size_t in <sys/types.h> or <stdio.h>)
AC_CACHE_VAL(td_cv_type_size_t,[
	AC_TRY_COMPILE([
#include <sys/types.h>
#include <stdio.h>],
		[size_t x],
		[td_cv_type_size_t=yes],
		[td_cv_type_size_t=no])
	])
AC_MSG_RESULT($td_cv_type_size_t)
test $td_cv_type_size_t = no && AC_DEFINE(size_t, unsigned)
])dnl
dnl ---------------------------------------------------------------------------
dnl Test if the <sys/stat.h> 'stat' struct defines 'st_blocks' member.
dnl If not, assume it's some non-BSD system.
AC_DEFUN([TD_STAT_ST_BLOCKS],
[
AC_MSG_CHECKING(for .st_blocks in struct stat)
AC_CACHE_VAL(td_cv_st_blocks,[
	AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/stat.h>],
		[int t() {struct stat sb; return sb.st_blocks;}],
		[td_cv_st_blocks=yes],
		[td_cv_st_blocks=no])])
AC_MSG_RESULT($td_cv_st_blocks)
test $td_cv_st_blocks = yes && AC_DEFINE(STAT_HAS_ST_BLOCKS)
])dnl
dnl ---------------------------------------------------------------------------
dnl Test if curses defines 'struct screen'.
dnl
dnl	If this isn't defined, we cannot build a lint library that will check
dnl	for that type, since it isn't resolved.
dnl
AC_DEFUN([TD_STRUCT_SCREEN],
[
AC_MSG_CHECKING(if curses uses struct screen)
AC_CACHE_VAL(td_cv_have_struct_screen,[
	AC_TRY_COMPILE([
#define lint	/* sysvr4 has its own fallback for lint */
#include <curses.h>],
		[struct screen dummy],
		[td_cv_have_struct_screen=yes],
		[td_cv_have_struct_screen=no])
	])
AC_MSG_RESULT($td_cv_have_struct_screen)

AC_MSG_CHECKING(for definition of struct screen)
AC_CACHE_VAL(td_cv_need_struct_screen,[
	if test $td_cv_have_struct_screen = yes; then
		AC_TRY_COMPILE([#include <curses.h>],
			[struct screen { int dummy;}],
			[td_cv_need_struct_screen=yes],
			[td_cv_need_struct_screen=no])
	else
		td_cv_need_struct_screen=no
	fi
	])
AC_MSG_RESULT($td_cv_need_struct_screen)
test $td_cv_need_struct_screen = yes && AC_DEFINE(NEED_STRUCT_SCREEN)
])dnl
dnl ---------------------------------------------------------------------------
dnl Test for the presence of the 'sys_errlist[]' array if we don't have the
dnl 'strerror()' function.
AC_DEFUN([TD_SYS_ERRLIST],
[
AC_MSG_CHECKING(for sys_errlist in <errno.h>)
AC_CACHE_VAL(td_cv_sys_errlist,[
	AC_TRY_RUN([
#include <stdio.h>
#include <errno.h>
		extern char *sys_errlist[];
		extern int sys_nerr;
		int main() { char *x = sys_errlist[sys_nerr-1]; exit (x==0);}],
		[td_cv_sys_errlist=yes],
		[td_cv_sys_errlist=no],
		[td_cv_sys_errlist=unknown])])
AC_MSG_RESULT($td_cv_sys_errlist)
test $td_cv_sys_errlist = yes && AC_DEFINE(HAVE_SYS_ERRLIST)
])dnl
dnl ---------------------------------------------------------------------------
dnl Test if curses defines KD, KU, etc., for cursor keys
dnl
dnl	I found at least one implementation of curses that didn't declare these
dnl	variables in the include-file, but did define them in the library.  If
dnl	they're not in the include-file, ignore them.  Otherwise, assume that
dnl	curses initializes them in 'initscr()'.
dnl
AC_DEFUN([TD_TCAP_CURSOR],
[
AC_MSG_CHECKING(for termcap-cursor variables)
AC_CACHE_VAL(td_cv_termcap_cursor,[
	AC_TRY_COMPILE([#include <curses.h>],
		[char *d=KD, *u=KU, *r=KR, *l=KL],
		[td_cv_termcap_cursor=yes],
		[td_cv_termcap_cursor=no])])
AC_MSG_RESULT($td_cv_termcap_cursor)
test $td_cv_termcap_cursor = yes && AC_DEFINE(HAVE_TCAP_CURSOR)
])dnl
dnl ---------------------------------------------------------------------------
dnl Make an uppercase version of a given name
AC_DEFUN([TD_UPPERCASE],
[
changequote(,)dnl
$2=`echo $1 |tr '[a-z]' '[A-Z]'`
changequote([,])dnl
])dnl
dnl ---------------------------------------------------------------------------
dnl Check for ANSI stdarg.h vs varargs.h.  Note that some systems include
dnl <varargs.h> within <stdarg.h>.
AC_DEFUN([TD_VARARGS],
[
AC_MSG_CHECKING(for standard varargs)
AC_CACHE_VAL(td_cv_ansi_varargs,[
	AC_TRY_COMPILE([
#if HAVE_STDARG_H
#include <stdarg.h>
#else
#if HAVE_VARARGS_H
#include <varargs.h>
#endif
#endif
		],
		[return 0;} void foo(char *fmt,...){va_list args;va_start(args,fmt);va_end(args)],
		[td_cv_ansi_varargs=yes],
		[td_cv_ansi_varargs=no])
	])
AC_MSG_RESULT($td_cv_ansi_varargs)
test $td_cv_ansi_varargs = yes && AC_DEFINE(ANSI_VARARGS)
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
AC_DEFUN([TD_WAIT],
[
AC_HAVE_HEADERS(wait.h)
AC_HAVE_HEADERS(sys/wait.h)
td_decl="#include <sys/types.h>
"
if test $ac_cv_header_sys_wait_h = yes; then
td_decl="$td_decl
#include <sys/wait.h>
"
elif test $ac_cv_header_wait_h = yes; then
td_decl="$td_decl
#include <wait.h>
"
fi
AC_MSG_CHECKING(union wait declared)
AC_CACHE_VAL(td_cv_decl_union_wait,[
	AC_TRY_COMPILE($td_decl,
		[union wait x],
		[td_cv_decl_union_wait=yes],
		[td_cv_decl_union_wait=no])
	])
AC_MSG_RESULT($td_cv_decl_union_wait)

if test $td_cv_decl_union_wait = yes; then
	AC_MSG_CHECKING(union wait used as wait-arg)
	AC_CACHE_VAL(td_cv_arg_union_wait,[
		AC_TRY_COMPILE($td_decl,
 			[union wait x; wait(&x)],
			[td_cv_arg_union_wait=yes],
			[td_cv_arg_union_wait=no])
		])
	AC_MSG_RESULT($td_cv_arg_union_wait)
	test $td_cv_arg_union_wait = yes && AC_DEFINE(WAIT_USES_UNION)
fi
])dnl
