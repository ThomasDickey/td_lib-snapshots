dnl Extended Macros that test for specific features.
dnl $Id: aclocal.m4,v 12.160 2005/01/25 01:32:02 tom Exp $
dnl vi:set ts=4:
dnl ---------------------------------------------------------------------------
dnl BELOW THIS LINE CAN BE PUT INTO "acspecific.m4", by changing "CF_" to "AC_"
dnl and "cf_" to "ac_".
dnl ---------------------------------------------------------------------------
dnl ---------------------------------------------------------------------------
dnl CF_ADD_CFLAGS version: 7 updated: 2004/04/25 17:48:30
dnl -------------
dnl Copy non-preprocessor flags to $CFLAGS, preprocessor flags to $CPPFLAGS
dnl The second parameter if given makes this macro verbose.
dnl
dnl Put any preprocessor definitions that use quoted strings in $EXTRA_CPPFLAGS,
dnl to simplify use of $CPPFLAGS in compiler checks, etc., that are easily
dnl confused by the quotes (which require backslashes to keep them usable).
AC_DEFUN([CF_ADD_CFLAGS],
[
cf_fix_cppflags=no
cf_new_cflags=
cf_new_cppflags=
cf_new_extra_cppflags=

for cf_add_cflags in $1
do
case $cf_fix_cppflags in
no)
	case $cf_add_cflags in #(vi
	-undef|-nostdinc*|-I*|-D*|-U*|-E|-P|-C) #(vi
		case $cf_add_cflags in
		-D*)
			cf_tst_cflags=`echo ${cf_add_cflags} |sed -e 's/^-D[[^=]]*='\''\"[[^"]]*//'`

			test "${cf_add_cflags}" != "${cf_tst_cflags}" \
			&& test -z "${cf_tst_cflags}" \
			&& cf_fix_cppflags=yes

			if test $cf_fix_cppflags = yes ; then
				cf_new_extra_cppflags="$cf_new_extra_cppflags $cf_add_cflags"
				continue
			elif test "${cf_tst_cflags}" = "\"'" ; then
				cf_new_extra_cppflags="$cf_new_extra_cppflags $cf_add_cflags"
				continue
			fi
			;;
		esac
		case "$CPPFLAGS" in
		*$cf_add_cflags) #(vi
			;;
		*) #(vi
			cf_new_cppflags="$cf_new_cppflags $cf_add_cflags"
			;;
		esac
		;;
	*)
		cf_new_cflags="$cf_new_cflags $cf_add_cflags"
		;;
	esac
	;;
yes)
	cf_new_extra_cppflags="$cf_new_extra_cppflags $cf_add_cflags"

	cf_tst_cflags=`echo ${cf_add_cflags} |sed -e 's/^[[^"]]*"'\''//'`

	test "${cf_add_cflags}" != "${cf_tst_cflags}" \
	&& test -z "${cf_tst_cflags}" \
	&& cf_fix_cppflags=no
	;;
esac
done

if test -n "$cf_new_cflags" ; then
	ifelse($2,,,[CF_VERBOSE(add to \$CFLAGS $cf_new_cflags)])
	CFLAGS="$CFLAGS $cf_new_cflags"
fi

if test -n "$cf_new_cppflags" ; then
	ifelse($2,,,[CF_VERBOSE(add to \$CPPFLAGS $cf_new_cppflags)])
	CPPFLAGS="$cf_new_cppflags $CPPFLAGS"
fi

if test -n "$cf_new_extra_cppflags" ; then
	ifelse($2,,,[CF_VERBOSE(add to \$EXTRA_CPPFLAGS $cf_new_extra_cppflags)])
	EXTRA_CPPFLAGS="$cf_new_extra_cppflags $EXTRA_CPPFLAGS"
fi

AC_SUBST(EXTRA_CPPFLAGS)

])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ADD_INCDIR version: 4 updated: 2002/12/21 14:25:52
dnl -------------
dnl Add an include-directory to $CPPFLAGS.  Don't add /usr/include, since it's
dnl redundant.  We don't normally need to add -I/usr/local/include for gcc,
dnl but old versions (and some misinstalled ones) need that.  To make things
dnl worse, gcc 3.x gives error messages if -I/usr/local/include is added to
dnl the include-path).
AC_DEFUN([CF_ADD_INCDIR],
[
for cf_add_incdir in $1
do
	while true
	do
		case $cf_add_incdir in
		/usr/include) # (vi
			;;
		/usr/local/include) # (vi
			if test "$GCC" = yes
			then
				cf_save_CPPFLAGS="$CPPFLAGS"
				CPPFLAGS="$CPPFLAGS -I$cf_add_incdir"
				AC_TRY_COMPILE([#include <stdio.h>],
						[printf("Hello")],
						[],
						[CPPFLAGS="$cf_save_CPPFLAGS"])
			fi
			;;
		*) # (vi
			CPPFLAGS="$CPPFLAGS -I$cf_add_incdir"
			;;
		esac
		cf_top_incdir=`echo $cf_add_incdir | sed -e 's%/include/.*$%/include%'`
		test "$cf_top_incdir" = "$cf_add_incdir" && break
		cf_add_incdir="$cf_top_incdir"
	done
done
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ANSI_CC_CHECK version: 9 updated: 2001/12/30 17:53:34
dnl ----------------
dnl This is adapted from the macros 'fp_PROG_CC_STDC' and 'fp_C_PROTOTYPES'
dnl in the sharutils 4.2 distribution.
AC_DEFUN([CF_ANSI_CC_CHECK],
[
AC_CACHE_CHECK(for ${CC-cc} option to accept ANSI C, cf_cv_ansi_cc,[
cf_cv_ansi_cc=no
cf_save_CFLAGS="$CFLAGS"
cf_save_CPPFLAGS="$CPPFLAGS"
# Don't try gcc -ansi; that turns off useful extensions and
# breaks some systems' header files.
# AIX			-qlanglvl=ansi
# Ultrix and OSF/1	-std1
# HP-UX			-Aa -D_HPUX_SOURCE
# SVR4			-Xc
# UnixWare 1.2		(cannot use -Xc, since ANSI/POSIX clashes)
for cf_arg in "-DCC_HAS_PROTOS" \
	"" \
	-qlanglvl=ansi \
	-std1 \
	-Ae \
	"-Aa -D_HPUX_SOURCE" \
	-Xc
do
	CF_ADD_CFLAGS($cf_arg)
	AC_TRY_COMPILE(
[
#ifndef CC_HAS_PROTOS
#if !defined(__STDC__) || (__STDC__ != 1)
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
CPPFLAGS="$cf_save_CPPFLAGS"
])

if test "$cf_cv_ansi_cc" != "no"; then
if test ".$cf_cv_ansi_cc" != ".-DCC_HAS_PROTOS"; then
	CF_ADD_CFLAGS($cf_cv_ansi_cc)
else
	AC_DEFINE(CC_HAS_PROTOS)
fi
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ANSI_CC_REQD version: 3 updated: 1997/09/06 13:40:44
dnl ---------------
dnl For programs that must use an ANSI compiler, obtain compiler options that
dnl will make it recognize prototypes.  We'll do preprocessor checks in other
dnl macros, since tools such as unproto can fake prototypes, but only part of
dnl the preprocessor.
AC_DEFUN([CF_ANSI_CC_REQD],
[AC_REQUIRE([CF_ANSI_CC_CHECK])
if test "$cf_cv_ansi_cc" = "no"; then
	AC_ERROR(
[Your compiler does not appear to recognize prototypes.
You have the following choices:
	a. adjust your compiler options
	b. get an up-to-date compiler
	c. use a wrapper such as unproto])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ANSI_CPP version: 2 updated: 1997/09/07 15:13:55
dnl -----------
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
dnl CF_ARG_DISABLE version: 3 updated: 1999/03/30 17:24:31
dnl --------------
dnl Allow user to disable a normally-on option.
AC_DEFUN([CF_ARG_DISABLE],
[CF_ARG_OPTION($1,[$2],[$3],[$4],yes)])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ARG_OPTION version: 3 updated: 1997/10/18 14:42:41
dnl -------------
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
dnl CF_CHECK_CACHE version: 10 updated: 2004/05/23 13:03:31
dnl --------------
dnl Check if we're accidentally using a cache from a different machine.
dnl Derive the system name, as a check for reusing the autoconf cache.
dnl
dnl If we've packaged config.guess and config.sub, run that (since it does a
dnl better job than uname).  Normally we'll use AC_CANONICAL_HOST, but allow
dnl an extra parameter that we may override, e.g., for AC_CANONICAL_SYSTEM
dnl which is useful in cross-compiles.
dnl
dnl Note: we would use $ac_config_sub, but that is one of the places where
dnl autoconf 2.5x broke compatibility with autoconf 2.13
AC_DEFUN([CF_CHECK_CACHE],
[
if test -f $srcdir/config.guess || test -f $ac_aux_dir/config.guess ; then
	ifelse([$1],,[AC_CANONICAL_HOST],[$1])
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
test -n "$cf_cv_system_name" && AC_MSG_RESULT(Configuring for $cf_cv_system_name)

if test ".$system_name" != ".$cf_cv_system_name" ; then
	AC_MSG_RESULT(Cached system name ($system_name) does not agree with actual ($cf_cv_system_name))
	AC_ERROR("Please remove config.cache and try again.")
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CHECK_ERRNO version: 9 updated: 2001/12/30 18:03:23
dnl --------------
dnl Check for data that is usually declared in <stdio.h> or <errno.h>, e.g.,
dnl the 'errno' variable.  Define a DECL_xxx symbol if we must declare it
dnl ourselves.
dnl
dnl $1 = the name to check
AC_DEFUN([CF_CHECK_ERRNO],
[
AC_CACHE_CHECK(if external $1 is declared, cf_cv_dcl_$1,[
    AC_TRY_COMPILE([
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <stdio.h>
#include <sys/types.h>
#include <errno.h> ],
    [long x = (long) $1],
    [cf_cv_dcl_$1=yes],
    [cf_cv_dcl_$1=no])
])

if test "$cf_cv_dcl_$1" = no ; then
    CF_UPPER(cf_result,decl_$1)
    AC_DEFINE_UNQUOTED($cf_result)
fi

# It's possible (for near-UNIX clones) that the data doesn't exist
CF_CHECK_EXTERN_DATA($1,int)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CHECK_EXTERN_DATA version: 3 updated: 2001/12/30 18:03:23
dnl --------------------
dnl Check for existence of external data in the current set of libraries.  If
dnl we can modify it, it's real enough.
dnl $1 = the name to check
dnl $2 = its type
AC_DEFUN([CF_CHECK_EXTERN_DATA],
[
AC_CACHE_CHECK(if external $1 exists, cf_cv_have_$1,[
    AC_TRY_LINK([
#undef $1
extern $2 $1;
],
    [$1 = 2],
    [cf_cv_have_$1=yes],
    [cf_cv_have_$1=no])
])

if test "$cf_cv_have_$1" = yes ; then
    CF_UPPER(cf_result,have_$1)
    AC_DEFINE_UNQUOTED($cf_result)
fi

])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CHECK_REGEX version: 2 updated: 1997/09/07 17:43:58
dnl --------------
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
dnl CF_CURSES_CCHAR_T version: 3 updated: 2003/11/06 19:59:57
dnl -----------------
dnl Test if curses defines 'cchar_t' (usually a 'long' type for SysV curses).
AC_DEFUN([CF_CURSES_CCHAR_T],
[
AC_REQUIRE([CF_CURSES_CPPFLAGS])dnl
AC_CACHE_CHECK(for cchar_t typedef,cf_cv_cchar_t_decl,[
	AC_TRY_COMPILE([#include <${cf_cv_ncurses_header-curses.h}>],
		[cchar_t foo],
		[cf_cv_cchar_t_decl=yes],
		[cf_cv_cchar_t_decl=no])])
if test $cf_cv_cchar_t_decl = yes ; then
	AC_DEFINE(HAVE_TYPE_CCHAR_T)
	AC_CACHE_CHECK(if cchar_t is scalar or struct,cf_cv_cchar_t_type,[
		AC_TRY_COMPILE([#include <${cf_cv_ncurses_header-curses.h}>],
			[cchar_t foo; long x = foo],
			[cf_cv_cchar_t_type=scalar],
			[cf_cv_cchar_t_type=struct])])
	if test $cf_cv_cchar_t_type = scalar ; then
		AC_DEFINE(TYPE_CCHAR_T_IS_SCALAR)
	fi
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CURSES_CHTYPE version: 6 updated: 2003/11/06 19:59:57
dnl ----------------
dnl Test if curses defines 'chtype' (usually a 'long' type for SysV curses).
AC_DEFUN([CF_CURSES_CHTYPE],
[
AC_REQUIRE([CF_CURSES_CPPFLAGS])dnl
AC_CACHE_CHECK(for chtype typedef,cf_cv_chtype_decl,[
	AC_TRY_COMPILE([#include <${cf_cv_ncurses_header-curses.h}>],
		[chtype foo],
		[cf_cv_chtype_decl=yes],
		[cf_cv_chtype_decl=no])])
if test $cf_cv_chtype_decl = yes ; then
	AC_DEFINE(HAVE_TYPE_CHTYPE)
	AC_CACHE_CHECK(if chtype is scalar or struct,cf_cv_chtype_type,[
		AC_TRY_COMPILE([#include <${cf_cv_ncurses_header-curses.h}>],
			[chtype foo; long x = foo],
			[cf_cv_chtype_type=scalar],
			[cf_cv_chtype_type=struct])])
	if test $cf_cv_chtype_type = scalar ; then
		AC_DEFINE(TYPE_CHTYPE_IS_SCALAR)
	fi
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CURSES_CPPFLAGS version: 7 updated: 2003/06/06 00:48:41
dnl ------------------
dnl Look for the curses headers.
AC_DEFUN([CF_CURSES_CPPFLAGS],[

AC_CACHE_CHECK(for extra include directories,cf_cv_curses_incdir,[
cf_cv_curses_incdir=no
case $host_os in #(vi
hpux10.*|hpux11.*) #(vi
	test -d /usr/include/curses_colr && \
	cf_cv_curses_incdir="-I/usr/include/curses_colr"
	;;
sunos3*|sunos4*)
	test -d /usr/5lib && \
	test -d /usr/5include && \
	cf_cv_curses_incdir="-I/usr/5include"
	;;
esac
])
test "$cf_cv_curses_incdir" != no && CPPFLAGS="$cf_cv_curses_incdir $CPPFLAGS"

AC_CACHE_CHECK(if we have identified curses headers,cf_cv_ncurses_header,[
cf_cv_ncurses_header=none
for cf_header in \
	curses.h \
	ncurses.h \
	ncurses/curses.h \
	ncurses/ncurses.h
do
AC_TRY_COMPILE([#include <${cf_header}>],
	[initscr(); tgoto("?", 0,0)],
	[cf_cv_ncurses_header=$cf_header; break],[])
done
])

if test "$cf_cv_ncurses_header" = none ; then
	AC_MSG_ERROR(No curses header-files found)
fi

# cheat, to get the right #define's for HAVE_NCURSES_H, etc.
AC_CHECK_HEADERS($cf_cv_ncurses_header)

])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CURSES_DATA version: 3 updated: 1998/07/01 09:41:58
dnl --------------
dnl Test for curses data/types
AC_DEFUN([CF_CURSES_DATA],
[
CF_CURSES_CCHAR_T
CF_CURSES_CHTYPE
CF_CURSES_STYLE
CF_STRUCT_SCREEN
CF_TCAP_CURSOR
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CURSES_FUNCS version: 12 updated: 2003/11/06 19:59:57
dnl ---------------
dnl Curses-functions are a little complicated, since a lot of them are macros.
AC_DEFUN([CF_CURSES_FUNCS],
[
AC_REQUIRE([CF_CURSES_CPPFLAGS])dnl
AC_REQUIRE([CF_XOPEN_CURSES])
AC_REQUIRE([CF_CURSES_TERM_H])
for cf_func in $1
do
	CF_UPPER(cf_tr_func,$cf_func)
	AC_MSG_CHECKING(for ${cf_func})
	CF_MSG_LOG(${cf_func})
	AC_CACHE_VAL(cf_cv_func_$cf_func,[
		eval cf_result='$ac_cv_func_'$cf_func
		if test ".$cf_result" != ".no"; then
			AC_TRY_LINK([
#ifdef HAVE_XCURSES
#include <xcurses.h>
char * XCursesProgramName = "test";
#else
#include <${cf_cv_ncurses_header-curses.h}>
#if defined(NCURSES_VERSION) && defined(HAVE_NCURSESW_TERM_H)
#include <ncursesw/term.h>
#else
#if defined(NCURSES_VERSION) && defined(HAVE_NCURSES_TERM_H)
#include <ncurses/term.h>
#else
#ifdef HAVE_TERM_H
#include <term.h>
#endif
#endif
#endif
#endif],
			[
#ifndef ${cf_func}
long foo = (long)(&${cf_func});
exit(foo == 0);
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
dnl CF_CURSES_LIBS version: 23 updated: 2003/11/06 19:59:57
dnl --------------
dnl Look for the curses libraries.  Older curses implementations may require
dnl termcap/termlib to be linked as well.  Call CF_CURSES_CPPFLAGS first.
AC_DEFUN([CF_CURSES_LIBS],[

AC_REQUIRE([CF_CURSES_CPPFLAGS])dnl
AC_MSG_CHECKING(if we have identified curses libraries)
AC_TRY_LINK([#include <${cf_cv_ncurses_header-curses.h}>],
	[initscr(); tgoto("?", 0,0)],
	cf_result=yes,
	cf_result=no)
AC_MSG_RESULT($cf_result)

if test "$cf_result" = no ; then
case $host_os in #(vi
freebsd*) #(vi
	AC_CHECK_LIB(mytinfo,tgoto,[LIBS="-lmytinfo $LIBS"])
	;;
hpux10.*|hpux11.*) #(vi
	AC_CHECK_LIB(cur_colr,initscr,[
		LIBS="-lcur_colr $LIBS"
		ac_cv_func_initscr=yes
		],[
	AC_CHECK_LIB(Hcurses,initscr,[
		# HP's header uses __HP_CURSES, but user claims _HP_CURSES.
		LIBS="-lHcurses $LIBS"
		CPPFLAGS="-D__HP_CURSES -D_HP_CURSES $CPPFLAGS"
		ac_cv_func_initscr=yes
		])])
	;;
linux*) # Suse Linux does not follow /usr/lib convention
	LIBS="$LIBS -L/lib"
	;;
sunos3*|sunos4*)
	test -d /usr/5lib && \
	LIBS="$LIBS -L/usr/5lib -lcurses -ltermcap"
	ac_cv_func_initscr=yes
	;;
esac

if test ".$ac_cv_func_initscr" != .yes ; then
	cf_save_LIBS="$LIBS"
	cf_term_lib=""
	cf_curs_lib=""

	if test ".${cf_cv_ncurses_version-no}" != .no
	then
		cf_check_list="ncurses curses cursesX"
	else
		cf_check_list="cursesX curses ncurses"
	fi

	# Check for library containing tgoto.  Do this before curses library
	# because it may be needed to link the test-case for initscr.
	AC_CHECK_FUNC(tgoto,[cf_term_lib=predefined],[
		for cf_term_lib in $cf_check_list termcap termlib unknown
		do
			AC_CHECK_LIB($cf_term_lib,tgoto,[break])
		done
	])

	# Check for library containing initscr
	test "$cf_term_lib" != predefined && test "$cf_term_lib" != unknown && LIBS="-l$cf_term_lib $cf_save_LIBS"
	for cf_curs_lib in $cf_check_list xcurses jcurses unknown
	do
		AC_CHECK_LIB($cf_curs_lib,initscr,[break])
	done
	test $cf_curs_lib = unknown && AC_ERROR(no curses library found)

	LIBS="-l$cf_curs_lib $cf_save_LIBS"
	if test "$cf_term_lib" = unknown ; then
		AC_MSG_CHECKING(if we can link with $cf_curs_lib library)
		AC_TRY_LINK([#include <${cf_cv_ncurses_header-curses.h}>],
			[initscr()],
			[cf_result=yes],
			[cf_result=no])
		AC_MSG_RESULT($cf_result)
		test $cf_result = no && AC_ERROR(Cannot link curses library)
	elif test "$cf_curs_lib" = "$cf_term_lib" ; then
		:
	elif test "$cf_term_lib" != predefined ; then
		AC_MSG_CHECKING(if we need both $cf_curs_lib and $cf_term_lib libraries)
		AC_TRY_LINK([#include <${cf_cv_ncurses_header-curses.h}>],
			[initscr(); tgoto((char *)0, 0, 0);],
			[cf_result=no],
			[
			LIBS="-l$cf_curs_lib -l$cf_term_lib $cf_save_LIBS"
			AC_TRY_LINK([#include <${cf_cv_ncurses_header-curses.h}>],
				[initscr()],
				[cf_result=yes],
				[cf_result=error])
			])
		AC_MSG_RESULT($cf_result)
	fi
fi
fi

])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CURSES_MOUSE version: 3 updated: 2003/11/06 19:59:57
dnl ---------------
dnl Test for the existence of SysVr4 mouse support in curses. If we've not got
dnl it, we'll simulate the interface (for xterm, at least).
AC_DEFUN([CF_CURSES_MOUSE],
[
AC_REQUIRE([CF_CURSES_CPPFLAGS])dnl
AC_MSG_CHECKING(for curses mouse-support)
AC_CACHE_VAL(cf_cv_curses_mouse,[
AC_TRY_LINK([#include <${cf_cv_ncurses_header-curses.h}>],[
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
dnl CF_CURSES_STYLE version: 3 updated: 1997/09/07 18:01:49
dnl ---------------
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
dnl CF_CURSES_TERMCAP version: 10 updated: 2002/01/13 16:02:19
dnl -----------------
dnl Check if we should include <curses.h> to pick up prototypes for termcap
dnl functions.  On terminfo systems, these are normally declared in <curses.h>,
dnl but may be in <term.h>.  We check for termcap.h as an alternate, but it
dnl isn't standard (usually associated with GNU termcap).
dnl
dnl The 'tgoto()' function is declared in both terminfo and termcap.
dnl
dnl See CF_TYPE_OUTCHAR for more details.
AC_DEFUN([CF_CURSES_TERMCAP],
[
AC_REQUIRE([CF_CURSES_TERM_H])
AC_CACHE_CHECK(if we should include curses.h or termcap.h, cf_cv_need_curses_h,[
cf_save_CPPFLAGS="$CPPFLAGS"
cf_cv_need_curses_h=no

for cf_t_opts in "" "NEED_TERMCAP_H"
do
for cf_c_opts in "" "NEED_CURSES_H"
do

    CPPFLAGS="$cf_save_CPPFLAGS $CHECK_DECL_FLAG"
    test -n "$cf_c_opts" && CPPFLAGS="$CPPFLAGS -D$cf_c_opts"
    test -n "$cf_t_opts" && CPPFLAGS="$CPPFLAGS -D$cf_t_opts"

    AC_TRY_LINK([/* $cf_c_opts $cf_t_opts */
$CHECK_DECL_HDRS],
	[char *x = (char *)tgoto("")],
	[test "$cf_cv_need_curses_h" = no && {
	     cf_cv_need_curses_h=maybe
	     cf_ok_c_opts=$cf_c_opts
	     cf_ok_t_opts=$cf_t_opts
	}],
	[echo "Recompiling with corrected call (C:$cf_c_opts, T:$cf_t_opts)" >&AC_FD_CC
	AC_TRY_LINK([
$CHECK_DECL_HDRS],
	[char *x = (char *)tgoto("",0,0)],
	[cf_cv_need_curses_h=yes
	 cf_ok_c_opts=$cf_c_opts
	 cf_ok_t_opts=$cf_t_opts])])

	CPPFLAGS="$cf_save_CPPFLAGS"
	test "$cf_cv_need_curses_h" = yes && break
done
	test "$cf_cv_need_curses_h" = yes && break
done

if test "$cf_cv_need_curses_h" != no ; then
	echo "Curses/termcap test = $cf_cv_need_curses_h (C:$cf_ok_c_opts, T:$cf_ok_t_opts)" >&AC_FD_CC
	if test -n "$cf_ok_c_opts" ; then
		if test -n "$cf_ok_t_opts" ; then
			cf_cv_need_curses_h=both
		else
			cf_cv_need_curses_h=curses.h
		fi
	elif test -n "$cf_ok_t_opts" ; then
		cf_cv_need_curses_h=termcap.h
	elif test "$cf_cv_term_header" != no ; then
		cf_cv_need_curses_h=term.h
	else
		cf_cv_need_curses_h=no
	fi
fi
])

case $cf_cv_need_curses_h in
both) #(vi
	AC_DEFINE_UNQUOTED(NEED_CURSES_H)
	AC_DEFINE_UNQUOTED(NEED_TERMCAP_H)
	;;
curses.h) #(vi
	AC_DEFINE_UNQUOTED(NEED_CURSES_H)
	;;
term.h) #(vi
	AC_DEFINE_UNQUOTED(NEED_TERM_H)
	;;
termcap.h) #(vi
	AC_DEFINE_UNQUOTED(NEED_TERMCAP_H)
	;;
esac

])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CURSES_TERM_H version: 6 updated: 2003/11/06 19:59:57
dnl ----------------
dnl SVr4 curses should have term.h as well (where it puts the definitions of
dnl the low-level interface).  This may not be true in old/broken implementations,
dnl as well as in misconfigured systems (e.g., gcc configured for Solaris 2.4
dnl running with Solaris 2.5.1).
AC_DEFUN([CF_CURSES_TERM_H],
[
AC_CACHE_CHECK(for term.h, cf_cv_term_header,[

AC_REQUIRE([CF_CURSES_CPPFLAGS])dnl
# If we found <ncurses/curses.h>, look for <ncurses/term.h>, but always look
# for <term.h> if we do not find the variant.
for cf_header in \
	`echo ${cf_cv_ncurses_header-curses.h} | sed -e 's%/.*%/%'`term.h \
	term.h
do
	AC_TRY_COMPILE([
#include <${cf_cv_ncurses_header-curses.h}>
#include <${cf_header}>],
	[WINDOW *x],
	[cf_cv_term_header=$cf_header
	 break],
	[cf_cv_term_header=no])
done
])

case $cf_cv_term_header in #(vi
term.h) #(vi
	AC_DEFINE(HAVE_TERM_H)
	;;
ncurses/term.h)
	AC_DEFINE(HAVE_NCURSES_TERM_H)
	;;
ncursesw/term.h)
	AC_DEFINE(HAVE_NCURSESW_TERM_H)
	;;
esac
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_DIRNAME version: 4 updated: 2002/12/21 19:25:52
dnl ----------
dnl "dirname" is not portable, so we fake it with a shell script.
AC_DEFUN([CF_DIRNAME],[$1=`echo $2 | sed -e 's%/[[^/]]*$%%'`])dnl
dnl ---------------------------------------------------------------------------
dnl CF_DISABLE_ECHO version: 10 updated: 2003/04/17 22:27:11
dnl ---------------
dnl You can always use "make -n" to see the actual options, but it's hard to
dnl pick out/analyze warning messages when the compile-line is long.
dnl
dnl Sets:
dnl	ECHO_LT - symbol to control if libtool is verbose
dnl	ECHO_LD - symbol to prefix "cc -o" lines
dnl	RULE_CC - symbol to put before implicit "cc -c" lines (e.g., .c.o)
dnl	SHOW_CC - symbol to put before explicit "cc -c" lines
dnl	ECHO_CC - symbol to put before any "cc" line
dnl
AC_DEFUN([CF_DISABLE_ECHO],[
AC_MSG_CHECKING(if you want to see long compiling messages)
CF_ARG_DISABLE(echo,
	[  --disable-echo          display "compiling" commands],
	[
    ECHO_LT='--silent'
    ECHO_LD='@echo linking [$]@;'
    RULE_CC='	@echo compiling [$]<'
    SHOW_CC='	@echo compiling [$]@'
    ECHO_CC='@'
],[
    ECHO_LT=''
    ECHO_LD=''
    RULE_CC='# compiling'
    SHOW_CC='# compiling'
    ECHO_CC=''
])
AC_MSG_RESULT($enableval)
AC_SUBST(ECHO_LT)
AC_SUBST(ECHO_LD)
AC_SUBST(RULE_CC)
AC_SUBST(SHOW_CC)
AC_SUBST(ECHO_CC)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_FIND_LIBRARY version: 8 updated: 2004/11/23 20:14:58
dnl ---------------
dnl Look for a non-standard library, given parameters for AC_TRY_LINK.  We
dnl prefer a standard location, and use -L options only if we do not find the
dnl library in the standard library location(s).
dnl	$1 = library name
dnl	$2 = library class, usually the same as library name
dnl	$3 = includes
dnl	$4 = code fragment to compile/link
dnl	$5 = corresponding function-name
dnl	$6 = flag, nonnull if failure should not cause an error-exit
dnl
dnl Sets the variable "$cf_libdir" as a side-effect, so we can see if we had
dnl to use a -L option.
AC_DEFUN([CF_FIND_LIBRARY],
[
	eval 'cf_cv_have_lib_'$1'=no'
	cf_libdir=""
	AC_CHECK_FUNC($5,
		eval 'cf_cv_have_lib_'$1'=yes',[
		cf_save_LIBS="$LIBS"
		AC_MSG_CHECKING(for $5 in -l$1)
		LIBS="-l$1 $LIBS"
		AC_TRY_LINK([$3],[$4],
			[AC_MSG_RESULT(yes)
			 eval 'cf_cv_have_lib_'$1'=yes'
			],
			[AC_MSG_RESULT(no)
			CF_LIBRARY_PATH(cf_search,$2)
			for cf_libdir in $cf_search
			do
				AC_MSG_CHECKING(for -l$1 in $cf_libdir)
				LIBS="-L$cf_libdir -l$1 $cf_save_LIBS"
				AC_TRY_LINK([$3],[$4],
					[AC_MSG_RESULT(yes)
			 		 eval 'cf_cv_have_lib_'$1'=yes'
					 break],
					[AC_MSG_RESULT(no)
					 LIBS="$cf_save_LIBS"])
			done
			])
		])
eval 'cf_found_library=[$]cf_cv_have_lib_'$1
ifelse($6,,[
if test $cf_found_library = no ; then
	AC_ERROR(Cannot link $1 library)
fi
])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_FUNC_LSTAT version: 2 updated: 1997/09/07 13:00:21
dnl -------------
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
dnl CF_GCC_ATTRIBUTES version: 9 updated: 2002/12/21 19:25:52
dnl -----------------
dnl Test for availability of useful gcc __attribute__ directives to quiet
dnl compiler warnings.  Though useful, not all are supported -- and contrary
dnl to documentation, unrecognized directives cause older compilers to barf.
AC_DEFUN([CF_GCC_ATTRIBUTES],
[
if test "$GCC" = yes
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
if test "$GCC" = yes
then
	AC_CHECKING([for $CC __attribute__ directives])
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
int main(int argc GCC_UNUSED, char *argv[[]] GCC_UNUSED) { return 0; }
EOF
	for cf_attribute in scanf printf unused noreturn
	do
		CF_UPPER(CF_ATTRIBUTE,$cf_attribute)
		cf_directive="__attribute__(($cf_attribute))"
		echo "checking for $CC $cf_directive" 1>&AC_FD_CC
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
		fi
	done
else
	fgrep define conftest.i >>confdefs.h
fi
rm -rf conftest*
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_GCC_VERSION version: 3 updated: 2003/09/06 19:16:57
dnl --------------
dnl Find version of gcc
AC_DEFUN([CF_GCC_VERSION],[
AC_REQUIRE([AC_PROG_CC])
GCC_VERSION=none
if test "$GCC" = yes ; then
	AC_MSG_CHECKING(version of $CC)
	GCC_VERSION="`${CC} --version|sed -e '2,$d' -e 's/^[[^0-9.]]*//' -e 's/[[^0-9.]].*//'`"
	test -z "$GCC_VERSION" && GCC_VERSION=unknown
	AC_MSG_RESULT($GCC_VERSION)
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_GCC_WARNINGS version: 18 updated: 2004/12/03 20:51:07
dnl ---------------
dnl Check if the compiler supports useful warning options.  There's a few that
dnl we don't use, simply because they're too noisy:
dnl
dnl	-Wconversion (useful in older versions of gcc, but not in gcc 2.7.x)
dnl	-Wredundant-decls (system headers make this too noisy)
dnl	-Wtraditional (combines too many unrelated messages, only a few useful)
dnl	-Wwrite-strings (too noisy, but should review occasionally).  This
dnl		is enabled for ncurses using "--enable-const".
dnl	-pedantic
dnl
dnl Parameter:
dnl	$1 is an optional list of gcc warning flags that a particular
dnl		application might want to use, e.g., "no-unused" for
dnl		-Wno-unused
dnl Special:
dnl	If $with_ext_const is "yes", add a check for -Wwrite-strings
dnl
AC_DEFUN([CF_GCC_WARNINGS],
[
AC_REQUIRE([CF_INTEL_COMPILER])
AC_REQUIRE([CF_GCC_VERSION])
if test "$INTEL_COMPILER" = yes
then
# The "-wdXXX" options suppress warnings:
# remark #1419: external declaration in primary source file
# remark #193: zero used for undefined preprocessing identifier
# remark #593: variable "curs_sb_left_arrow" was set but never used
# remark #810: conversion from "int" to "Dimension={unsigned short}" may lose significant bits
# remark #869: parameter "tw" was never referenced
# remark #981: operands are evaluated in unspecified order
# warning #269: invalid format string conversion
	EXTRA_CFLAGS="$EXTRA_CFLAGS -Wall \
 -wd1419 \
 -wd193 \
 -wd279 \
 -wd593 \
 -wd810 \
 -wd869 \
 -wd981"
elif test "$GCC" = yes
then
	cat > conftest.$ac_ext <<EOF
#line __oline__ "configure"
int main(int argc, char *argv[[]]) { return (argv[[argc-1]] == 0) ; }
EOF
	AC_CHECKING([for $CC warning options])
	cf_save_CFLAGS="$CFLAGS"
	EXTRA_CFLAGS="-W -Wall"
	cf_warn_CONST=""
	test "$with_ext_const" = yes && cf_warn_CONST="Wwrite-strings"
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
		Wstrict-prototypes \
		Wundef $cf_warn_CONST $1
	do
		CFLAGS="$cf_save_CFLAGS $EXTRA_CFLAGS -$cf_opt"
		if AC_TRY_EVAL(ac_compile); then
			test -n "$verbose" && AC_MSG_RESULT(... -$cf_opt)
			case $cf_opt in #(vi
			Wcast-qual) #(vi
				CPPFLAGS="$CPPFLAGS -DXTSTRINGDEFINES"
				;;
			Winline) #(vi
				case $GCC_VERSION in
				3.3*)
					CF_VERBOSE(feature is broken in gcc $GCC_VERSION)
					continue;;
				esac
				;;
			esac
			EXTRA_CFLAGS="$EXTRA_CFLAGS -$cf_opt"
		fi
	done
	rm -f conftest*
	CFLAGS="$cf_save_CFLAGS"
fi
AC_SUBST(EXTRA_CFLAGS)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_GMTOFF version: 4 updated: 2002/04/16 13:02:23
dnl ---------
AC_DEFUN([CF_GMTOFF],
[
cf_decl='#include <sys/types.h>
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
'
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
		[long x = timezone;
		 timezone = x],
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
dnl CF_GNU_SOURCE version: 4 updated: 2004/12/03 20:43:00
dnl -------------
dnl Check if we must define _GNU_SOURCE to get a reasonable value for
dnl _XOPEN_SOURCE, upon which many POSIX definitions depend.  This is a defect
dnl (or misfeature) of glibc2, which breaks portability of many applications,
dnl since it is interwoven with GNU extensions.
dnl
dnl Well, yes we could work around it...
AC_DEFUN([CF_GNU_SOURCE],
[
AC_REQUIRE([CF_INTEL_COMPILER])

if test "$INTEL_COMPILER" = no ; then
AC_CACHE_CHECK(if we must define _GNU_SOURCE,cf_cv_gnu_source,[
AC_TRY_COMPILE([#include <sys/types.h>],[
#ifndef _XOPEN_SOURCE
make an error
#endif],
	[cf_cv_gnu_source=no],
	[cf_save="$CPPFLAGS"
	 CPPFLAGS="$CPPFLAGS -D_GNU_SOURCE"
	 AC_TRY_COMPILE([#include <sys/types.h>],[
#ifdef _XOPEN_SOURCE
make an error
#endif],
	[cf_cv_gnu_source=no],
	[cf_cv_gnu_source=yes])
	CPPFLAGS="$cf_save"
	])
])
test "$cf_cv_gnu_source" = yes && CPPFLAGS="$CPPFLAGS -D_GNU_SOURCE"
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_HAVE_FUNCS version: 5 updated: 2003/02/02 19:21:05
dnl -------------
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
if test "$WithPrototypes" != yes; then
	cf_result=yes
elif test "$cf_opt_with_warnings" = yes; then
	if test "$GCC" = yes; then
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
if test "$cf_result" != no; then
	AC_DEFINE_UNQUOTED(HAVE_${cf_tr_func})
	if test "$cf_result" = undeclared; then
		AC_DEFINE_UNQUOTED(NEED_${cf_tr_func})
	fi
fi
done
CFLAGS="$cf_CFLAGS"
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_HEADER_PATH version: 8 updated: 2002/11/10 14:46:59
dnl --------------
dnl Construct a search-list for a nonstandard header-file
AC_DEFUN([CF_HEADER_PATH],
[CF_SUBDIR_PATH($1,$2,include)
test "$includedir" != NONE && \
test "$includedir" != "/usr/include" && \
test -d "$includedir" && {
	test -d $includedir &&    $1="[$]$1 $includedir"
	test -d $includedir/$2 && $1="[$]$1 $includedir/$2"
}

test "$oldincludedir" != NONE && \
test "$oldincludedir" != "/usr/include" && \
test -d "$oldincludedir" && {
	test -d $oldincludedir    && $1="[$]$1 $oldincludedir"
	test -d $oldincludedir/$2 && $1="[$]$1 $oldincludedir/$2"
}

])dnl
dnl ---------------------------------------------------------------------------
dnl CF_INTEL_COMPILER version: 1 updated: 2004/12/03 20:27:48
dnl -----------------
dnl Check if the given compiler is really the Intel compiler for Linux.
dnl It tries to imitate gcc, but does not return an error when it finds a
dnl mismatch between prototypes, e.g., as exercised by CF_MISSING_CHECK.
dnl
dnl This macro should be run "soon" after AC_PROG_CC, to ensure that it is
dnl not mistaken for gcc.
AC_DEFUN([CF_INTEL_COMPILER],[
AC_REQUIRE([AC_PROG_CC])

INTEL_COMPILER=no

if test "$GCC" = yes ; then
	case $host_os in
	linux*|gnu*)
		AC_MSG_CHECKING(if this is really Intel compiler)
		cf_save_CFLAGS="$CFLAGS"
		CFLAGS="$CFLAGS -no-gcc"
		AC_TRY_COMPILE([],[
#ifdef __INTEL_COMPILER
#else
make an error
#endif
],[INTEL_COMPILER=yes
cf_save_CFLAGS="$cf_save_CFLAGS -we147 -no-gcc"
],[])
		CFLAGS="$cf_save_CFLAGS"
		AC_MSG_RESULT($INTEL_COMPILER)
		;;
	esac
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_LIBRARY_PATH version: 7 updated: 2002/11/10 14:46:59
dnl ---------------
dnl Construct a search-list for a nonstandard library-file
AC_DEFUN([CF_LIBRARY_PATH],
[CF_SUBDIR_PATH($1,$2,lib)])dnl
dnl ---------------------------------------------------------------------------
dnl CF_LIB_PREFIX version: 7 updated: 2001/01/12 01:23:48
dnl -------------
dnl Compute the library-prefix for the given host system
dnl $1 = variable to set
AC_DEFUN([CF_LIB_PREFIX],
[
	case $cf_cv_system_name in
	OS/2*)	LIB_PREFIX=''     ;;
	os2*)	LIB_PREFIX=''     ;;
	*)	LIB_PREFIX='lib'  ;;
	esac
ifelse($1,,,[$1=$LIB_PREFIX])
	AC_SUBST(LIB_PREFIX)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_LOCALE version: 4 updated: 2003/02/16 08:16:04
dnl ---------
dnl Check if we have setlocale() and its header, <locale.h>
dnl The optional parameter $1 tells what to do if we do have locale support.
AC_DEFUN([CF_LOCALE],
[
AC_MSG_CHECKING(for setlocale())
AC_CACHE_VAL(cf_cv_locale,[
AC_TRY_LINK([#include <locale.h>],
	[setlocale(LC_ALL, "")],
	[cf_cv_locale=yes],
	[cf_cv_locale=no])
	])
AC_MSG_RESULT($cf_cv_locale)
test $cf_cv_locale = yes && { ifelse($1,,AC_DEFINE(LOCALE),[$1]) }
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_MAKEFLAGS version: 9 updated: 2001/12/30 18:17:27
dnl ------------
dnl Some 'make' programs support $(MAKEFLAGS), some $(MFLAGS), to pass 'make'
dnl options to lower-levels.  It's very useful for "make -n" -- if we have it.
dnl (GNU 'make' does both, something POSIX 'make', which happens to make the
dnl $(MAKEFLAGS) variable incompatible because it adds the assignments :-)
AC_DEFUN([CF_MAKEFLAGS],
[
AC_CACHE_CHECK(for makeflags variable, cf_cv_makeflags,[
	cf_cv_makeflags=''
	for cf_option in '-$(MAKEFLAGS)' '$(MFLAGS)'
	do
		cat >cf_makeflags.tmp <<CF_EOF
SHELL = /bin/sh
all :
	@ echo '.$cf_option'
CF_EOF
		cf_result=`${MAKE-make} -k -f cf_makeflags.tmp 2>/dev/null`
		case "$cf_result" in
		.*k)
			cf_result=`${MAKE-make} -k -f cf_makeflags.tmp CC=cc 2>/dev/null`
			case "$cf_result" in
			.*CC=*)	cf_cv_makeflags=
				;;
			*)	cf_cv_makeflags=$cf_option
				;;
			esac
			break
			;;
		*)	echo no match "$cf_result"
			;;
		esac
	done
	rm -f cf_makeflags.tmp
])

AC_SUBST(cf_cv_makeflags)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_MAKE_AR_RULES version: 3 updated: 2001/07/15 16:04:43
dnl ----------------
dnl Check if the 'make' program knows how to interpret archive rules.  Though
dnl this is common practice since the mid-80's, there are some holdouts (1997).
AC_DEFUN([CF_MAKE_AR_RULES],
[
AC_MSG_CHECKING(if ${MAKE-make} program knows about archives)
AC_CACHE_VAL(cf_cv_ar_rules,[
cf_dir=subd$$
cf_cv_ar_rules=unknown
mkdir $cf_dir
cat >$cf_dir/makefile <<CF_EOF
SHELL = /bin/sh
AR = ar crv
CC = $CC

.SUFFIXES:
.SUFFIXES: .c .o .a

all:  conf.a

.c.a:
	\$(CC) -c $<
	\$(AR) \$[]@ \$[]*.o
conf.a : conf.a(conftest.o)
CF_EOF
touch $cf_dir/conftest.c
CDPATH=; export CDPATH
if ( cd $cf_dir && ${MAKE-make} 2>&AC_FD_CC >&AC_FD_CC && test -f conf.a )
then
	cf_cv_ar_rules=yes
else
echo ... did not find archive >&AC_FD_CC
rm -f $cf_dir/conftest.o
cat >$cf_dir/makefile <<CF_EOF
SHELL = /bin/sh
AR = ar crv
CC = $CC

.SUFFIXES:
.SUFFIXES: .c .o

all:  conf.a

.c.o:
	\$(CC) -c $<

conf.a : conftest.o
	\$(AR) \$[]@ \$?
CF_EOF
CDPATH=; export CDPATH
if ( cd $cf_dir && ${MAKE-make} 2>&AC_FD_CC >&AC_FD_CC && test -f conf.a )
then
	cf_cv_ar_rules=no
else
	AC_ERROR(I do not know how to construct a library)
fi
fi
rm -rf $cf_dir
])
AC_MSG_RESULT($cf_cv_ar_rules)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_MAKE_INCLUDE version: 7 updated: 2001/07/15 16:04:43
dnl ---------------
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
	CDPATH=; export CDPATH
	eval `(cd $cf_dir && ${MAKE-make}) 2>&AC_FD_CC | grep cf_make_include=OK`
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
dnl CF_MIXEDCASE_FILENAMES version: 3 updated: 2003/09/20 17:07:55
dnl ----------------------
dnl Check if the file-system supports mixed-case filenames.  If we're able to
dnl create a lowercase name and see it as uppercase, it doesn't support that.
AC_DEFUN([CF_MIXEDCASE_FILENAMES],
[
AC_CACHE_CHECK(if filesystem supports mixed-case filenames,cf_cv_mixedcase,[
if test "$cross_compiling" = yes ; then
	case $target_alias in #(vi
	*-os2-emx*|*-msdosdjgpp*|*-cygwin*|*-mingw32*|*-uwin*) #(vi
		cf_cv_mixedcase=no
		;;
	*)
		cf_cv_mixedcase=yes
		;;
	esac
else
	rm -f conftest CONFTEST
	echo test >conftest
	if test -f CONFTEST ; then
		cf_cv_mixedcase=no
	else
		cf_cv_mixedcase=yes
	fi
	rm -f conftest CONFTEST
fi
])
test "$cf_cv_mixedcase" = yes && AC_DEFINE(MIXEDCASE_FILENAMES)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_MSG_LOG version: 3 updated: 1997/09/07 14:05:52
dnl ----------
dnl Write a debug message to config.log, along with the line number in the
dnl configure script.
AC_DEFUN([CF_MSG_LOG],[
echo "(line __oline__) testing $* ..." 1>&AC_FD_CC
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_NCURSES_CC_CHECK version: 3 updated: 2003/01/12 18:59:28
dnl -------------------
dnl Check if we can compile with ncurses' header file
dnl $1 is the cache variable to set
dnl $2 is the header-file to include
dnl $3 is the root name (ncurses or ncursesw)
AC_DEFUN([CF_NCURSES_CC_CHECK],[
	AC_TRY_COMPILE([
]ifelse($3,ncursesw,[
#define _XOPEN_SOURCE_EXTENDED
#undef  HAVE_LIBUTF8_H	/* in case we used CF_UTF8_LIB */
#define HAVE_LIBUTF8_H	/* to force ncurses' header file to use cchar_t */
])[
#include <$2>],[
#ifdef NCURSES_VERSION
]ifelse($3,ncursesw,[
#ifndef WACS_BSSB
	make an error
#endif
])[
printf("%s\n", NCURSES_VERSION);
#else
#ifdef __NCURSES_H
printf("old\n");
#else
	make an error
#endif
#endif
	]
	,[$1=$cf_header]
	,[$1=no])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_NCURSES_CPPFLAGS version: 17 updated: 2003/11/06 19:59:57
dnl -------------------
dnl Look for the SVr4 curses clone 'ncurses' in the standard places, adjusting
dnl the CPPFLAGS variable so we can include its header.
dnl
dnl The header files may be installed as either curses.h, or ncurses.h (would
dnl be obsolete, except that some packagers prefer this name to distinguish it
dnl from a "native" curses implementation).  If not installed for overwrite,
dnl the curses.h file would be in an ncurses subdirectory (e.g.,
dnl /usr/include/ncurses), but someone may have installed overwriting the
dnl vendor's curses.  Only very old versions (pre-1.9.2d, the first autoconf'd
dnl version) of ncurses don't define either __NCURSES_H or NCURSES_VERSION in
dnl the header.
dnl
dnl If the installer has set $CFLAGS or $CPPFLAGS so that the ncurses header
dnl is already in the include-path, don't even bother with this, since we cannot
dnl easily determine which file it is.  In this case, it has to be <curses.h>.
dnl
dnl The optional parameter gives the root name of the library, in case it is
dnl not installed as the default curses library.  That is how the
dnl wide-character version of ncurses is installed.
AC_DEFUN([CF_NCURSES_CPPFLAGS],
[AC_REQUIRE([CF_WITH_CURSES_DIR])

AC_PROVIDE([CF_CURSES_CPPFLAGS])dnl
cf_ncuhdr_root=ifelse($1,,ncurses,$1)

test -n "$cf_cv_curses_dir" && \
test "$cf_cv_curses_dir" != "no" && \
CPPFLAGS="-I$cf_cv_curses_dir/include -I$cf_cv_curses_dir/include/$cf_ncuhdr_root $CPPFLAGS"

AC_CACHE_CHECK(for $cf_ncuhdr_root header in include-path, cf_cv_ncurses_h,[
	cf_header_list="$cf_ncuhdr_root/curses.h $cf_ncuhdr_root/ncurses.h"
	( test "$cf_ncuhdr_root" = ncurses || test "$cf_ncuhdr_root" = ncursesw ) && cf_header_list="$cf_header_list curses.h ncurses.h"
	for cf_header in $cf_header_list
	do
		CF_NCURSES_CC_CHECK(cf_cv_ncurses_h,$cf_header,$1)
		test "$cf_cv_ncurses_h" != no && break
	done
])

if test "$cf_cv_ncurses_h" != no ; then
	cf_cv_ncurses_header=$cf_cv_ncurses_h
else
AC_CACHE_CHECK(for $cf_ncuhdr_root include-path, cf_cv_ncurses_h2,[
	test -n "$verbose" && echo
	CF_HEADER_PATH(cf_search,$cf_ncuhdr_root)
	test -n "$verbose" && echo search path $cf_search
	cf_save2_CPPFLAGS="$CPPFLAGS"
	for cf_incdir in $cf_search
	do
		CF_ADD_INCDIR($cf_incdir)
		for cf_header in \
			ncurses.h \
			curses.h
		do
			CF_NCURSES_CC_CHECK(cf_cv_ncurses_h2,$cf_header,$1)
			if test "$cf_cv_ncurses_h2" != no ; then
				cf_cv_ncurses_h2=$cf_incdir/$cf_header
				test -n "$verbose" && echo $ac_n "	... found $ac_c" 1>&AC_FD_MSG
				break
			fi
			test -n "$verbose" && echo "	... tested $cf_incdir/$cf_header" 1>&AC_FD_MSG
		done
		CPPFLAGS="$cf_save2_CPPFLAGS"
		test "$cf_cv_ncurses_h2" != no && break
	done
	test "$cf_cv_ncurses_h2" = no && AC_ERROR(not found)
	])

	CF_DIRNAME(cf_1st_incdir,$cf_cv_ncurses_h2)
	cf_cv_ncurses_header=`basename $cf_cv_ncurses_h2`
	if test `basename $cf_1st_incdir` = $cf_ncuhdr_root ; then
		cf_cv_ncurses_header=$cf_ncuhdr_root/$cf_cv_ncurses_header
	fi
	CF_ADD_INCDIR($cf_1st_incdir)

fi

AC_DEFINE(NCURSES)

case $cf_cv_ncurses_header in # (vi
*ncurses.h)
	AC_DEFINE(HAVE_NCURSES_H)
	;;
esac

case $cf_cv_ncurses_header in # (vi
ncurses/curses.h|ncurses/ncurses.h)
	AC_DEFINE(HAVE_NCURSES_NCURSES_H)
	;;
ncursesw/curses.h|ncursesw/ncurses.h)
	AC_DEFINE(HAVE_NCURSESW_NCURSES_H)
	;;
esac

CF_NCURSES_VERSION
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_NCURSES_LIBS version: 12 updated: 2004/04/27 16:26:05
dnl ---------------
dnl Look for the ncurses library.  This is a little complicated on Linux,
dnl because it may be linked with the gpm (general purpose mouse) library.
dnl Some distributions have gpm linked with (bsd) curses, which makes it
dnl unusable with ncurses.  However, we don't want to link with gpm unless
dnl ncurses has a dependency, since gpm is normally set up as a shared library,
dnl and the linker will record a dependency.
dnl
dnl The optional parameter gives the root name of the library, in case it is
dnl not installed as the default curses library.  That is how the
dnl wide-character version of ncurses is installed.
AC_DEFUN([CF_NCURSES_LIBS],
[AC_REQUIRE([CF_NCURSES_CPPFLAGS])

cf_nculib_root=ifelse($1,,ncurses,$1)
	# This works, except for the special case where we find gpm, but
	# ncurses is in a nonstandard location via $LIBS, and we really want
	# to link gpm.
cf_ncurses_LIBS=""
cf_ncurses_SAVE="$LIBS"
AC_CHECK_LIB(gpm,Gpm_Open,
	[AC_CHECK_LIB(gpm,initscr,
		[LIBS="$cf_ncurses_SAVE"],
		[cf_ncurses_LIBS="-lgpm"])])

case $host_os in #(vi
freebsd*)
	# This is only necessary if you are linking against an obsolete
	# version of ncurses (but it should do no harm, since it's static).
	if test "$cf_nculib_root" = ncurses ; then
		AC_CHECK_LIB(mytinfo,tgoto,[cf_ncurses_LIBS="-lmytinfo $cf_ncurses_LIBS"])
	fi
	;;
esac

LIBS="$cf_ncurses_LIBS $LIBS"

if ( test -n "$cf_cv_curses_dir" && test "$cf_cv_curses_dir" != "no" )
then
	LIBS="-L$cf_cv_curses_dir/lib -l$cf_nculib_root $LIBS"
else
	CF_FIND_LIBRARY($cf_nculib_root,$cf_nculib_root,
		[#include <${cf_cv_ncurses_header-curses.h}>],
		[initscr()],
		initscr)
fi

if test -n "$cf_ncurses_LIBS" ; then
	AC_MSG_CHECKING(if we can link $cf_nculib_root without $cf_ncurses_LIBS)
	cf_ncurses_SAVE="$LIBS"
	for p in $cf_ncurses_LIBS ; do
		q=`echo $LIBS | sed -e "s%$p %%" -e "s%$p$%%"`
		if test "$q" != "$LIBS" ; then
			LIBS="$q"
		fi
	done
	AC_TRY_LINK([#include <${cf_cv_ncurses_header-curses.h}>],
		[initscr(); mousemask(0,0); tgoto((char *)0, 0, 0);],
		[AC_MSG_RESULT(yes)],
		[AC_MSG_RESULT(no)
		 LIBS="$cf_ncurses_SAVE"])
fi

CF_UPPER(cf_nculib_ROOT,HAVE_LIB$cf_nculib_root)
AC_DEFINE_UNQUOTED($cf_nculib_ROOT)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_NCURSES_VERSION version: 11 updated: 2003/11/06 19:59:57
dnl ------------------
dnl Check for the version of ncurses, to aid in reporting bugs, etc.
dnl Call CF_CURSES_CPPFLAGS first, or CF_NCURSES_CPPFLAGS.  We don't use
dnl AC_REQUIRE since that does not work with the shell's if/then/else/fi.
AC_DEFUN([CF_NCURSES_VERSION],
[
AC_REQUIRE([CF_CURSES_CPPFLAGS])dnl
AC_CACHE_CHECK(for ncurses version, cf_cv_ncurses_version,[
	cf_cv_ncurses_version=no
	cf_tempfile=out$$
	rm -f $cf_tempfile
	AC_TRY_RUN([
#include <${cf_cv_ncurses_header-curses.h}>
#include <stdio.h>
int main()
{
	FILE *fp = fopen("$cf_tempfile", "w");
#ifdef NCURSES_VERSION
# ifdef NCURSES_VERSION_PATCH
	fprintf(fp, "%s.%d\n", NCURSES_VERSION, NCURSES_VERSION_PATCH);
# else
	fprintf(fp, "%s\n", NCURSES_VERSION);
# endif
#else
# ifdef __NCURSES_H
	fprintf(fp, "old\n");
# else
	make an error
# endif
#endif
	exit(0);
}],[
	cf_cv_ncurses_version=`cat $cf_tempfile`],,[

	# This will not work if the preprocessor splits the line after the
	# Autoconf token.  The 'unproto' program does that.
	cat > conftest.$ac_ext <<EOF
#include <${cf_cv_ncurses_header-curses.h}>
#undef Autoconf
#ifdef NCURSES_VERSION
Autoconf NCURSES_VERSION
#else
#ifdef __NCURSES_H
Autoconf "old"
#endif
;
#endif
EOF
	cf_try="$ac_cpp conftest.$ac_ext 2>&AC_FD_CC | grep '^Autoconf ' >conftest.out"
	AC_TRY_EVAL(cf_try)
	if test -f conftest.out ; then
		cf_out=`cat conftest.out | sed -e 's%^Autoconf %%' -e 's%^[[^"]]*"%%' -e 's%".*%%'`
		test -n "$cf_out" && cf_cv_ncurses_version="$cf_out"
		rm -f conftest.out
	fi
])
	rm -f $cf_tempfile
])
test "$cf_cv_ncurses_version" = no || AC_DEFINE(NCURSES)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_OUTPUT_IF_CHANGED version: 2 updated: 1997/09/07 18:53:59
dnl --------------------
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
dnl CF_PATHSEP version: 3 updated: 2001/01/12 01:23:53
dnl ----------
dnl Provide a value for the $PATH and similar separator
AC_DEFUN([CF_PATHSEP],
[
	case $cf_cv_system_name in
	os2*)	PATHSEP=';'  ;;
	*)	PATHSEP=':'  ;;
	esac
ifelse($1,,,[$1=$PATHSEP])
	AC_SUBST(PATHSEP)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_PATH_SYNTAX version: 9 updated: 2002/09/17 23:03:38
dnl --------------
dnl Check the argument to see that it looks like a pathname.  Rewrite it if it
dnl begins with one of the prefix/exec_prefix variables, and then again if the
dnl result begins with 'NONE'.  This is necessary to work around autoconf's
dnl delayed evaluation of those symbols.
AC_DEFUN([CF_PATH_SYNTAX],[
case ".[$]$1" in #(vi
.\[$]\(*\)*|.\'*\'*) #(vi
  ;;
..|./*|.\\*) #(vi
  ;;
.[[a-zA-Z]]:[[\\/]]*) #(vi OS/2 EMX
  ;;
.\[$]{*prefix}*) #(vi
  eval $1="[$]$1"
  case ".[$]$1" in #(vi
  .NONE/*)
    $1=`echo [$]$1 | sed -e s%NONE%$ac_default_prefix%`
    ;;
  esac
  ;; #(vi
.NONE/*)
  $1=`echo [$]$1 | sed -e s%NONE%$ac_default_prefix%`
  ;;
*)
  ifelse($2,,[AC_ERROR([expected a pathname, not \"[$]$1\"])],$2)
  ;;
esac
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_POSIX_C_SOURCE version: 1 updated: 2004/10/17 10:43:13
dnl -----------------
dnl Define _POSIX_C_SOURCE to the given level, and _POSIX_SOURCE if needed.
dnl
dnl	POSIX.1-1990				_POSIX_SOURCE
dnl	POSIX.1-1990 and			_POSIX_SOURCE and
dnl		POSIX.2-1992 C-Language			_POSIX_C_SOURCE=2
dnl		Bindings Option
dnl	POSIX.1b-1993				_POSIX_C_SOURCE=199309L
dnl	POSIX.1c-1996				_POSIX_C_SOURCE=199506L
dnl	X/Open 2000				_POSIX_C_SOURCE=200112L
dnl
dnl Parameters:
dnl	$1 is the nominal value for _POSIX_C_SOURCE
AC_DEFUN([CF_POSIX_C_SOURCE],
[
cf_POSIX_C_SOURCE=ifelse($1,,199506L,$1)
AC_CACHE_CHECK(if we should define _POSIX_C_SOURCE,cf_cv_posix_c_source,[
	CF_MSG_LOG(if the symbol is already defined go no further)
	AC_TRY_COMPILE([#include <sys/types.h>],[
#ifndef _POSIX_C_SOURCE
make an error
#endif],
	[cf_cv_posix_c_source=no],
	[case .$cf_POSIX_C_SOURCE in
	 .[[12]]??*)
		cf_cv_posix_c_source="-D_POSIX_C_SOURCE=$cf_POSIX_C_SOURCE"
		;;
	 .2)
		cf_cv_posix_c_source="-D_POSIX_C_SOURCE=$cf_POSIX_C_SOURCE -D_POSIX_SOURCE"
		;;
	 .*)
		cf_cv_posix_c_source="-D_POSIX_SOURCE"
		;;
	 esac
	 CF_MSG_LOG(ifdef from value $cf_POSIX_C_SOURCE)
	 cf_save="$CPPFLAGS"
	 CPPFLAGS="$CPPFLAGS $cf_cv_posix_c_source"
	 CF_MSG_LOG(if the second compile does not leave our definition intact error)
	 AC_TRY_COMPILE([#include <sys/types.h>],[
#ifndef _POSIX_C_SOURCE
make an error
#endif],,
	[cf_cv_posix_c_source=no])
	CPPFLAGS="$cf_save"
	])
])
test "$cf_cv_posix_c_source" != no && CPPFLAGS="$CPPFLAGS $cf_cv_posix_c_source"
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_PROGRAM_FULLPATH version: 4 updated: 2000/11/02 01:26:33
dnl -------------------
dnl Tests for one or more programs given by name along the user's path, and
dnl sets a variable to the program's full-path if found.
AC_DEFUN([CF_PROGRAM_FULLPATH],
[
AC_REQUIRE([CF_PATHSEP])
AC_REQUIRE([CF_PROG_EXT])
AC_MSG_CHECKING(full path of $1)
AC_CACHE_VAL(cf_cv_$1,[
	cf_cv_$1="[$]$1"
	if test -z "[$]cf_cv_$1"; then
		set -- $2;
		while test [$]# != 0; do
			cf_word=[$]1${PROG_EXT}
			case [$]1 in
			-*)
				;;
			*)
				if test -f "$cf_word" && test ! -f "./$cf_word" && test -x "$cf_word"; then
					cf_cv_$1="$cf_word"
				else
					IFS="${IFS= 	}"; cf_save_ifs="$IFS"; IFS="${IFS}${PATHSEP}"
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
dnl CF_PROGRAM_PREFIX version: 3 updated: 2000/11/02 01:26:33
dnl -----------------
dnl	Tests for a program given by name along the user's path, and sets a
dnl	variable to the program's directory-prefix if found.  Don't match if
dnl	the directory is ".", since we need an absolute path-reference.
AC_DEFUN([CF_PROGRAM_PREFIX],
[
AC_REQUIRE([CF_PATHSEP])
# Extract the first word of `$2', so it can be a program name with args.
set cf_dummy $2; cf_word=[$]2
AC_MSG_CHECKING(for $cf_word prefix ($1))
AC_CACHE_VAL([cf_cv_$1],[
	# allow import from environment-variable
	cf_cv_$1="[$]$1"
	if test -z "[$]cf_cv_$1"; then
		IFS="${IFS= 	}"; cf_save_ifs="$IFS"; IFS="${IFS}${PATHSEP}"
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
dnl CF_PROG_EXT version: 10 updated: 2004/01/03 19:28:18
dnl -----------
dnl Compute $PROG_EXT, used for non-Unix ports, such as OS/2 EMX.
AC_DEFUN([CF_PROG_EXT],
[
AC_REQUIRE([CF_CHECK_CACHE])
case $cf_cv_system_name in
os2*)
    CFLAGS="$CFLAGS -Zmt"
    CPPFLAGS="$CPPFLAGS -D__ST_MT_ERRNO__"
    CXXFLAGS="$CXXFLAGS -Zmt"
    # autoconf's macro sets -Zexe and suffix both, which conflict:w
    LDFLAGS="$LDFLAGS -Zmt -Zcrtdll"
    ac_cv_exeext=.exe
    ;;
esac

AC_EXEEXT
AC_OBJEXT

PROG_EXT="$EXEEXT"
AC_SUBST(PROG_EXT)
test -n "$PROG_EXT" && AC_DEFINE_UNQUOTED(PROG_EXT,"$PROG_EXT")
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_RCS_SCCS version: 4 updated: 2003/06/10 20:35:36
dnl -----------
dnl Tests for the ensemble of programs that are used in RCS, SCCS, VCS, CVS.
dnl We'll have to assume that the related utilities all reside in the same
dnl directory.
AC_DEFUN([CF_RCS_SCCS],
[
cf_rcs_sccs_path="$PATH"
PATH="/usr/local/lib/cssc:$PATH"
PATH="/usr/local/libexec/cssc:$PATH"
PATH="/usr/libexec/cssc:$PATH"
PATH="/usr/lib/cssc:$PATH"
CF_PROGRAM_PREFIX(RCS_PATH, rcs)
CF_PROGRAM_PREFIX(SCCS_PATH, admin)dnl the SCCS tool
CF_PROGRAM_PREFIX(VCS_PATH, vcs)dnl VCS is my RCS application
CF_PROGRAM_PREFIX(CVS_PATH, cvs)dnl CVS is a layer above RCS
CF_PROGRAM_PREFIX(CMV_PATH, cmv)dnl CmVision combines RCS and SCCS archives
PATH="$cf_rcs_sccs_path"
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_REGCMP_FUNCS version: 2 updated: 1997/09/07 15:13:55
dnl ---------------
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
dnl CF_REGCMP_LIBS version: 2 updated: 1997/09/07 15:13:55
dnl --------------
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
dnl CF_REGEXPR_H_FUNCS version: 3 updated: 2001/12/11 13:49:56
dnl ------------------
dnl Tests for the <regexpr.h> include-file, and the functions associated with it.
AC_DEFUN([CF_REGEXPR_H_FUNCS],
[
cf_save_libs="$LIBS"
AC_CHECK_LIB(gen, compile)
AC_CACHE_CHECK(compile/step functions, cf_cv_REGEXPR_H,[
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
if test $cf_cv_REGEXPR_H = yes; then
	AC_DEFINE(HAVE_REGEXPR_H_FUNCS)
else
	LIBS="$cf_save_libs"
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_REGEX_H_FUNCS version: 3 updated: 2001/12/11 13:49:56
dnl ----------------
dnl Tests for the <regex.h> include-file, and the functions associated with it.
AC_DEFUN([CF_REGEX_H_FUNCS],
[
AC_CHECK_LIB(regex,regcomp,[LIBS="-lregex $LIBS"])
AC_CACHE_CHECK(regcomp/regexec functions, cf_cv_REGEX_H,[
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
test $cf_cv_REGEX_H = yes && AC_DEFINE(HAVE_REGEX_H_FUNCS)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_RE_COMP_FUNCS version: 2 updated: 1997/09/07 15:13:55
dnl ----------------
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
dnl CF_SIZE_T version: 4 updated: 2000/01/22 00:19:54
dnl ---------
dnl	On both Ultrix and CLIX, I find size_t defined in <stdio.h>
AC_DEFUN([CF_SIZE_T],
[
AC_MSG_CHECKING(for size_t in <sys/types.h> or <stdio.h>)
AC_CACHE_VAL(cf_cv_type_size_t,[
	AC_TRY_COMPILE([
#include <sys/types.h>
#ifdef STDC_HEADERS
#include <stdlib.h>
#include <stddef.h>
#endif
#include <stdio.h>],
		[size_t x],
		[cf_cv_type_size_t=yes],
		[cf_cv_type_size_t=no])
	])
AC_MSG_RESULT($cf_cv_type_size_t)
test $cf_cv_type_size_t = no && AC_DEFINE(size_t, unsigned)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_SRC_MAKEFILE version: 2 updated: 1997/09/13 01:16:12
dnl ---------------
dnl Append predefined lists to $2/makefile, given a path to a directory that
dnl has a 'modules' file in $1.
dnl
dnl The library is named $Z, to avoid problems with parentheses.
AC_DEFUN([CF_SRC_MAKEFILE],
[
cf_mod=$1/$2/modules
cf_out=$2/makefile
if test -f $cf_mod ; then
${AWK-awk} <$cf_mod >>$cf_out '
BEGIN	{
		found = 0;
	}
	{
		if ( found == 0 )
		{
			printf "\nCSRC="
			found = 1;
		}
		printf " \\\n\t%s.c", [$]1
	}
END	{
		print ""
	}
'
	if test $cf_cv_ar_rules = yes ; then
${AWK-awk} <$cf_mod >>$cf_out '
BEGIN	{
		found = 0;
	}
	{
		if ( found == 0 )
		{
			printf "\nOBJS="
			found = 1;
		}
		printf " \\\n\t$Z(%s.o)", [$]1
	}
END	{
		print ""
	}
'
	else
${AWK-awk} <$cf_mod >>$cf_out '
BEGIN	{
		found = 0;
	}
	{
		if ( found == 0 )
		{
			printf "\nOBJS="
			found = 1;
		}
		printf " \\\n\t%s.o", [$]1
	}
END	{
		print ""
	}
'
	fi
ifelse($3,,,[
	cat >>$cf_out <<CF_EOF


${make_include_left}$3${make_include_right}
CF_EOF
])
test -f $1/$2/makefile.2nd && \
    cat $1/$2/makefile.2nd >>$2/makefile

	if test $cf_cv_ar_rules = yes ; then
cat >>$cf_out <<CF_EOF

\$Z:	\$(OBJS)
	\$(RANLIB) \$Z
CF_EOF
	else
cat >>$cf_out <<CF_EOF

\$Z:	\$(OBJS)
	\$(AR) \$Z \$(OBJS)
	\$(RANLIB) \$Z
CF_EOF
	fi
fi
test -f $1/$2/makefile.end && \
    cat $1/$2/makefile.end >>$2/makefile
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_STAT_ST_BLOCKS version: 3 updated: 1999/01/15 22:38:46
dnl -----------------
dnl Test if the <sys/stat.h> 'stat' struct defines 'st_blocks' member.
dnl If not, assume it's some non-BSD system.
AC_DEFUN([CF_STAT_ST_BLOCKS],
[
AC_MSG_CHECKING(for .st_blocks in struct stat)
AC_CACHE_VAL(cf_cv_st_blocks,[
	AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/stat.h>],
		[int t(); {struct stat sb; return sb.st_blocks;}],
		[cf_cv_st_blocks=yes],
		[cf_cv_st_blocks=no])])
AC_MSG_RESULT($cf_cv_st_blocks)
test $cf_cv_st_blocks = yes && AC_DEFINE(STAT_HAS_ST_BLOCKS)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_STRIP_G_OPT version: 3 updated: 2002/12/21 19:25:52
dnl --------------
dnl	Remove "-g" option from the compiler options
AC_DEFUN([CF_STRIP_G_OPT],
[$1=`echo ${$1} | sed -e 's%-g %%' -e 's%-g$%%'`])dnl
dnl ---------------------------------------------------------------------------
dnl CF_STRUCT_SCREEN version: 3 updated: 2003/11/06 19:59:57
dnl ----------------
dnl Test if curses defines 'struct screen'.
dnl
dnl	If this isn't defined, we cannot build a lint library that will check
dnl	for that type, since it isn't resolved.
dnl
AC_DEFUN([CF_STRUCT_SCREEN],
[
AC_REQUIRE([CF_CURSES_CPPFLAGS])dnl
AC_MSG_CHECKING(if curses uses struct screen)
AC_CACHE_VAL(cf_cv_have_struct_screen,[
	AC_TRY_COMPILE([
#define lint	/* sysvr4 has its own fallback for lint */
#include <${cf_cv_ncurses_header-curses.h}>],
		[struct screen dummy],
		[cf_cv_have_struct_screen=yes],
		[cf_cv_have_struct_screen=no])
	])
AC_MSG_RESULT($cf_cv_have_struct_screen)

AC_MSG_CHECKING(for definition of struct screen)
AC_CACHE_VAL(cf_cv_need_struct_screen,[
	if test $cf_cv_have_struct_screen = yes; then
		AC_TRY_COMPILE([#include <${cf_cv_ncurses_header-curses.h}>],
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
dnl CF_SUBDIR_PATH version: 3 updated: 2002/12/29 18:30:46
dnl --------------
dnl Construct a search-list for a nonstandard header/lib-file
dnl	$1 = the variable to return as result
dnl	$2 = the package name
dnl	$3 = the subdirectory, e.g., bin, include or lib
AC_DEFUN([CF_SUBDIR_PATH],
[$1=""

test -d [$]HOME && {
	test -n "$verbose" && echo "	... testing $3-directories under [$]HOME"
	test -d [$]HOME/$3 &&          $1="[$]$1 [$]HOME/$3"
	test -d [$]HOME/$3/$2 &&       $1="[$]$1 [$]HOME/$3/$2"
	test -d [$]HOME/$3/$2/$3 &&    $1="[$]$1 [$]HOME/$3/$2/$3"
}

# For other stuff under the home directory, it should be sufficient to put
# a symbolic link for $HOME/$2 to the actual package location:
test -d [$]HOME/$2 && {
	test -n "$verbose" && echo "	... testing $3-directories under [$]HOME/$2"
	test -d [$]HOME/$2/$3 &&       $1="[$]$1 [$]HOME/$2/$3"
	test -d [$]HOME/$2/$3/$2 &&    $1="[$]$1 [$]HOME/$2/$3/$2"
}

test "$prefix" != /usr/local && \
test -d /usr/local && {
	test -n "$verbose" && echo "	... testing $3-directories under /usr/local"
	test -d /usr/local/$3 &&       $1="[$]$1 /usr/local/$3"
	test -d /usr/local/$3/$2 &&    $1="[$]$1 /usr/local/$3/$2"
	test -d /usr/local/$3/$2/$3 && $1="[$]$1 /usr/local/$3/$2/$3"
	test -d /usr/local/$2/$3 &&    $1="[$]$1 /usr/local/$2/$3"
	test -d /usr/local/$2/$3/$2 && $1="[$]$1 /usr/local/$2/$3/$2"
}

test "$prefix" != NONE && \
test -d $prefix && {
	test -n "$verbose" && echo "	... testing $3-directories under $prefix"
	test -d $prefix/$3 &&          $1="[$]$1 $prefix/$3"
	test -d $prefix/$3/$2 &&       $1="[$]$1 $prefix/$3/$2"
	test -d $prefix/$3/$2/$3 &&    $1="[$]$1 $prefix/$3/$2/$3"
	test -d $prefix/$2/$3 &&       $1="[$]$1 $prefix/$2/$3"
	test -d $prefix/$2/$3/$2 &&    $1="[$]$1 $prefix/$2/$3/$2"
}

test "$prefix" != /opt && \
test -d /opt && {
	test -n "$verbose" && echo "	... testing $3-directories under /opt"
	test -d /opt/$3 &&             $1="[$]$1 /opt/$3"
	test -d /opt/$3/$2 &&          $1="[$]$1 /opt/$3/$2"
	test -d /opt/$3/$2/$3 &&       $1="[$]$1 /opt/$3/$2/$3"
	test -d /opt/$2/$3 &&          $1="[$]$1 /opt/$2/$3"
	test -d /opt/$2/$3/$2 &&       $1="[$]$1 /opt/$2/$3/$2"
}

test "$prefix" != /usr && \
test -d /usr && {
	test -n "$verbose" && echo "	... testing $3-directories under /usr"
	test -d /usr/$3 &&             $1="[$]$1 /usr/$3"
	test -d /usr/$3/$2 &&          $1="[$]$1 /usr/$3/$2"
	test -d /usr/$3/$2/$3 &&       $1="[$]$1 /usr/$3/$2/$3"
	test -d /usr/$2/$3 &&          $1="[$]$1 /usr/$2/$3"
}
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_SYS_ERRLIST version: 6 updated: 2001/12/30 13:03:23
dnl --------------
dnl Check for declaration of sys_nerr and sys_errlist in one of stdio.h and
dnl errno.h.  Declaration of sys_errlist on BSD4.4 interferes with our
dnl declaration.  Reported by Keith Bostic.
AC_DEFUN([CF_SYS_ERRLIST],
[
    CF_CHECK_ERRNO(sys_nerr)
    CF_CHECK_ERRNO(sys_errlist)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_TCAP_CURSOR version: 3 updated: 2003/11/06 19:59:57
dnl --------------
dnl Test if curses defines KD, KU, etc., for cursor keys
dnl
dnl	I found at least one implementation of curses that didn't declare these
dnl	variables in the include-file, but did define them in the library.  If
dnl	they're not in the include-file, ignore them.  Otherwise, assume that
dnl	curses initializes them in 'initscr()'.
dnl
AC_DEFUN([CF_TCAP_CURSOR],
[
AC_REQUIRE([CF_CURSES_CPPFLAGS])dnl
AC_MSG_CHECKING(for termcap-cursor variables)
AC_CACHE_VAL(cf_cv_termcap_cursor,[
	AC_TRY_COMPILE([#include <${cf_cv_ncurses_header-curses.h}>],
		[char *d=KD, *u=KU, *r=KR, *l=KL],
		[cf_cv_termcap_cursor=yes],
		[cf_cv_termcap_cursor=no])])
AC_MSG_RESULT($cf_cv_termcap_cursor)
test $cf_cv_termcap_cursor = yes && AC_DEFINE(HAVE_TCAP_CURSOR)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_TD_SIG_ARGS version: 3 updated: 2002/12/21 18:32:04
dnl --------------
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

if test -n "$GCC" && test "$cf_opt_with_warnings" = yes
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
dnl CF_TD_SRC_MAKEFILES version: 4 updated: 1997/09/13 12:04:36
dnl -------------------
dnl Append predefined lists to src/*/makefile.
dnl
dnl Also, make a series of "cd XXX && make" statements, which is understood by
dnl all "make -n" commands.
dnl
AC_DEFUN([CF_TD_SRC_MAKEFILES],
[
for cf_src in $cf_cv_src_modules
do
	CF_SRC_MAKEFILE($srcdir,src/$cf_src,../td_rules.mk)
	echo "	cd $cf_src &&	\$(MAKE) \$(MAKE_OPTS) \[$]@" >>src/makefile
done
test -f $srcdir/src/makefile.end && \
    cat $srcdir/src/makefile.end >>src/makefile
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_TD_SRC_MODULES version: 3 updated: 1997/09/11 23:08:06
dnl -----------------
dnl Make a list of src/*/modules so that AC_OUTPUT has the list on-hand.
AC_DEFUN([CF_TD_SRC_MODULES],
[
AC_MSG_CHECKING(for src modules)
AC_CACHE_VAL(cf_cv_src_modules,[
cf_cv_src_modules=""
cf_cv_src_makefiles=""
for p in $srcdir/src/*
do
	if test -d $p ; then
		if test -f $p/modules ; then
			p=`basename $p`
			cf_cv_src_modules="$cf_cv_src_modules $p"
			cf_cv_src_makefiles="$cf_cv_src_makefiles src/$p/makefile:$srcdir/src/sub_vars.in"
		fi
	fi
done
])
AC_MSG_RESULT($cf_cv_src_modules)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_TD_TEST_MAKEFILES version: 2 updated: 1997/09/11 23:08:06
dnl --------------------
dnl Append predefined lists to test/*/makefile.
dnl
dnl Also, make a series of "cd XXX && make" statements, which is understood by
dnl all "make -n" commands.
AC_DEFUN([CF_TD_TEST_MAKEFILES],
[
for p in $cf_cv_test_modules
do
	q=$srcdir/test/$p/modules
	cf_out=test/$p/makefile
	if test -f $q ; then
		mv $cf_out $cf_out.tmp
		echo "THIS=$p" >$cf_out
		echo >>$cf_out
		cat $cf_out.tmp >>$cf_out
		rm -f $cf_out.tmp
${AWK-awk} <$q >>$cf_out '
BEGIN	{
		found = 0;
	}
	{
		if ( found == 0 )
		{
			printf "\nREF_FILES="
			found = 1;
		}
		printf " \\\n\t%s.ref", [$]1
	}
'
${AWK-awk} <$q >>$cf_out '
BEGIN	{
		found = 0;
	}
	{
		if ( found == 0 )
		{
			printf "\n\nSCRIPTS= \\\n\trun_test.sh"
			found = 1;
		}
		printf " \\\n\t%s.sh", [$]1
	}
'
${AWK-awk} <$q >>$cf_out '
BEGIN	{
		found = 0;
	}
	{
		if ( found == 0 )
		{
			printf "\n\nPROGS="
			found = 1;
		}
		printf " \\\n\t%s", [$]1
	}
'
	cat >>$cf_out <<CF_EOF


${make_include_left}../td_rules.mk${make_include_right}

# Fix for SunOS VPATH

CF_EOF
${AWK-awk} <$q >>$cf_out '
	{
		printf "%s.c:\n", [$]1
	}
'
	echo "	cd $p &&	\$(MAKE) \$(MAKE_OPTS) \[$]@" >>test/makefile
	fi
done
test -f $srcdir/test/makefile.end && \
    cat $srcdir/test/makefile.end >>test/makefile
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_TD_TEST_MODULES version: 2 updated: 1997/09/11 23:08:06
dnl ------------------
dnl Make a list of test/*/modules so that AC_OUTPUT has the list on-hand.
AC_DEFUN([CF_TD_TEST_MODULES],
[
AC_MSG_CHECKING(for test modules)
AC_CACHE_VAL(cf_cv_test_modules,[
cf_cv_test_modules=""
cf_cv_test_makefiles=""
for p in $srcdir/test/*
do
	if test -d $p ; then
		if test -f $p/modules ; then
			p=`basename $p`
			cf_cv_test_modules="$cf_cv_test_modules $p"
			cf_cv_test_makefiles="$cf_cv_test_makefiles test/$p/makefile:$srcdir/test/sub_vars.in"
		fi
	fi
done
])
AC_MSG_RESULT($cf_cv_test_modules)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_TOP_SRCDIR version: 3 updated: 2001/07/15 16:04:43
dnl -------------
dnl Make an absolute symbol for the top of the configuration.
AC_DEFUN([CF_TOP_SRCDIR],
[
CDPATH=; export CDPATH
TOP_SRCDIR=`cd $srcdir;pwd`
AC_SUBST(TOP_SRCDIR)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_UPPER version: 5 updated: 2001/01/29 23:40:59
dnl --------
dnl Make an uppercase version of a variable
dnl $1=uppercase($2)
AC_DEFUN([CF_UPPER],
[
$1=`echo "$2" | sed y%abcdefghijklmnopqrstuvwxyz./-%ABCDEFGHIJKLMNOPQRSTUVWXYZ___%`
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_VARARGS version: 3 updated: 1998/12/10 20:06:29
dnl ----------
dnl Check for ANSI stdarg.h vs varargs.h.  Note that some systems include
dnl <varargs.h> within <stdarg.h>.
AC_DEFUN([CF_VARARGS],
[
AC_CHECK_HEADERS(stdarg.h varargs.h)
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
dnl CF_VERBOSE version: 2 updated: 1997/09/05 10:45:14
dnl ----------
dnl Use AC_VERBOSE w/o the warnings
AC_DEFUN([CF_VERBOSE],
[test -n "$verbose" && echo "	$1" 1>&AC_FD_MSG
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WAIT version: 2 updated: 1997/09/07 15:13:55
dnl -------
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
dnl ---------------------------------------------------------------------------
dnl CF_WITH_CURSES_DIR version: 2 updated: 2002/11/10 14:46:59
dnl ------------------
dnl Wrapper for AC_ARG_WITH to specify directory under which to look for curses
dnl libraries.
AC_DEFUN([CF_WITH_CURSES_DIR],[
AC_ARG_WITH(curses-dir,
	[  --with-curses-dir=DIR   directory in which (n)curses is installed],
	[CF_PATH_SYNTAX(withval)
	 cf_cv_curses_dir=$withval],
	[cf_cv_curses_dir=no])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_WARNINGS version: 5 updated: 2004/07/23 14:40:34
dnl ----------------
dnl Combine the checks for gcc features into a configure-script option
dnl
dnl Parameters:
dnl	$1 - see CF_GCC_WARNINGS
AC_DEFUN([CF_WITH_WARNINGS],
[
if ( test "$GCC" = yes || test "$GXX" = yes )
then
AC_MSG_CHECKING(if you want to check for gcc warnings)
AC_ARG_WITH(warnings,
	[  --with-warnings         test: turn on gcc warnings],
	[cf_opt_with_warnings=$withval],
	[cf_opt_with_warnings=no])
AC_MSG_RESULT($cf_opt_with_warnings)
if test "$cf_opt_with_warnings" != no ; then
	CF_GCC_ATTRIBUTES
	CF_GCC_WARNINGS([$1])
fi
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_XOPEN_CURSES version: 8 updated: 2003/11/07 19:47:46
dnl ---------------
dnl Test if we should define X/Open source for curses, needed on Digital Unix
dnl 4.x, to see the extended functions, but breaks on IRIX 6.x.
dnl
dnl The getbegyx() check is needed for HPUX, which omits legacy macros such
dnl as getbegy().  The latter is better design, but the former is standard.
AC_DEFUN([CF_XOPEN_CURSES],
[
AC_REQUIRE([CF_CURSES_CPPFLAGS])dnl
AC_CACHE_CHECK(if we must define _XOPEN_SOURCE_EXTENDED,cf_cv_need_xopen_extension,[
AC_TRY_LINK([
#include <stdlib.h>
#include <${cf_cv_ncurses_header-curses.h}>],[
	long x = winnstr(stdscr, "", 0);
	int x1, y1;
	getbegyx(stdscr, y1, x1)],
	[cf_cv_need_xopen_extension=no],
	[AC_TRY_LINK([
#define _XOPEN_SOURCE_EXTENDED
#include <stdlib.h>
#include <${cf_cv_ncurses_header-curses.h}>],[
	long x = winnstr(stdscr, "", 0);
	int x1, y1;
	getbegyx(stdscr, y1, x1)],
	[cf_cv_need_xopen_extension=yes],
	[cf_cv_need_xopen_extension=unknown])])])
test $cf_cv_need_xopen_extension = yes && CPPFLAGS="$CPPFLAGS -D_XOPEN_SOURCE_EXTENDED"
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_XOPEN_SOURCE version: 15 updated: 2004/11/23 15:41:32
dnl ---------------
dnl Try to get _XOPEN_SOURCE defined properly that we can use POSIX functions,
dnl or adapt to the vendor's definitions to get equivalent functionality.
dnl
dnl Parameters:
dnl	$1 is the nominal value for _XOPEN_SOURCE
dnl	$2 is the nominal value for _POSIX_C_SOURCE
AC_DEFUN([CF_XOPEN_SOURCE],[

cf_XOPEN_SOURCE=ifelse($1,,500,$1)
cf_POSIX_C_SOURCE=ifelse($2,,199506L,$2)

case $host_os in #(vi
freebsd*) #(vi
	# 5.x headers associate
	#	_XOPEN_SOURCE=600 with _POSIX_C_SOURCE=200112L
	#	_XOPEN_SOURCE=500 with _POSIX_C_SOURCE=199506L
	cf_POSIX_C_SOURCE=200112L
	cf_XOPEN_SOURCE=600
	CPPFLAGS="$CPPFLAGS -D_BSD_TYPES -D__BSD_VISIBLE -D_POSIX_C_SOURCE=$cf_POSIX_C_SOURCE -D_XOPEN_SOURCE=$cf_XOPEN_SOURCE"
	;;
hpux*) #(vi
	CPPFLAGS="$CPPFLAGS -D_HPUX_SOURCE"
	;;
irix[[56]].*) #(vi
	CPPFLAGS="$CPPFLAGS -D_SGI_SOURCE"
	;;
linux*|gnu*) #(vi
	CF_GNU_SOURCE
	;;
mirbsd*) #(vi
	# setting _XOPEN_SOURCE or _POSIX_SOURCE breaks <arpa/inet.h>
	;;
netbsd*) #(vi
	# setting _XOPEN_SOURCE breaks IPv6 for lynx on NetBSD 1.6, breaks xterm, is not needed for ncursesw
	;;
openbsd*) #(vi
	# setting _XOPEN_SOURCE breaks xterm on OpenBSD 2.8, is not needed for ncursesw
	;;
osf[[45]]*) #(vi
	CPPFLAGS="$CPPFLAGS -D_OSF_SOURCE"
	;;
sco*) #(vi
	# setting _XOPEN_SOURCE breaks Lynx on SCO Unix / OpenServer
	;;
solaris*) #(vi
	CPPFLAGS="$CPPFLAGS -D__EXTENSIONS__"
	;;
*)
	AC_CACHE_CHECK(if we should define _XOPEN_SOURCE,cf_cv_xopen_source,[
	AC_TRY_COMPILE([#include <sys/types.h>],[
#ifndef _XOPEN_SOURCE
make an error
#endif],
	[cf_cv_xopen_source=no],
	[cf_save="$CPPFLAGS"
	 CPPFLAGS="$CPPFLAGS -D_XOPEN_SOURCE=$cf_XOPEN_SOURCE"
	 AC_TRY_COMPILE([#include <sys/types.h>],[
#ifdef _XOPEN_SOURCE
make an error
#endif],
	[cf_cv_xopen_source=no],
	[cf_cv_xopen_source=$cf_XOPEN_SOURCE])
	CPPFLAGS="$cf_save"
	])
])
test "$cf_cv_xopen_source" != no && CPPFLAGS="$CPPFLAGS -D_XOPEN_SOURCE=$cf_cv_xopen_source"
	CF_POSIX_C_SOURCE($cf_POSIX_C_SOURCE)
	;;
esac
])
