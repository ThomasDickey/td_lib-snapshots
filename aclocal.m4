dnl Extended Macros that test for specific features.
dnl $Header: /users/source/archives/td_lib.vcs/RCS/aclocal.m4,v 12.89 1997/09/08 23:58:03 tom Exp $
dnl vi:set ts=4:
dnl ---------------------------------------------------------------------------
dnl BELOW THIS LINE CAN BE PUT INTO "acspecific.m4", by changing "CF_" to "AC_"
dnl and "cf_" to "ac_".
dnl ---------------------------------------------------------------------------
dnl ---------------------------------------------------------------------------
dnl This is adapted from the macros 'fp_PROG_CC_STDC' and 'fp_C_PROTOTYPES'
dnl in the sharutils 4.2 distribution.
AC_DEFUN([CF_ANSI_CC_CHECK],
[
AC_MSG_CHECKING(for ${CC-cc} option to accept ANSI C)
AC_CACHE_VAL(cf_cv_ansi_cc,[
cf_cv_ansi_cc=no
cf_save_CFLAGS="$CFLAGS"
# Don't try gcc -ansi; that turns off useful extensions and
# breaks some systems' header files.
# AIX			-qlanglvl=ansi
# Ultrix and OSF/1	-std1
# HP-UX			-Aa -D_HPUX_SOURCE
# SVR4			-Xc
# UnixWare 1.2		(cannot use -Xc, since ANSI/POSIX clashes)
for cf_arg in "-DCC_HAS_PROTOS" "" -qlanglvl=ansi -std1 "-Aa -D_HPUX_SOURCE" -Xc
do
	CFLAGS="$cf_save_CFLAGS $cf_arg"
	AC_TRY_COMPILE(
[
#ifndef CC_HAS_PROTOS
#if !defined(__STDC__) || __STDC__ != 1
choke me
#endif
#endif
],[
	int test (int i, double x);
	struct s1 {int (*f) (int a);};
	struct s2 {int (*f) (double a);};],
	[cf_cv_ansi_cc="$cf_arg"; break])
done
CFLAGS="$cf_save_CFLAGS"
])
AC_MSG_RESULT($cf_cv_ansi_cc)

if test "$cf_cv_ansi_cc" != "no"; then
if test ".$cf_cv_ansi_cc" != ".-DCC_HAS_PROTOS"; then
	CFLAGS="$CFLAGS $cf_cv_ansi_cc"
else
	AC_DEFINE(CC_HAS_PROTOS)
fi
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl Test if "##" is substituted properly, or failing that, if /**/ can do
dnl the trick.  We can test concatenation with a compile, but quoting has to
dnl be tested by running a program.
AC_DEFUN([CF_ANSI_CPP],
[
AC_MSG_CHECKING(for ANSI CPP token-splicing)
AC_CACHE_VAL(cf_cv_ansi_splice,[
	cf_cv_ansi_splice=unknown
	AC_TRY_COMPILE(
		[#define cat(a,b) a##b],
		[cat(lo,ng) x; char *y = "a" "b"],
		[cf_cv_ansi_splice=new],
		[AC_TRY_COMPILE(
			[#define cat(a,b) a/**/b],
			[cat(lo,ng) x],
			[cf_cv_ansi_splice=old])
		])
	])

AC_MSG_RESULT(${cf_cv_ansi_splice}-style)
if test $cf_cv_ansi_splice = new; then
	AC_DEFINE(HAVE_NEW_TOKEN_SPLICE)
elif test $cf_cv_ansi_splice = old; then
	AC_DEFINE(HAVE_OLD_TOKEN_SPLICE)
fi

AC_MSG_CHECKING(for ANSI CPP token-quoting)
AC_CACHE_VAL(cf_cv_ansi_quote,[
	AC_TRY_RUN([#define quote(name) #name
		int main() { char *y = quote(a); exit (*y != 'a');} ],
		[cf_cv_ansi_quote=new],
		[cf_cv_ansi_quote=unknown],
		[cf_cv_ansi_quote=unknown])
	if test $cf_cv_ansi_quote = unknown; then
		AC_TRY_RUN([#define quote(name) "name"
			int main() { char *y = quote(a); exit (*y != 'a');} ],
			[cf_cv_ansi_quote=old],
			[cf_cv_ansi_quote=unknown],
			[cf_cv_ansi_quote=unknown])
		fi
	])

AC_MSG_RESULT(${cf_cv_ansi_quote}-style)
if test $cf_cv_ansi_quote = new; then
	AC_DEFINE(HAVE_NEW_TOKEN_QUOTE)
elif test $cf_cv_ansi_quote = old; then
	AC_DEFINE(HAVE_OLD_TOKEN_QUOTE)
fi

])dnl
dnl ---------------------------------------------------------------------------
dnl Allow user to disable a normally-on option.
AC_DEFUN([CF_ARG_DISABLE],
[CF_ARG_OPTION($1,[$2 (default: on)],[$3],[$4],yes)])dnl
dnl ---------------------------------------------------------------------------
dnl Restricted form of AC_ARG_ENABLE that ensures user doesn't give bogus
dnl values.
dnl
dnl Parameters:
dnl $1 = option name
dnl $2 = help-string 
dnl $3 = action to perform if option is not default
dnl $4 = action if perform if option is default
dnl $5 = default option value (either 'yes' or 'no')
AC_DEFUN([CF_ARG_OPTION],
[AC_ARG_ENABLE($1,[$2],[test "$enableval" != ifelse($5,no,yes,no) && enableval=ifelse($5,no,no,yes)
  if test "$enableval" != "$5" ; then
ifelse($3,,[    :]dnl
,[    $3]) ifelse($4,,,[
  else
    $4])
  fi],[enableval=$5 ifelse($4,,,[
  $4
])dnl
  ])])dnl
dnl ---------------------------------------------------------------------------
dnl Check if we're accidentally using a cache from a different machine.
dnl Derive the system name, as a check for reusing the autoconf cache.
dnl
dnl If we've packaged config.guess and config.sub, run that (since it does a
dnl better job than uname). 
AC_DEFUN([CF_CHECK_CACHE],
[
if test -f $srcdir/config.guess ; then
	AC_CANONICAL_HOST
	system_name="$host_os"
else
	system_name="`(uname -s -r) 2>/dev/null`"
	if test -z "$system_name" ; then
		system_name="`(hostname) 2>/dev/null`"
	fi
fi
test -n "$system_name" && AC_DEFINE_UNQUOTED(SYSTEM_NAME,"$system_name")
AC_CACHE_VAL(cf_cv_system_name,[cf_cv_system_name="$system_name"])

test -z "$system_name" && system_name="$cf_cv_system_name"
test -n "$cf_cv_system_name" && AC_MSG_RESULT("Configuring for $cf_cv_system_name")

if test ".$system_name" != ".$cf_cv_system_name" ; then
	AC_MSG_RESULT("Cached system name does not agree with actual")
	AC_ERROR("Please remove config.cache and try again.")
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for the ensemble of include-files and functions that make up the
dnl host's regular expression parsing.
dnl
dnl Only use REGCMP functions if no other is available, to avoid spurious
dnl match on HP/UX with its broken -lPW.
AC_DEFUN([CF_CHECK_REGEX],
[
CF_REGEX_H_FUNCS
CF_REGEXPR_H_FUNCS
CF_RE_COMP_FUNCS
if	test $cf_cv_REGEX_H = no && \
	test $cf_cv_REGEXPR_H = no && \
	test $cf_cv_RE_COMP_func = no ; then
	CF_REGCMP_FUNCS
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl Test if curses defines 'chtype' (usually a 16-bit type for SysV curses).
AC_DEFUN([CF_CURSES_CHTYPE],
[
AC_MSG_CHECKING(for chtype typedef)
AC_CACHE_VAL(cf_cv_chtype_decl,[
	AC_TRY_COMPILE([#include <curses.h>],
		[chtype foo],
		[cf_cv_chtype_decl=yes],
		[cf_cv_chtype_decl=no])])
AC_MSG_RESULT($cf_cv_chtype_decl)
test $cf_cv_chtype_decl = yes && AC_DEFINE(HAVE_TYPE_CHTYPE)
])dnl
dnl ---------------------------------------------------------------------------
dnl Test for curses data/types
AC_DEFUN([CF_CURSES_DATA],
[
CF_CURSES_CHTYPE
CF_CURSES_STYLE
CF_STRUCT_SCREEN
CF_TCAP_CURSOR
])dnl
dnl ---------------------------------------------------------------------------
dnl Curses-functions are a little complicated, since a lot of them are macros.
AC_DEFUN([CF_CURSES_FUNCS],
[
for cf_func in $1
do
	CF_UPPER(cf_tr_func,$cf_func)
	AC_MSG_CHECKING(for ${cf_func})
	CF_MSG_LOG(${cf_func})
	AC_CACHE_VAL(cf_cv_func_$cf_func,[
		eval cf_result='$ac_cv_func_'$cf_func
		if test ".$cf_result" != ".no"; then
			AC_TRY_LINK([#include <curses.h>],
			[
#ifndef ${cf_func}
long foo = (long)(&${cf_func});
#endif
			],
			[cf_result=yes],
			[cf_result=no])
		fi
		eval 'cf_cv_func_'$cf_func'=$cf_result'
	])
	# use the computed/retrieved cache-value:
	eval 'cf_result=$cf_cv_func_'$cf_func
	AC_MSG_RESULT($cf_result)
	if test $cf_result != no; then
		AC_DEFINE_UNQUOTED(HAVE_${cf_tr_func})
	fi
done
])dnl
dnl ---------------------------------------------------------------------------
dnl Test for the existence of SysVr4 mouse support in curses. If we've not got
dnl it, we'll simulate the interface (for xterm, at least).
AC_DEFUN([CF_CURSES_MOUSE],
[
AC_MSG_CHECKING(for curses mouse-support)
AC_CACHE_VAL(cf_cv_curses_mouse,[
AC_TRY_LINK([#include <curses.h>],[
	int x, y;
	getmouse();
	request_mouse_pos();
	wmouse_position(stdscr, &x, &y);
	mouse_on(TRUE);
	mouse_off(FALSE);
	mouse_set(0);],
	[cf_cv_curses_mouse=yes],
	[cf_cv_curses_mouse=no])
])
AC_MSG_RESULT($cf_cv_curses_mouse)
test $cf_cv_curses_mouse = yes && AC_DEFINE(CURSES_HAS_MOUSE)
])dnl
dnl ---------------------------------------------------------------------------
dnl See which (if any!) of the styles of curses' line/row struct fit.
AC_DEFUN([CF_CURSES_STYLE],
[
AC_MSG_CHECKING(for curses struct-style)
AC_CACHE_VAL(cf_cv_curses_style,[
	cf_cv_curses_style=unknown
	save_CFLAGS="$CFLAGS"
	CFLAGS="-I./include $CFLAGS"
	for cf_type in ncurses sysv bsd bsd44
	do
		CF_UPPER(cf_tr_type,$cf_type)
		CF_MSG_LOG($cf_tr_type curses-struct)
		AC_TRY_COMPILE([
#define	TESTING_CONFIG_H 1
#define CURSES_LIKE_$cf_tr_type 1
#include <td_curse.h>],
			[long test = (long)&(CursesLine(curscr,0))],
			[cf_cv_curses_style=$cf_type; break])
	done
	CFLAGS="$save_CFLAGS"
])
AC_MSG_RESULT($cf_cv_curses_style)
CF_UPPER(cf_tr_type,$cf_cv_curses_style)
AC_DEFINE_UNQUOTED(CURSES_LIKE_${cf_tr_type})
])dnl
dnl ---------------------------------------------------------------------------
dnl You can always use "make -n" to see the actual options, but it's hard to
dnl pick out/analyze warning messages when the compile-line is long.
AC_DEFUN([CF_DISABLE_ECHO],[
AC_MSG_CHECKING(if you want to see long compiling messages)
CF_ARG_DISABLE(echo,
	[  --disable-echo          test: display \"compiling\" commands],
	[
    ECHO_LD='@echo linking [$]@;'
    SHOW_CC='	@echo compiling [$]@'
    ECHO_CC='@'
],[
    ECHO_LD=''
    SHOW_CC='# compiling'
    ECHO_CC=''
])
AC_MSG_RESULT($enableval)
AC_SUBST(ECHO_LD)
AC_SUBST(SHOW_CC)
AC_SUBST(ECHO_CC)
])dnl
dnl ---------------------------------------------------------------------------
dnl A conventional existence-check for 'lstat' won't work with the Linux
dnl version of gcc 2.7.0, since the symbol is defined only within <sys/stat.h>
dnl as an inline function.
dnl
dnl So much for portability.
AC_DEFUN([CF_FUNC_LSTAT],
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
dnl Test for availability of useful gcc __attribute__ directives to quiet
dnl compiler warnings.  Though useful, not all are supported -- and contrary
dnl to documentation, unrecognized directives cause older compilers to barf.
AC_DEFUN([CF_GCC_ATTRIBUTES],
[
if test -n "$GCC"
then
cat > conftest.i <<EOF
#ifndef GCC_PRINTF
#define GCC_PRINTF 0
#endif
#ifndef GCC_SCANF
#define GCC_SCANF 0
#endif
#ifndef GCC_NORETURN
#define GCC_NORETURN /* nothing */
#endif
#ifndef GCC_UNUSED
#define GCC_UNUSED /* nothing */
#endif
EOF
if test -n "$GCC"
then
	AC_CHECKING([for gcc __attribute__ directives])
	changequote(,)dnl
cat > conftest.$ac_ext <<EOF
#line __oline__ "configure"
#include "confdefs.h"
#include "conftest.h"
#include "conftest.i"
#if	GCC_PRINTF
#define GCC_PRINTFLIKE(fmt,var) __attribute__((format(printf,fmt,var)))
#else
#define GCC_PRINTFLIKE(fmt,var) /*nothing*/
#endif
#if	GCC_SCANF
#define GCC_SCANFLIKE(fmt,var)  __attribute__((format(scanf,fmt,var)))
#else
#define GCC_SCANFLIKE(fmt,var)  /*nothing*/
#endif
extern void wow(char *,...) GCC_SCANFLIKE(1,2);
extern void oops(char *,...) GCC_PRINTFLIKE(1,2) GCC_NORETURN;
extern void foo(void) GCC_NORETURN;
int main(int argc GCC_UNUSED, char *argv[] GCC_UNUSED) { return 0; }
EOF
	changequote([,])dnl
	for cf_attribute in scanf printf unused noreturn
	do
		CF_UPPER(CF_ATTRIBUTE,$cf_attribute)
		cf_directive="__attribute__(($cf_attribute))"
		echo "checking for gcc $cf_directive" 1>&AC_FD_CC
		case $cf_attribute in
		scanf|printf)
		cat >conftest.h <<EOF
#define GCC_$CF_ATTRIBUTE 1
EOF
			;;
		*)
		cat >conftest.h <<EOF
#define GCC_$CF_ATTRIBUTE $cf_directive
EOF
			;;
		esac
		if AC_TRY_EVAL(ac_compile); then
			test -n "$verbose" && AC_MSG_RESULT(... $cf_attribute)
			cat conftest.h >>confdefs.h
#		else
#			sed -e 's/__attr.*/\/*nothing*\//' conftest.h >>confdefs.h
		fi
	done
else
	fgrep define conftest.i >>confdefs.h
fi
rm -rf conftest*
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl Check if the compiler supports useful warning options.  There's a few that
dnl we don't use, simply because they're too noisy:
dnl
dnl	-Wconversion (useful in older versions of gcc, but not in gcc 2.7.x)
dnl	-Wredundant-decls (system headers make this too noisy)
dnl	-Wtraditional (combines too many unrelated messages, only a few useful)
dnl	-Wwrite-strings (too noisy, but should review occasionally)
dnl	-pedantic
dnl
AC_DEFUN([CF_GCC_WARNINGS],
[EXTRA_CFLAGS=""
if test -n "$GCC"
then
	changequote(,)dnl
	cat > conftest.$ac_ext <<EOF
#line __oline__ "configure"
int main(int argc, char *argv[]) { return argv[argc-1] == 0; }
EOF
	changequote([,])dnl
	AC_CHECKING([for gcc warning options])
	cf_save_CFLAGS="$CFLAGS"
	EXTRA_CFLAGS="-W -Wall"
	for cf_opt in \
		Wbad-function-cast \
		Wcast-align \
		Wcast-qual \
		Winline \
		Wmissing-declarations \
		Wmissing-prototypes \
		Wnested-externs \
		Wpointer-arith \
		Wshadow \
		Wstrict-prototypes
	do
		CFLAGS="$cf_save_CFLAGS $EXTRA_CFLAGS -$cf_opt"
		if AC_TRY_EVAL(ac_compile); then
			test -n "$verbose" && AC_MSG_RESULT(... -$cf_opt)
			EXTRA_CFLAGS="$EXTRA_CFLAGS -$cf_opt"
			test "$cf_opt" = Wcast-qual && EXTRA_CFLAGS="$EXTRA_CFLAGS -DXTSTRINGDEFINES"
		fi
	done
	rm -f conftest*
	CFLAGS="$cf_save_CFLAGS"
fi
AC_SUBST(EXTRA_CFLAGS)
])dnl
dnl ---------------------------------------------------------------------------
AC_DEFUN([CF_GMTOFF],
[
cf_decl='#include <sys/types.h>
'
if test $ac_cv_header_sys_time_h = yes; then
	cf_decl="$cf_decl
#include <sys/time.h>
"
else
	cf_decl="$cf_decl
#include <time.h>
"
fi

AC_STRUCT_TM
AC_MSG_CHECKING(for localzone declared)
AC_CACHE_VAL(cf_cv_decl_localzone,[
	AC_TRY_COMPILE([$cf_decl],
		[long x = localzone],
		[cf_cv_decl_localzone=yes],
		[cf_cv_decl_localzone=no])])
AC_MSG_RESULT($cf_cv_decl_localzone)
test $cf_cv_decl_localzone = yes && AC_DEFINE(LOCALZONE_DECLARED)

AC_MSG_CHECKING(for timezone declared)
AC_CACHE_VAL(cf_cv_decl_timezone,[
	AC_TRY_COMPILE($cf_decl,
		[long x = timezone],
		[cf_cv_decl_timezone=yes],
		[cf_cv_decl_timezone=no])])
AC_MSG_RESULT($cf_cv_decl_timezone)
if test $cf_cv_decl_timezone = yes; then
	AC_DEFINE(TIMEZONE_DECLARED)

	AC_MSG_CHECKING(for daylight declared)
	AC_CACHE_VAL(cf_cv_decl_daylight,[
		AC_TRY_COMPILE($cf_decl,
			[long x = daylight],
			[cf_cv_decl_daylight=yes],
			[cf_cv_decl_daylight=no])])
	AC_MSG_RESULT($cf_cv_decl_daylight)
	test $cf_cv_decl_daylight = yes && AC_DEFINE(DAYLIGHT_DECLARED)
fi

AC_MSG_CHECKING(for .tm_gmtoff in struct tm)
AC_CACHE_VAL(cf_cv_tm_gmtoff_decl,[
	AC_TRY_COMPILE([$cf_decl],
		[struct tm tm; tm.tm_gmtoff],
		[cf_cv_tm_gmtoff_decl=yes],
		[cf_cv_tm_gmtoff_decl=no])])
AC_MSG_RESULT($cf_cv_tm_gmtoff_decl)
test $cf_cv_tm_gmtoff_decl = yes && AC_DEFINE(HAVE_TM_GMTOFF)

AC_MSG_CHECKING(for .tm_isdst in struct tm)
AC_CACHE_VAL(cf_cv_tm_isdst_decl,[
	AC_TRY_COMPILE([$cf_decl],
		[struct tm tm; tm.tm_isdst],
		[cf_cv_tm_isdst_decl=yes],
		[cf_cv_tm_isdst_decl=no])])
AC_MSG_RESULT($cf_cv_tm_isdst_decl)
test $cf_cv_tm_isdst_decl = yes && AC_DEFINE(HAVE_TM_ISDST)

AC_MSG_CHECKING(for .tm_zone in struct tm)
AC_CACHE_VAL(cf_cv_tm_zone_decl,[
	AC_TRY_COMPILE([$cf_decl],
		[struct tm tm; tm.tm_zone],
		[cf_cv_tm_zone_decl=yes],
		[cf_cv_tm_zone_decl=no])])
AC_MSG_RESULT($cf_cv_tm_zone_decl)
test $cf_cv_tm_zone_decl = yes && AC_DEFINE(HAVE_TM_ZONE)
])dnl
dnl ---------------------------------------------------------------------------
dnl	Combines AC_HAVE_FUNCS logic with additional test from Kevin Buettner
dnl	that checks to see if we need a prototype for the given function.
dnl
dnl The prototype-check is only turned on when we're configuring to compile
dnl with warnings.
dnl
AC_DEFUN([CF_HAVE_FUNCS],
[
cf_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS -Iinclude"
for cf_func in $1
do
CF_UPPER(cf_tr_func,$cf_func)
AC_MSG_CHECKING(for ${cf_func})
CF_MSG_LOG(${cf_func})
AC_CACHE_VAL(cf_cv_func_$cf_func,[
	# Test for prior functional test (e.g., vfork).
eval cf_result='$ac_cv_func_'$cf_func
if test ".$cf_result" != ".no"; then
	# GCC won't reliably fail on prototype mismatches unless we make all
	# warnings into errors.  Of course, _this_ assumes that the config is
	# otherwise ok.
AC_TRY_LINK([#undef ${cf_func}],[${cf_func}();],[
if test $WithPrototypes != yes; then
	cf_result=yes
elif test $WithWarnings = yes; then
	if test $ac_cv_prog_gcc = yes; then
		CFLAGS="$CFLAGS -Werror"
	fi
	AC_TRY_COMPILE([
#undef  HAVE_${cf_tr_func}
#define HAVE_${cf_tr_func} 1
#include <td_local.h>],
[
#undef $cf_func
	struct zowie { int a; double b; struct zowie *c; char d; };
	extern struct zowie *$cf_func(); $cf_func() ],
	[cf_result=undeclared],
	[cf_result=declared])
else
	cf_result=yes
fi
],
[cf_result=no])
fi
eval 'cf_cv_func_'$cf_func'=$cf_result'
])
# use the computed/retrieved cache-value:
eval 'cf_result=$cf_cv_func_'$cf_func
AC_MSG_RESULT($cf_result)
if test $cf_result != no; then
	AC_DEFINE_UNQUOTED(HAVE_${cf_tr_func})
	if test $cf_result = undeclared; then
		AC_DEFINE_UNQUOTED(NEED_${cf_tr_func})
	fi
fi
done
CFLAGS="$cf_CFLAGS"
])dnl
dnl ---------------------------------------------------------------------------
dnl	Adds to the include-path
dnl
dnl	Autoconf 1.11 should have provided a way to add include path options to
dnl	the cpp-tests.
dnl
AC_DEFUN([CF_INCLUDE_PATH],
[
for cf_path in $1
do
	cf_result="no"
	AC_MSG_CHECKING(for directory $cf_path)
	if test -d $cf_path
	then
		INCLUDES="$INCLUDES -I$cf_path"
		ac_cpp="${ac_cpp} -I$cf_path"
		CFLAGS="$CFLAGS -I$cf_path"
		cf_result="yes"
		case $cf_path in
		/usr/include|/usr/include/*)
			;;
		*)
changequote(,)dnl
			cf_temp=`echo $cf_path | sed -e s'%/[^/]*$%%'`
changequote([,])dnl
			case $cf_temp in
			*/include)
				INCLUDES="$INCLUDES -I$cf_temp"
				ac_cpp="${ac_cpp} -I$cf_temp"
				CFLAGS="$CFLAGS -I$cf_temp"
				;;
			esac
		esac
	fi
	AC_MSG_RESULT($cf_result)
done
])dnl
dnl ---------------------------------------------------------------------------
dnl Some 'make' programs support $(MAKEFLAGS), some $(MFLAGS), to pass 'make'
dnl options to lower-levels.  It's very useful for "make -n" -- if we have it.
dnl (GNU 'make' does both :-)
AC_DEFUN([CF_MAKEFLAGS],
[
AC_MSG_CHECKING([for makeflags variable])
AC_CACHE_VAL(cf_cv_makeflags,[
	cf_cv_makeflags=''
	for cf_option in '$(MFLAGS)' '-$(MAKEFLAGS)'
	do
		cat >cf_makeflags.tmp <<CF_EOF
all :
	echo '.$cf_option'
CF_EOF
		set cf_result=`${MAKE-make} -f cf_makeflags.tmp 2>/dev/null`
		if test "$cf_result" != "."
		then
			cf_cv_makeflags=$cf_option
			break
		fi
	done
	rm -f cf_makeflags.tmp])
AC_MSG_RESULT($cf_cv_makeflags)
AC_SUBST(cf_cv_makeflags)
])dnl
dnl ---------------------------------------------------------------------------
dnl Check for the use of 'include' in 'make' (BSDI is a special case)
dnl The symbol $ac_make is set in AC_MAKE_SET, as a side-effect.
AC_DEFUN([CF_MAKE_INCLUDE],
[
AC_MSG_CHECKING(for style of include in makefiles)

make_include_left=""
make_include_right=""
make_include_quote="unknown"

cf_inc=head$$
cf_dir=subd$$
echo 'RESULT=OK' >$cf_inc
mkdir $cf_dir

for cf_include in "include" ".include" "!include"
do
	for cf_quote in '' '"'
	do
		cat >$cf_dir/makefile <<CF_EOF
SHELL=/bin/sh
${cf_include} ${cf_quote}../$cf_inc${cf_quote}
all :
	@echo 'cf_make_include=\$(RESULT)'
CF_EOF
	cf_make_include=""
	eval `cd $cf_dir && ${MAKE-make} 2>&AC_FD_CC | grep cf_make_include=OK`
	if test -n "$cf_make_include"; then
		make_include_left="$cf_include"
		make_include_quote="$cf_quote"
		break
	else
		echo Tried 1>&AC_FD_CC
		cat $cf_dir/makefile 1>&AC_FD_CC
	fi
	done
	test -n "$cf_make_include" && break
done

rm -rf $cf_inc $cf_dir

if test -z "$make_include_left" ; then
	AC_ERROR(Your $ac_make program does not support includes)
fi
if test ".$make_include_quote" != .unknown ; then
	make_include_left="$make_include_left $make_include_quote"
	make_include_right="$make_include_quote"
fi

AC_MSG_RESULT(${make_include_left}file${make_include_right})

AC_SUBST(make_include_left)
AC_SUBST(make_include_right)
])dnl
dnl ---------------------------------------------------------------------------
dnl Write a debug message to config.log, along with the line number in the
dnl configure script.
AC_DEFUN([CF_MSG_LOG],[
echo "(line __oline__) testing $* ..." 1>&AC_FD_CC
])dnl
dnl ---------------------------------------------------------------------------
dnl Within AC_OUTPUT, check if the given file differs from the target, and
dnl update it if so.  Otherwise, remove the generated file.
dnl
dnl Parameters:
dnl $1 = input, which configure has done substitutions upon
dnl $2 = target file
dnl
AC_DEFUN([CF_OUTPUT_IF_CHANGED],[
if ( cmp -s $1 $2 2>/dev/null )
then
	echo "$2 is unchanged"
	rm -f $1
else
	echo "creating $2"
	rm -f $2
	mv $1 $2
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for one or more programs given by name along the user's path, and
dnl sets a variable to the program's full-path if found.
AC_DEFUN([CF_PROGRAM_FULLPATH],
[
AC_MSG_CHECKING(full path of $1)
AC_CACHE_VAL(cf_cv_$1,[
	cf_cv_$1="[$]$1"
	if test -z "[$]cf_cv_$1"; then
		set -- $2;
		while test [$]# != 0; do
			cf_word=[$]1
			case [$]1 in
			-*)
				;;
			*)
				if test -f "$cf_word" && test ! -f "./$cf_word" && test -x "$cf_word"; then
					cf_cv_$1="$cf_word"
				else
					IFS="${IFS= 	}"; cf_save_ifs="$IFS"; IFS="${IFS}:"
					for cf_dir in $PATH; do
						test -z "$cf_dir" && cf_dir=.
						if test "$cf_dir" != "." && test -f $cf_dir/$cf_word && test -x $cf_dir/$cf_word; then
							cf_cv_$1="$cf_dir/$cf_word"
							break
						fi
					done
					IFS="$cf_save_ifs"
				fi
				if test -n "[$]cf_cv_$1"; then
					shift
					break
				fi
				;;
			esac
			shift
		done
	fi
	# append options, if any
	if test -n "[$]cf_cv_$1"; then
		while test [$]# != 0; do
			case [$]1 in
			-[*]) cf_cv_$1="[$]cf_cv_$1 [$]1";;
			[*])  set -- end;;
			esac
			shift
		done
	fi
])
if test -n "[$]cf_cv_$1"; then
	AC_DEFINE_UNQUOTED($1,"[$]cf_cv_$1")
  AC_MSG_RESULT("[$]cf_cv_$1")
else
  AC_MSG_RESULT((not found))
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl	Tests for a program given by name along the user's path, and sets a
dnl	variable to the program's directory-prefix if found.  Don't match if
dnl	the directory is ".", since we need an absolute path-reference.
AC_DEFUN([CF_PROGRAM_PREFIX],
[
# Extract the first word of `$2', so it can be a program name with args.
set cf_dummy $2; cf_word=[$]2
AC_MSG_CHECKING(for $cf_word prefix ($1))
AC_CACHE_VAL([cf_cv_$1],[
	# allow import from environment-variable
	cf_cv_$1="[$]$1"
	if test -z "[$]cf_cv_$1"; then
		IFS="${IFS= 	}"; cf_save_ifs="$IFS"; IFS="${IFS}:"
		for cf_dir in $PATH; do
			test -z "$cf_dir" && cf_dir=.
			if test "$cf_dir" != "." && test -f $cf_dir/$cf_word && test -x $cf_dir/$cf_word; then
				cf_cv_$1="$cf_dir"
				break
			fi
		done
		IFS="$cf_save_ifs"
	fi
	ifelse([$3],,, [test -z "[$]cf_cv_$1" && cf_cv_$1="$3"])
])
if test -n "[$]cf_cv_$1"; then
  AC_DEFINE_UNQUOTED($1,"[$]cf_cv_$1")
  AC_MSG_RESULT("[$]cf_cv_$1")
else
  AC_MSG_RESULT((not found))
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for the ensemble of programs that are used in RCS, SCCS, VCS, CVS.
dnl We'll have to assume that the related utilities all reside in the same
dnl directory.
AC_DEFUN([CF_RCS_SCCS],
[CF_PROGRAM_PREFIX(RCS_PATH, rcs)
CF_PROGRAM_PREFIX(SCCS_PATH, admin)dnl the SCCS tool
CF_PROGRAM_PREFIX(VCS_PATH, vcs)dnl VCS is my RCS application
CF_PROGRAM_PREFIX(CVS_PATH, cvs)dnl CVS is a layer above RCS
CF_PROGRAM_PREFIX(CMV_PATH, cmv)dnl CmVision combines RCS and SCCS archives
])dnl
dnl ---------------------------------------------------------------------------
dnl Test for the presence of regcmp/regex functions.
AC_DEFUN([CF_REGCMP_FUNCS],
[
AC_HAVE_HEADERS(pw.h libgen.h)
AC_MSG_CHECKING(regcmp/regex functions)
AC_CACHE_VAL(cf_cv_REGCMP_func,[
	CF_REGCMP_LIBS
	if test $cf_cv_REGCMP_func = no; then
		cf_save_LIBS="$LIBS"
		LIBS="-lPW $LIBS"
		CF_REGCMP_LIBS
		LIBS="${cf_save_LIBS}"
		test $cf_cv_REGCMP_func = yes && cf_cv_REGCMP_func="yes-lPW"
	fi
])
AC_MSG_RESULT($cf_cv_REGCMP_func)
if test $cf_cv_REGCMP_func != no; then
	AC_DEFINE(HAVE_REGCMP_FUNCS)
	test $cf_cv_REGCMP_func != yes && AC_CHECK_LIB(PW, regcmp)
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
AC_DEFUN([CF_REGCMP_LIBS],
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
	[cf_cv_REGCMP_func=yes],
	[cf_cv_REGCMP_func=no],
	[cf_cv_REGCMP_func=unknown])
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for the <regexpr.h> include-file, and the functions associated with it.
AC_DEFUN([CF_REGEXPR_H_FUNCS],
[
cf_save_libs="$LIBS"
AC_CHECK_LIB(gen, compile)
AC_MSG_CHECKING(compile/step functions)
AC_CACHE_VAL(cf_cv_REGEXPR_H,[
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
		[cf_cv_REGEXPR_H=yes],
		[cf_cv_REGEXPR_H=no],
		[cf_cv_REGEXPR_H=unknown])
	])
AC_MSG_RESULT($cf_cv_REGEXPR_H)
if test $cf_cv_REGEXPR_H = yes; then
	AC_DEFINE(HAVE_REGEXPR_H_FUNCS)
else
	LIBS="$cf_save_libs"
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for the <regex.h> include-file, and the functions associated with it.
AC_DEFUN([CF_REGEX_H_FUNCS],
[
AC_MSG_CHECKING(regcomp/regexec functions)
AC_CACHE_VAL(cf_cv_REGEX_H,[
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
		[cf_cv_REGEX_H=yes],
		[cf_cv_REGEX_H=no],
		[cf_cv_REGEX_H=unknown])
	])
AC_MSG_RESULT($cf_cv_REGEX_H)
test $cf_cv_REGEX_H = yes && AC_DEFINE(HAVE_REGEX_H_FUNCS)
])dnl
dnl ---------------------------------------------------------------------------
dnl Tests for the presence of re_comp/re_exec functions (no include-file?)
AC_DEFUN([CF_RE_COMP_FUNCS],
[
AC_MSG_CHECKING(re_comp/re_exec functions)
AC_CACHE_VAL(cf_cv_RE_COMP_func,[
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
	[cf_cv_RE_COMP_func=yes],
	[cf_cv_RE_COMP_func=no],
	[cf_cv_RE_COMP_func=unknown])
	])
AC_MSG_RESULT($cf_cv_RE_COMP_func)
test $cf_cv_RE_COMP_func = yes && AC_DEFINE(HAVE_RE_COMP_FUNCS)
])dnl
dnl ---------------------------------------------------------------------------
dnl This bypasses the normal autoconf process because we're generating an
dnl arbitrary number of NEED_xxxx definitions with the CF_HAVE_FUNCS macro. 
dnl Rather than populate an aclocal.h file with all of those definitions, we do
dnl it here.
dnl
dnl Parameters:
dnl $1 = input, which configure has done substitutions upon
dnl $2 = target file
dnl
AC_DEFUN([CF_SED_CONFIG_H],[
changequote({,})dnl
{
cf_config_h=conf$$
rm -f $cf_config_h
echo '/* generated by configure-script */' >$cf_config_h
grep -v '^ -D' config_h >>$cf_config_h
sed	-e '/^ -D/!d' \
	-e '/^# /d' \
	-e 's/ -D/\
#define /g' \
	-e 's/\(#define [A-Za-z_][A-Za-z0-9_]*\)=/\1	/g' \
	-e 's@\\@@g' \
	$1 | sort >>$cf_config_h
}
changequote([,])dnl
CF_OUTPUT_IF_CHANGED($cf_config_h,$2)
rm -f $cf_config_h
])dnl
dnl ---------------------------------------------------------------------------
dnl	On both Ultrix and CLIX, I find size_t defined in <stdio.h>
AC_DEFUN([CF_SIZE_T],
[
AC_MSG_CHECKING(for size_t in <sys/types.h> or <stdio.h>)
AC_CACHE_VAL(cf_cv_type_size_t,[
	AC_TRY_COMPILE([
#include <sys/types.h>
#include <stdio.h>],
		[size_t x],
		[cf_cv_type_size_t=yes],
		[cf_cv_type_size_t=no])
	])
AC_MSG_RESULT($cf_cv_type_size_t)
test $cf_cv_type_size_t = no && AC_DEFINE(size_t, unsigned)
])dnl
dnl ---------------------------------------------------------------------------
dnl Test if the <sys/stat.h> 'stat' struct defines 'st_blocks' member.
dnl If not, assume it's some non-BSD system.
AC_DEFUN([CF_STAT_ST_BLOCKS],
[
AC_MSG_CHECKING(for .st_blocks in struct stat)
AC_CACHE_VAL(cf_cv_st_blocks,[
	AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/stat.h>],
		[int t() {struct stat sb; return sb.st_blocks;}],
		[cf_cv_st_blocks=yes],
		[cf_cv_st_blocks=no])])
AC_MSG_RESULT($cf_cv_st_blocks)
test $cf_cv_st_blocks = yes && AC_DEFINE(STAT_HAS_ST_BLOCKS)
])dnl
dnl ---------------------------------------------------------------------------
dnl	Remove "-g" option from the compiler options
AC_DEFUN([CF_STRIP_G_OPT],
[$1=`echo ${$1} | sed -e 's/-g //' -e 's/-g$//'`])dnl
dnl ---------------------------------------------------------------------------
dnl Test if curses defines 'struct screen'.
dnl
dnl	If this isn't defined, we cannot build a lint library that will check
dnl	for that type, since it isn't resolved.
dnl
AC_DEFUN([CF_STRUCT_SCREEN],
[
AC_MSG_CHECKING(if curses uses struct screen)
AC_CACHE_VAL(cf_cv_have_struct_screen,[
	AC_TRY_COMPILE([
#define lint	/* sysvr4 has its own fallback for lint */
#include <curses.h>],
		[struct screen dummy],
		[cf_cv_have_struct_screen=yes],
		[cf_cv_have_struct_screen=no])
	])
AC_MSG_RESULT($cf_cv_have_struct_screen)

AC_MSG_CHECKING(for definition of struct screen)
AC_CACHE_VAL(cf_cv_need_struct_screen,[
	if test $cf_cv_have_struct_screen = yes; then
		AC_TRY_COMPILE([#include <curses.h>],
			[struct screen { int dummy;}],
			[cf_cv_need_struct_screen=yes],
			[cf_cv_need_struct_screen=no])
	else
		cf_cv_need_struct_screen=no
	fi
	])
AC_MSG_RESULT($cf_cv_need_struct_screen)
test $cf_cv_need_struct_screen = yes && AC_DEFINE(NEED_STRUCT_SCREEN)
])dnl
dnl ---------------------------------------------------------------------------
dnl Check for declaration of sys_errlist in one of stdio.h and errno.h.
dnl Declaration of sys_errlist on BSD4.4 interferes with our declaration.
dnl Reported by Keith Bostic.
AC_DEFUN([CF_SYS_ERRLIST],
[
AC_MSG_CHECKING([declaration of sys_errlist])
AC_CACHE_VAL(cf_cv_dcl_sys_errlist,[
    AC_TRY_COMPILE([
#include <stdio.h>
#include <sys/types.h>
#include <errno.h> ],
    [char *c = (char *) *sys_errlist],
    [cf_cv_dcl_sys_errlist=yes],
    [cf_cv_dcl_sys_errlist=no])])
AC_MSG_RESULT($cf_cv_dcl_sys_errlist)

# It's possible (for near-UNIX clones) that sys_errlist doesn't exist
if test $cf_cv_dcl_sys_errlist = no ; then
    AC_DEFINE(DECL_SYS_ERRLIST)
    AC_MSG_CHECKING([existence of sys_errlist])
    AC_CACHE_VAL(cf_cv_have_sys_errlist,[
        AC_TRY_LINK([#include <errno.h>],
            [char *c = (char *) *sys_errlist],
            [cf_cv_have_sys_errlist=yes],
            [cf_cv_have_sys_errlist=no])])
    AC_MSG_RESULT($cf_cv_have_sys_errlist)
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl Test if curses defines KD, KU, etc., for cursor keys
dnl
dnl	I found at least one implementation of curses that didn't declare these
dnl	variables in the include-file, but did define them in the library.  If
dnl	they're not in the include-file, ignore them.  Otherwise, assume that
dnl	curses initializes them in 'initscr()'.
dnl
AC_DEFUN([CF_TCAP_CURSOR],
[
AC_MSG_CHECKING(for termcap-cursor variables)
AC_CACHE_VAL(cf_cv_termcap_cursor,[
	AC_TRY_COMPILE([#include <curses.h>],
		[char *d=KD, *u=KU, *r=KR, *l=KL],
		[cf_cv_termcap_cursor=yes],
		[cf_cv_termcap_cursor=no])])
AC_MSG_RESULT($cf_cv_termcap_cursor)
test $cf_cv_termcap_cursor = yes && AC_DEFINE(HAVE_TCAP_CURSOR)
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
AC_DEFUN([CF_TD_CURSES_LIBS],
[
AC_PROVIDE([$0])
# Check for gpm (general purpose mouse) since we may need this for ncurses.
AC_CHECK_LIB(gpm,Gpm_Open)
if test $WithNcurses = yes; then
	CF_MSG_LOG(ncurses library, by option)
	AC_CHECK_LIB(ncurses,initscr)
else
	# Check if we've already got curses functions in our library list
	# (e.g., if the user is overriding this test by setting $LIBS).
	CF_MSG_LOG(curses library, by default)
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
CF_MSG_LOG(ncurses state $ac_cv_lib_ncurses_initscr)
if test $ac_cv_lib_ncurses_initscr = yes; then
	# Linux installs NCURSES's include files in a separate directory to avoid
	# confusion with the native curses.  NCURSES has its own termcap support.
	CF_INCLUDE_PATH(/usr/include/ncurses /usr/local/include/ncurses)
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
CF_MSG_LOG(curses-type BSD/SYSV)
AC_CACHE_VAL(cf_cv_curses_type,[
	cf_cv_curses_type=unknown
	if test .$ac_cv_lib_curses_initscr = .yes; then
		AC_TRY_LINK([#include <curses.h>],
			[keypad(curscr,1)],
			[cf_cv_curses_type=sysv],
			[cf_cv_curses_type=bsd])
	fi
])
AC_MSG_RESULT($cf_cv_curses_type)
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
AC_DEFUN([CF_TD_SIG_ARGS],
[
AC_REQUIRE([AC_TYPE_SIGNAL])
AC_MSG_CHECKING(for non-standard signal handler prototype)
AC_CACHE_VAL(cf_cv_sig_args,[
	AC_TRY_RUN([
#include <signal.h>
		RETSIGTYPE (*signal(int sig, RETSIGTYPE(*func)(int sig)))(int sig2);
		RETSIGTYPE catch(int sig) { exit(1); }
		main() { signal(SIGINT, catch); exit(0); } ],
		[cf_cv_sig_args=STANDARD],
		[AC_TRY_RUN([
#include <signal.h>
			RETSIGTYPE (*signal(int sig, RETSIGTYPE(*func)(int sig,...)))(int sig2,...);
			RETSIGTYPE catch(int sig, ...) { exit(1); }
			main() { signal(SIGINT, catch); exit(0); } ],
			[cf_cv_sig_args=VARYING],
			[cf_cv_sig_args=UNKNOWN],
			[cf_cv_sig_args=UNKNOWN])
		],
		[cf_cv_sig_args=UNKNOWN])
	])
AC_MSG_RESULT($cf_cv_sig_args)
AC_DEFINE_UNQUOTED(SIG_ARGS_$cf_cv_sig_args)

if test -n "$GCC" && test $WithWarnings = yes
then
	AC_MSG_CHECKING(redefinable signal handler prototype)
	AC_CACHE_VAL(cf_cv_sigs_redef,[
		cf_cv_sigs_redef=no
		# We're checking the definitions of the commonly-used predefined signal
		# macros, to see if their values are the ones that we expect.  If so,
		# we'll plug in our own definitions, that have complete prototypes.  We
		# do this when we're developing with gcc, with all warnings, and
		# shouldn't do it for other compilers, since (for example) the IRIX
		# compiler complains a lot.
		if test $cf_cv_sig_args != UNKNOWN; then	# we have prototypes
			AC_TRY_RUN([
#include <signal.h>
#undef  NOT
#define NOT(s,d) ((long)(s) != (long)(d))
				main() { exit(NOT(SIG_IGN,1) || NOT(SIG_DFL,0) || NOT(SIG_ERR,-1)); } ],
				[cf_cv_sigs_redef=yes],
				[cf_cv_sigs_redef=no],
				[cf_cv_sigs_redef=unknown])
		fi
		])
		AC_MSG_RESULT($cf_cv_sigs_redef)
		test $cf_cv_sigs_redef = yes && AC_DEFINE(SIG_IGN_REDEFINEABLE)
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl Make an uppercase version of a variable
dnl $1=uppercase($2)
AC_DEFUN([CF_UPPER],
[
changequote(,)dnl
$1=`echo $2 | tr '[a-z]' '[A-Z]'`
changequote([,])dnl
])dnl
dnl ---------------------------------------------------------------------------
dnl Check for ANSI stdarg.h vs varargs.h.  Note that some systems include
dnl <varargs.h> within <stdarg.h>.
AC_DEFUN([CF_VARARGS],
[
AC_MSG_CHECKING(for standard varargs)
AC_CACHE_VAL(cf_cv_ansi_varargs,[
	AC_TRY_COMPILE([
#if HAVE_STDARG_H
#include <stdarg.h>
#else
#if HAVE_VARARGS_H
#include <varargs.h>
#endif
#endif
		],
		[return 0;} int foo(char *fmt,...){va_list args;va_start(args,fmt);va_end(args)],
		[cf_cv_ansi_varargs=yes],
		[cf_cv_ansi_varargs=no])
	])
AC_MSG_RESULT($cf_cv_ansi_varargs)
test $cf_cv_ansi_varargs = yes && AC_DEFINE(ANSI_VARARGS)
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
AC_DEFUN([CF_WAIT],
[
AC_HAVE_HEADERS(wait.h)
AC_HAVE_HEADERS(sys/wait.h)
cf_decl="#include <sys/types.h>
"
if test $ac_cv_header_sys_wait_h = yes; then
cf_decl="$cf_decl
#include <sys/wait.h>
"
elif test $ac_cv_header_wait_h = yes; then
cf_decl="$cf_decl
#include <wait.h>
"
fi
AC_MSG_CHECKING(union wait declared)
AC_CACHE_VAL(cf_cv_decl_union_wait,[
	AC_TRY_COMPILE($cf_decl,
		[union wait x],
		[cf_cv_decl_union_wait=yes],
		[cf_cv_decl_union_wait=no])
	])
AC_MSG_RESULT($cf_cv_decl_union_wait)

if test $cf_cv_decl_union_wait = yes; then
	AC_MSG_CHECKING(union wait used as wait-arg)
	AC_CACHE_VAL(cf_cv_arg_union_wait,[
		AC_TRY_COMPILE($cf_decl,
 			[union wait x; wait(&x)],
			[cf_cv_arg_union_wait=yes],
			[cf_cv_arg_union_wait=no])
		])
	AC_MSG_RESULT($cf_cv_arg_union_wait)
	test $cf_cv_arg_union_wait = yes && AC_DEFINE(WAIT_USES_UNION)
fi
])dnl
