dnl Extended Macros that test for specific features.
dnl $Id: aclocal.m4,v 12.230 2025/01/18 00:08:12 tom Exp $
dnl vi:set ts=4:
dnl ---------------------------------------------------------------------------
dnl
dnl Copyright 1994-2024,2025 by Thomas E. Dickey
dnl
dnl                         All Rights Reserved
dnl
dnl Permission is hereby granted, free of charge, to any person obtaining a
dnl copy of this software and associated documentation files (the
dnl "Software"), to deal in the Software without restriction, including
dnl without limitation the rights to use, copy, modify, merge, publish,
dnl distribute, sublicense, and/or sell copies of the Software, and to
dnl permit persons to whom the Software is furnished to do so, subject to
dnl the following conditions:
dnl
dnl The above copyright notice and this permission notice shall be included
dnl in all copies or substantial portions of the Software.
dnl
dnl THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
dnl OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
dnl MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
dnl IN NO EVENT SHALL THE ABOVE LISTED COPYRIGHT HOLDER(S) BE LIABLE FOR ANY
dnl CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
dnl TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
dnl SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
dnl
dnl Except as contained in this notice, the name(s) of the above copyright
dnl holders shall not be used in advertising or otherwise to promote the
dnl sale, use or other dealings in this Software without prior written
dnl authorization.
dnl
dnl ---------------------------------------------------------------------------
dnl See
dnl     https://invisible-island.net/autoconf/autoconf.html
dnl     https://invisible-island.net/autoconf/my-autoconf.html
dnl ---------------------------------------------------------------------------
dnl BELOW THIS LINE CAN BE PUT INTO "acspecific.m4", by changing "CF_" to "AC_"
dnl and "cf_" to "ac_".
dnl ---------------------------------------------------------------------------
dnl ---------------------------------------------------------------------------
dnl AM_LANGINFO_CODESET version: 7 updated: 2023/01/11 04:05:23
dnl -------------------
dnl Inserted as requested by gettext 0.10.40
dnl File from /usr/share/aclocal
dnl codeset.m4
dnl ====================
dnl serial AM1
dnl
dnl From Bruno Haible.
AC_DEFUN([AM_LANGINFO_CODESET],
[
AC_CACHE_CHECK([for nl_langinfo and CODESET], am_cv_langinfo_codeset,
	[AC_TRY_LINK([
$ac_includes_default
#include <langinfo.h>],
	[char* cs = nl_langinfo(CODESET); (void)cs],
	am_cv_langinfo_codeset=yes,
	am_cv_langinfo_codeset=no)
	])
	if test "$am_cv_langinfo_codeset" = yes; then
		AC_DEFINE(HAVE_LANGINFO_CODESET, 1,
		[Define if you have <langinfo.h> and nl_langinfo(CODESET).])
	fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ACVERSION_CHECK version: 5 updated: 2014/06/04 19:11:49
dnl ------------------
dnl Conditionally generate script according to whether we're using a given autoconf.
dnl
dnl $1 = version to compare against
dnl $2 = code to use if AC_ACVERSION is at least as high as $1.
dnl $3 = code to use if AC_ACVERSION is older than $1.
define([CF_ACVERSION_CHECK],
[
ifdef([AC_ACVERSION], ,[ifdef([AC_AUTOCONF_VERSION],[m4_copy([AC_AUTOCONF_VERSION],[AC_ACVERSION])],[m4_copy([m4_PACKAGE_VERSION],[AC_ACVERSION])])])dnl
ifdef([m4_version_compare],
[m4_if(m4_version_compare(m4_defn([AC_ACVERSION]), [$1]), -1, [$3], [$2])],
[CF_ACVERSION_COMPARE(
AC_PREREQ_CANON(AC_PREREQ_SPLIT([$1])),
AC_PREREQ_CANON(AC_PREREQ_SPLIT(AC_ACVERSION)), AC_ACVERSION, [$2], [$3])])])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ACVERSION_COMPARE version: 3 updated: 2012/10/03 18:39:53
dnl --------------------
dnl CF_ACVERSION_COMPARE(MAJOR1, MINOR1, TERNARY1,
dnl                      MAJOR2, MINOR2, TERNARY2,
dnl                      PRINTABLE2, not FOUND, FOUND)
define([CF_ACVERSION_COMPARE],
[ifelse(builtin([eval], [$2 < $5]), 1,
[ifelse([$8], , ,[$8])],
[ifelse([$9], , ,[$9])])])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ADD_CFLAGS version: 15 updated: 2020/12/31 10:54:15
dnl -------------
dnl Copy non-preprocessor flags to $CFLAGS, preprocessor flags to $CPPFLAGS
dnl $1 = flags to add
dnl $2 = if given makes this macro verbose.
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
case "$cf_fix_cppflags" in
(no)
	case "$cf_add_cflags" in
	(-undef|-nostdinc*|-I*|-D*|-U*|-E|-P|-C)
		case "$cf_add_cflags" in
		(-D*)
			cf_tst_cflags=`echo "${cf_add_cflags}" |sed -e 's/^-D[[^=]]*='\''\"[[^"]]*//'`

			test "x${cf_add_cflags}" != "x${cf_tst_cflags}" \
				&& test -z "${cf_tst_cflags}" \
				&& cf_fix_cppflags=yes

			if test "$cf_fix_cppflags" = yes ; then
				CF_APPEND_TEXT(cf_new_extra_cppflags,$cf_add_cflags)
				continue
			elif test "${cf_tst_cflags}" = "\"'" ; then
				CF_APPEND_TEXT(cf_new_extra_cppflags,$cf_add_cflags)
				continue
			fi
			;;
		esac
		case "$CPPFLAGS" in
		(*$cf_add_cflags)
			;;
		(*)
			case "$cf_add_cflags" in
			(-D*)
				cf_tst_cppflags=`echo "x$cf_add_cflags" | sed -e 's/^...//' -e 's/=.*//'`
				CF_REMOVE_DEFINE(CPPFLAGS,$CPPFLAGS,$cf_tst_cppflags)
				;;
			esac
			CF_APPEND_TEXT(cf_new_cppflags,$cf_add_cflags)
			;;
		esac
		;;
	(*)
		CF_APPEND_TEXT(cf_new_cflags,$cf_add_cflags)
		;;
	esac
	;;
(yes)
	CF_APPEND_TEXT(cf_new_extra_cppflags,$cf_add_cflags)

	cf_tst_cflags=`echo "${cf_add_cflags}" |sed -e 's/^[[^"]]*"'\''//'`

	test "x${cf_add_cflags}" != "x${cf_tst_cflags}" \
		&& test -z "${cf_tst_cflags}" \
		&& cf_fix_cppflags=no
	;;
esac
done

if test -n "$cf_new_cflags" ; then
	ifelse([$2],,,[CF_VERBOSE(add to \$CFLAGS $cf_new_cflags)])
	CF_APPEND_TEXT(CFLAGS,$cf_new_cflags)
fi

if test -n "$cf_new_cppflags" ; then
	ifelse([$2],,,[CF_VERBOSE(add to \$CPPFLAGS $cf_new_cppflags)])
	CF_APPEND_TEXT(CPPFLAGS,$cf_new_cppflags)
fi

if test -n "$cf_new_extra_cppflags" ; then
	ifelse([$2],,,[CF_VERBOSE(add to \$EXTRA_CPPFLAGS $cf_new_extra_cppflags)])
	CF_APPEND_TEXT(EXTRA_CPPFLAGS,$cf_new_extra_cppflags)
fi

AC_SUBST(EXTRA_CPPFLAGS)

])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ADD_INCDIR version: 17 updated: 2021/09/04 06:35:04
dnl -------------
dnl Add an include-directory to $CPPFLAGS.  Don't add /usr/include, since it is
dnl redundant.  We don't normally need to add -I/usr/local/include for gcc,
dnl but old versions (and some misinstalled ones) need that.  To make things
dnl worse, gcc 3.x may give error messages if -I/usr/local/include is added to
dnl the include-path).
AC_DEFUN([CF_ADD_INCDIR],
[
if test -n "$1" ; then
  for cf_add_incdir in $1
  do
	while test "$cf_add_incdir" != /usr/include
	do
	  if test -d "$cf_add_incdir"
	  then
		cf_have_incdir=no
		if test -n "$CFLAGS$CPPFLAGS" ; then
		  # a loop is needed to ensure we can add subdirs of existing dirs
		  for cf_test_incdir in $CFLAGS $CPPFLAGS ; do
			if test ".$cf_test_incdir" = ".-I$cf_add_incdir" ; then
			  cf_have_incdir=yes; break
			fi
		  done
		fi

		if test "$cf_have_incdir" = no ; then
		  if test "$cf_add_incdir" = /usr/local/include ; then
			if test "$GCC" = yes
			then
			  cf_save_CPPFLAGS=$CPPFLAGS
			  CF_APPEND_TEXT(CPPFLAGS,-I$cf_add_incdir)
			  AC_TRY_COMPILE([#include <stdio.h>],
				  [printf("Hello")],
				  [],
				  [cf_have_incdir=yes])
			  CPPFLAGS=$cf_save_CPPFLAGS
			fi
		  fi
		fi

		if test "$cf_have_incdir" = no ; then
		  CF_VERBOSE(adding $cf_add_incdir to include-path)
		  ifelse([$2],,CPPFLAGS,[$2])="$ifelse([$2],,CPPFLAGS,[$2]) -I$cf_add_incdir"

		  cf_top_incdir=`echo "$cf_add_incdir" | sed -e 's%/include/.*$%/include%'`
		  test "$cf_top_incdir" = "$cf_add_incdir" && break
		  cf_add_incdir="$cf_top_incdir"
		else
		  break
		fi
	  else
		break
	  fi
	done
  done
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ADD_LIB version: 2 updated: 2010/06/02 05:03:05
dnl ----------
dnl Add a library, used to enforce consistency.
dnl
dnl $1 = library to add, without the "-l"
dnl $2 = variable to update (default $LIBS)
AC_DEFUN([CF_ADD_LIB],[CF_ADD_LIBS(-l$1,ifelse($2,,LIBS,[$2]))])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ADD_LIBDIR version: 11 updated: 2020/12/31 20:19:42
dnl -------------
dnl	Adds to the library-path
dnl
dnl	Some machines have trouble with multiple -L options.
dnl
dnl $1 is the (list of) directory(s) to add
dnl $2 is the optional name of the variable to update (default LDFLAGS)
dnl
AC_DEFUN([CF_ADD_LIBDIR],
[
if test -n "$1" ; then
	for cf_add_libdir in $1
	do
		if test "$cf_add_libdir" = /usr/lib ; then
			:
		elif test -d "$cf_add_libdir"
		then
			cf_have_libdir=no
			if test -n "$LDFLAGS$LIBS" ; then
				# a loop is needed to ensure we can add subdirs of existing dirs
				for cf_test_libdir in $LDFLAGS $LIBS ; do
					if test ".$cf_test_libdir" = ".-L$cf_add_libdir" ; then
						cf_have_libdir=yes; break
					fi
				done
			fi
			if test "$cf_have_libdir" = no ; then
				CF_VERBOSE(adding $cf_add_libdir to library-path)
				ifelse([$2],,LDFLAGS,[$2])="-L$cf_add_libdir $ifelse([$2],,LDFLAGS,[$2])"
			fi
		fi
	done
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ADD_LIBS version: 3 updated: 2019/11/02 16:47:33
dnl -----------
dnl Add one or more libraries, used to enforce consistency.  Libraries are
dnl prepended to an existing list, since their dependencies are assumed to
dnl already exist in the list.
dnl
dnl $1 = libraries to add, with the "-l", etc.
dnl $2 = variable to update (default $LIBS)
AC_DEFUN([CF_ADD_LIBS],[
cf_add_libs="[$]ifelse($2,,LIBS,[$2])"
# reverse order
cf_add_0lib=
for cf_add_1lib in $1; do cf_add_0lib="$cf_add_1lib $cf_add_0lib"; done
# filter duplicates
for cf_add_1lib in $cf_add_0lib; do
	for cf_add_2lib in $cf_add_libs; do
		if test "x$cf_add_1lib" = "x$cf_add_2lib"; then
			cf_add_1lib=
			break
		fi
	done
	test -n "$cf_add_1lib" && cf_add_libs="$cf_add_1lib $cf_add_libs"
done
ifelse($2,,LIBS,[$2])="$cf_add_libs"
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ADD_LIB_AFTER version: 3 updated: 2013/07/09 21:27:22
dnl ----------------
dnl Add a given library after another, e.g., following the one it satisfies a
dnl dependency for.
dnl
dnl $1 = the first library
dnl $2 = its dependency
AC_DEFUN([CF_ADD_LIB_AFTER],[
CF_VERBOSE(...before $LIBS)
LIBS=`echo "$LIBS" | sed -e "s/[[ 	]][[ 	]]*/ /g" -e "s%$1 %$1 $2 %" -e 's%  % %g'`
CF_VERBOSE(...after  $LIBS)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ADD_SUBDIR_PATH version: 5 updated: 2020/12/31 20:19:42
dnl ------------------
dnl Append to a search-list for a nonstandard header/lib-file
dnl	$1 = the variable to return as result
dnl	$2 = the package name
dnl	$3 = the subdirectory, e.g., bin, include or lib
dnl $4 = the directory under which we will test for subdirectories
dnl $5 = a directory that we do not want $4 to match
AC_DEFUN([CF_ADD_SUBDIR_PATH],
[
test "x$4" != "x$5" && \
test -d "$4" && \
ifelse([$5],NONE,,[{ test -z "$5" || test "x$5" = xNONE || test "x$4" != "x$5"; } &&]) {
	test -n "$verbose" && echo "	... testing for $3-directories under $4"
	test -d "$4/$3" &&          $1="[$]$1 $4/$3"
	test -d "$4/$3/$2" &&       $1="[$]$1 $4/$3/$2"
	test -d "$4/$3/$2/$3" &&    $1="[$]$1 $4/$3/$2/$3"
	test -d "$4/$2/$3" &&       $1="[$]$1 $4/$2/$3"
	test -d "$4/$2/$3/$2" &&    $1="[$]$1 $4/$2/$3/$2"
}
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ANSI_CPP version: 7 updated: 2023/01/05 18:56:48
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
if test "$cf_cv_ansi_splice" = new; then
	AC_DEFINE(HAVE_NEW_TOKEN_SPLICE,1,[Define to 1 if C preprocessor does ANSI splicing])
elif test "$cf_cv_ansi_splice" = old; then
	AC_DEFINE(HAVE_OLD_TOKEN_SPLICE,1,[Define to 1 if C preprocessor does not do ANSI splicing])
fi

AC_MSG_CHECKING(for ANSI CPP token-quoting)
AC_CACHE_VAL(cf_cv_ansi_quote,[
	AC_TRY_RUN([#define quote(name) #name
		int main(void) { char *y = quote(a); ${cf_cv_main_return:-return} (*y != 'a');} ],
		[cf_cv_ansi_quote=new],
		[cf_cv_ansi_quote=unknown],
		[cf_cv_ansi_quote=unknown])
	if test "$cf_cv_ansi_quote" = unknown; then
		AC_TRY_RUN([#define quote(name) "name"
			int main(void) { char *y = quote(a); ${cf_cv_main_return:-return} (*y != 'a');} ],
			[cf_cv_ansi_quote=old],
			[cf_cv_ansi_quote=unknown],
			[cf_cv_ansi_quote=unknown])
		fi
	])

AC_MSG_RESULT(${cf_cv_ansi_quote}-style)
if test "$cf_cv_ansi_quote" = new; then
	AC_DEFINE(HAVE_NEW_TOKEN_QUOTE,1,[Define to 1 if C preprocessor does ANSI quoting])
elif test "$cf_cv_ansi_quote" = old; then
	AC_DEFINE(HAVE_OLD_TOKEN_QUOTE,1,[Define to 1 if C preprocessor does not do ANSI quoting])
fi

])dnl
dnl ---------------------------------------------------------------------------
dnl CF_APPEND_CFLAGS version: 3 updated: 2021/09/05 17:25:40
dnl ----------------
dnl Use CF_ADD_CFLAGS after first checking for potential redefinitions.
dnl $1 = flags to add
dnl $2 = if given makes this macro verbose.
define([CF_APPEND_CFLAGS],
[
for cf_add_cflags in $1
do
	case "x$cf_add_cflags" in
	(x-[[DU]]*)
		CF_REMOVE_CFLAGS($cf_add_cflags,CFLAGS,[$2])
		CF_REMOVE_CFLAGS($cf_add_cflags,CPPFLAGS,[$2])
		;;
	esac
	CF_ADD_CFLAGS([$cf_add_cflags],[$2])
done
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_APPEND_TEXT version: 1 updated: 2017/02/25 18:58:55
dnl --------------
dnl use this macro for appending text without introducing an extra blank at
dnl the beginning
define([CF_APPEND_TEXT],
[
	test -n "[$]$1" && $1="[$]$1 "
	$1="[$]{$1}$2"
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ARG_DISABLE version: 3 updated: 1999/03/30 17:24:31
dnl --------------
dnl Allow user to disable a normally-on option.
AC_DEFUN([CF_ARG_DISABLE],
[CF_ARG_OPTION($1,[$2],[$3],[$4],yes)])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ARG_ENABLE version: 3 updated: 1999/03/30 17:24:31
dnl -------------
dnl Allow user to enable a normally-off option.
AC_DEFUN([CF_ARG_ENABLE],
[CF_ARG_OPTION($1,[$2],[$3],[$4],no)])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ARG_OPTION version: 5 updated: 2015/05/10 19:52:14
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
[AC_ARG_ENABLE([$1],[$2],[test "$enableval" != ifelse([$5],no,yes,no) && enableval=ifelse([$5],no,no,yes)
	if test "$enableval" != "$5" ; then
ifelse([$3],,[    :]dnl
,[    $3]) ifelse([$4],,,[
	else
		$4])
	fi],[enableval=$5 ifelse([$4],,,[
	$4
])dnl
])])dnl
dnl ---------------------------------------------------------------------------
dnl CF_AR_FLAGS version: 9 updated: 2021/01/01 13:31:04
dnl -----------
dnl Check for suitable "ar" (archiver) options for updating an archive.
dnl
dnl In particular, handle some obsolete cases where the "-" might be omitted,
dnl as well as a workaround for breakage of make's archive rules by the GNU
dnl binutils "ar" program.
AC_DEFUN([CF_AR_FLAGS],[
AC_REQUIRE([CF_PROG_AR])

AC_CACHE_CHECK(for options to update archives, cf_cv_ar_flags,[
	case "$cf_cv_system_name" in
	(*-msvc*)
		cf_cv_ar_flags=''
		cat >mk_static_lib.sh <<-EOF
		#!$SHELL
		MSVC_BIN="[$]AR"
		out="\[$]1"
		shift
		exec \[$]MSVC_BIN -out:"\[$]out" \[$]@
		EOF
		chmod +x mk_static_lib.sh
		AR=`pwd`/mk_static_lib.sh
		;;
	(*)
		cf_cv_ar_flags=unknown
		for cf_ar_flags in -curvU -curv curv -crv crv -cqv cqv -rv rv
		do

			# check if $ARFLAGS already contains this choice
			if test "x$ARFLAGS" != "x" ; then
				cf_check_ar_flags=`echo "x$ARFLAGS" | sed -e "s/$cf_ar_flags\$//" -e "s/$cf_ar_flags / /"`
				if test "x$ARFLAGS" != "$cf_check_ar_flags" ; then
					cf_cv_ar_flags=
					break
				fi
			fi

			rm -f "conftest.$ac_cv_objext"
			rm -f conftest.a

			cat >"conftest.$ac_ext" <<EOF
#line __oline__ "configure"
int	testdata[[3]] = { 123, 456, 789 };
EOF
			if AC_TRY_EVAL(ac_compile) ; then
				echo "$AR $ARFLAGS $cf_ar_flags conftest.a conftest.$ac_cv_objext" >&AC_FD_CC
				$AR $ARFLAGS "$cf_ar_flags" conftest.a "conftest.$ac_cv_objext" 2>&AC_FD_CC 1>/dev/null
				if test -f conftest.a ; then
					cf_cv_ar_flags="$cf_ar_flags"
					break
				fi
			else
				CF_VERBOSE(cannot compile test-program)
				break
			fi
		done
		rm -f conftest.a "conftest.$ac_ext" "conftest.$ac_cv_objext"
		;;
	esac
])

if test -n "$ARFLAGS" ; then
	if test -n "$cf_cv_ar_flags" ; then
		ARFLAGS="$ARFLAGS $cf_cv_ar_flags"
	fi
else
	ARFLAGS=$cf_cv_ar_flags
fi

AC_SUBST(ARFLAGS)
])
dnl ---------------------------------------------------------------------------
dnl CF_BUILD_DIR version: 1 updated: 2020/12/06 15:33:41
dnl ------------
dnl Make an absolute symbol for the top of the build-tree.
AC_DEFUN([CF_BUILD_DIR],
[
CDPATH=; export CDPATH
BUILD_DIR=`pwd`
AC_SUBST(BUILD_DIR)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_C11_NORETURN version: 4 updated: 2023/02/18 17:41:25
dnl ---------------
AC_DEFUN([CF_C11_NORETURN],
[
AC_MSG_CHECKING(if you want to use C11 _Noreturn feature)
CF_ARG_ENABLE(stdnoreturn,
	[  --enable-stdnoreturn    enable C11 _Noreturn feature for diagnostics],
	[enable_stdnoreturn=yes],
	[enable_stdnoreturn=no])
AC_MSG_RESULT($enable_stdnoreturn)

if test $enable_stdnoreturn = yes; then
AC_CACHE_CHECK([for C11 _Noreturn feature], cf_cv_c11_noreturn,
	[AC_TRY_COMPILE([
$ac_includes_default
#include <stdnoreturn.h>
static _Noreturn void giveup(void) { exit(0); }
	],
	[if (feof(stdin)) giveup()],
	cf_cv_c11_noreturn=yes,
	cf_cv_c11_noreturn=no)
	])
else
	cf_cv_c11_noreturn=no,
fi

if test "$cf_cv_c11_noreturn" = yes; then
	AC_DEFINE(HAVE_STDNORETURN_H, 1,[Define if <stdnoreturn.h> header is available and working])
	AC_DEFINE_UNQUOTED(STDC_NORETURN,_Noreturn,[Define if C11 _Noreturn keyword is supported])
	HAVE_STDNORETURN_H=1
else
	HAVE_STDNORETURN_H=0
fi

AC_SUBST(HAVE_STDNORETURN_H)
AC_SUBST(STDC_NORETURN)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CC_ENV_FLAGS version: 11 updated: 2023/02/20 11:15:46
dnl ---------------
dnl Check for user's environment-breakage by stuffing CFLAGS/CPPFLAGS content
dnl into CC.  This will not help with broken scripts that wrap the compiler
dnl with options, but eliminates a more common category of user confusion.
dnl
dnl In particular, it addresses the problem of being able to run the C
dnl preprocessor in a consistent manner.
dnl
dnl Caveat: this also disallows blanks in the pathname for the compiler, but
dnl the nuisance of having inconsistent settings for compiler and preprocessor
dnl outweighs that limitation.
AC_DEFUN([CF_CC_ENV_FLAGS],
[
# This should have been defined by AC_PROG_CC
: "${CC:=cc}"

AC_MSG_CHECKING(\$CFLAGS variable)
case "x$CFLAGS" in
(*-[[IUD]]*)
	AC_MSG_RESULT(broken)
	AC_MSG_WARN(your environment uses the CFLAGS variable to hold CPPFLAGS options)
	cf_flags="$CFLAGS"
	CFLAGS=
	for cf_arg in $cf_flags
	do
		CF_ADD_CFLAGS($cf_arg)
	done
	;;
(*)
	AC_MSG_RESULT(ok)
	;;
esac

AC_MSG_CHECKING(\$CC variable)
case "$CC" in
(*[[\ \	]]-*)
	AC_MSG_RESULT(broken)
	AC_MSG_WARN(your environment uses the CC variable to hold CFLAGS/CPPFLAGS options)
	# humor him...
	cf_prog=`echo "$CC" | sed -e 's/	/ /g' -e 's/[[ ]]* / /g' -e 's/[[ ]]*[[ ]]-[[^ ]].*//'`
	cf_flags=`echo "$CC" | sed -e "s%^$cf_prog%%"`
	CC="$cf_prog"
	for cf_arg in $cf_flags
	do
		case "x$cf_arg" in
		(x-[[IUDfgOW]]*)
			CF_ADD_CFLAGS($cf_arg)
			;;
		(*)
			CC="$CC $cf_arg"
			;;
		esac
	done
	CF_VERBOSE(resulting CC: '$CC')
	CF_VERBOSE(resulting CFLAGS: '$CFLAGS')
	CF_VERBOSE(resulting CPPFLAGS: '$CPPFLAGS')
	;;
(*)
	AC_MSG_RESULT(ok)
	;;
esac
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CHECK_CACHE version: 13 updated: 2020/12/31 10:54:15
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
if test -f "$srcdir/config.guess" || test -f "$ac_aux_dir/config.guess" ; then
	ifelse([$1],,[AC_CANONICAL_HOST],[$1])
	system_name="$host_os"
else
	system_name="`(uname -s -r) 2>/dev/null`"
	if test -z "$system_name" ; then
		system_name="`(hostname) 2>/dev/null`"
	fi
fi
test -n "$system_name" && AC_DEFINE_UNQUOTED(SYSTEM_NAME,"$system_name",[Define to the system name.])
AC_CACHE_VAL(cf_cv_system_name,[cf_cv_system_name="$system_name"])

test -z "$system_name" && system_name="$cf_cv_system_name"
test -n "$cf_cv_system_name" && AC_MSG_RESULT(Configuring for $cf_cv_system_name)

if test ".$system_name" != ".$cf_cv_system_name" ; then
	AC_MSG_RESULT(Cached system name ($system_name) does not agree with actual ($cf_cv_system_name))
	AC_MSG_ERROR("Please remove config.cache and try again.")
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CHECK_CFLAGS version: 4 updated: 2021/01/02 19:22:58
dnl ---------------
dnl Conditionally add to $CFLAGS and $CPPFLAGS values which are derived from
dnl a build-configuration such as imake.  These have the pitfall that they
dnl often contain compiler-specific options which we cannot use, mixed with
dnl preprocessor options that we usually can.
AC_DEFUN([CF_CHECK_CFLAGS],
[
CF_VERBOSE(checking additions to CFLAGS)
cf_check_cflags="$CFLAGS"
cf_check_cppflags="$CPPFLAGS"
CF_ADD_CFLAGS($1,yes)
if test "x$cf_check_cflags" != "x$CFLAGS" ; then
AC_TRY_LINK([#include <stdio.h>],[printf("Hello world");],,
	[CF_VERBOSE(test-compile failed.  Undoing change to \$CFLAGS)
	 if test "x$cf_check_cppflags" != "x$CPPFLAGS" ; then
		 CF_VERBOSE(but keeping change to \$CPPFLAGS)
	 fi
	 CFLAGS="$cf_check_cflags"])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CHECK_ERRNO version: 14 updated: 2023/02/18 17:41:25
dnl --------------
dnl Check for data that is usually declared in <stdio.h> or <errno.h>, e.g.,
dnl the 'errno' variable.  Define a DECL_xxx symbol if we must declare it
dnl ourselves.
dnl
dnl $1 = the name to check
dnl $2 = the assumed type
AC_DEFUN([CF_CHECK_ERRNO],
[
AC_CACHE_CHECK(if external $1 is declared, cf_cv_dcl_$1,[
	AC_TRY_COMPILE([
$ac_includes_default
#include <errno.h> ],
	ifelse([$2],,int,[$2]) x = (ifelse([$2],,int,[$2])) $1; (void)x,
	[cf_cv_dcl_$1=yes],
	[cf_cv_dcl_$1=no])
])

if test "$cf_cv_dcl_$1" = no ; then
	CF_UPPER(cf_result,decl_$1)
	AC_DEFINE_UNQUOTED($cf_result)
fi

# It's possible (for near-UNIX clones) that the data doesn't exist
CF_CHECK_EXTERN_DATA($1,ifelse([$2],,int,[$2]))
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CHECK_EXTERN_DATA version: 5 updated: 2021/09/04 06:35:04
dnl --------------------
dnl Check for existence of external data in the current set of libraries.  If
dnl we can modify it, it is real enough.
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
dnl CF_CHECK_FUNCDECL version: 8 updated: 2023/01/24 04:22:19
dnl -----------------
dnl Check if a function is declared by including a set of include files.
dnl Invoke the corresponding actions according to whether it is found or not.
dnl
dnl Gcc (unlike other compilers) will only warn about the miscast assignment
dnl in the first test, but most compilers will oblige with an error in the
dnl second test.
dnl
dnl CF_CHECK_FUNCDECL(INCLUDES, FUNCTION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
AC_DEFUN([CF_CHECK_FUNCDECL],
[
AC_MSG_CHECKING([for $2 declaration])
AC_CACHE_VAL(ac_cv_func_decl_$2,
[AC_TRY_COMPILE([
$ac_includes_default
$1],
[#ifndef $2
extern	int	$2(void);
#endif],[
AC_TRY_COMPILE([$1],
[#ifndef $2
int	(*p)(struct cf_check_funcdecl*) = $2;
#endif],[
eval "ac_cv_func_decl_$2=yes"],[
eval "ac_cv_func_decl_$2=no"])],[
eval "ac_cv_func_decl_$2=yes"])])
if eval "test \"`echo '$ac_cv_func_'decl_$2`\" = yes"; then
  AC_MSG_RESULT(yes)
  ifelse([$3], , :, [$3])
else
  AC_MSG_RESULT(no)
ifelse([$4], , , [$4
])dnl
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CHECK_REGEX version: 3 updated: 2021/01/10 18:23:00
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
if	test "$cf_cv_REGEX_H" = no && \
	test "$cf_cv_REGEXPR_H" = no && \
	test "$cf_cv_RE_COMP_func" = no ; then
	CF_REGCMP_FUNCS
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CLANG_COMPILER version: 9 updated: 2023/02/18 17:41:25
dnl -----------------
dnl Check if the given compiler is really clang.  clang's C driver defines
dnl __GNUC__ (fooling the configure script into setting $GCC to yes) but does
dnl not ignore some gcc options.
dnl
dnl This macro should be run "soon" after AC_PROG_CC or AC_PROG_CPLUSPLUS, to
dnl ensure that it is not mistaken for gcc/g++.  It is normally invoked from
dnl the wrappers for gcc and g++ warnings.
dnl
dnl $1 = GCC (default) or GXX
dnl $2 = CLANG_COMPILER (default)
dnl $3 = CFLAGS (default) or CXXFLAGS
AC_DEFUN([CF_CLANG_COMPILER],[
ifelse([$2],,CLANG_COMPILER,[$2])=no

if test "$ifelse([$1],,[$1],GCC)" = yes ; then
	AC_MSG_CHECKING(if this is really Clang ifelse([$1],GXX,C++,C) compiler)
	cf_save_CFLAGS="$ifelse([$3],,CFLAGS,[$3])"
	AC_TRY_COMPILE([],[
#ifdef __clang__
#else
#error __clang__ is not defined
#endif
],[ifelse([$2],,CLANG_COMPILER,[$2])=yes
],[])
	ifelse([$3],,CFLAGS,[$3])="$cf_save_CFLAGS"
	AC_MSG_RESULT($ifelse([$2],,CLANG_COMPILER,[$2]))
fi

CLANG_VERSION=none

if test "x$ifelse([$2],,CLANG_COMPILER,[$2])" = "xyes" ; then
	case "$CC" in
	(c[[1-9]][[0-9]]|*/c[[1-9]][[0-9]])
		AC_MSG_WARN(replacing broken compiler alias $CC)
		CFLAGS="$CFLAGS -std=`echo "$CC" | sed -e 's%.*/%%'`"
		CC=clang
		;;
	esac

	AC_MSG_CHECKING(version of $CC)
	CLANG_VERSION="`$CC --version 2>/dev/null | sed -e '2,$d' -e 's/^.*(CLANG[[^)]]*) //' -e 's/^.*(Debian[[^)]]*) //' -e 's/^[[^0-9.]]*//' -e 's/[[^0-9.]].*//'`"
	test -z "$CLANG_VERSION" && CLANG_VERSION=unknown
	AC_MSG_RESULT($CLANG_VERSION)

	for cf_clang_opt in \
		-Qunused-arguments \
		-Wno-error=implicit-function-declaration
	do
		AC_MSG_CHECKING(if option $cf_clang_opt works)
		cf_save_CFLAGS="$CFLAGS"
		CFLAGS="$CFLAGS $cf_clang_opt"
		AC_TRY_LINK([
			#include <stdio.h>],[
			printf("hello!\\n");],[
			cf_clang_optok=yes],[
			cf_clang_optok=no])
		AC_MSG_RESULT($cf_clang_optok)
		CFLAGS="$cf_save_CFLAGS"
		if test "$cf_clang_optok" = yes; then
			CF_VERBOSE(adding option $cf_clang_opt)
			CF_APPEND_TEXT(CFLAGS,$cf_clang_opt)
		fi
	done
fi
])
dnl ---------------------------------------------------------------------------
dnl CF_CONST_X_STRING version: 9 updated: 2024/12/04 03:49:57
dnl -----------------
dnl The X11R4-X11R6 Xt specification uses an ambiguous String type for most
dnl character-strings.
dnl
dnl It is ambiguous because the specification accommodated the pre-ANSI
dnl compilers bundled by more than one vendor in lieu of providing a standard C
dnl compiler other than by costly add-ons.  Because of this, the specification
dnl did not take into account the use of const for telling the compiler that
dnl string literals would be in readonly memory.
dnl
dnl As a workaround, one could (starting with X11R5) define XTSTRINGDEFINES, to
dnl let the compiler decide how to represent Xt's strings which were #define'd.
dnl That does not solve the problem of using the block of Xt's strings which
dnl are compiled into the library (and is less efficient than one might want).
dnl
dnl Xt specification 7 introduces the _CONST_X_STRING symbol which is used both
dnl when compiling the library and compiling using the library, to tell the
dnl compiler that String is const.
AC_DEFUN([CF_CONST_X_STRING],
[
AC_REQUIRE([AC_PATH_XTRA])

CF_SAVE_XTRA_FLAGS([CF_CONST_X_STRING])

AC_TRY_COMPILE(
[
$ac_includes_default
#include <X11/Intrinsic.h>
],
[String foo = malloc(1); free((void*)foo)],[

AC_CACHE_CHECK(for X11/Xt const-feature,cf_cv_const_x_string,[
	AC_TRY_COMPILE(
		[
#undef  _CONST_X_STRING
#define _CONST_X_STRING	/* X11R7.8 (perhaps) */
#undef  XTSTRINGDEFINES	/* X11R5 and later */
$ac_includes_default
#include <X11/Intrinsic.h>
		],[String foo = malloc(1); *foo = 0],[
			cf_cv_const_x_string=no
		],[
			cf_cv_const_x_string=yes
		])
])

CF_RESTORE_XTRA_FLAGS([CF_CONST_X_STRING])

case "$cf_cv_const_x_string" in
(no)
	CF_APPEND_TEXT(CPPFLAGS,-DXTSTRINGDEFINES)
	;;
(*)
	CF_APPEND_TEXT(CPPFLAGS,-D_CONST_X_STRING)
	;;
esac

])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CURSES_CCHAR_T version: 7 updated: 2021/01/10 18:23:00
dnl -----------------
dnl Test if curses defines 'cchar_t' (a 'struct' type for X/Open curses).
AC_DEFUN([CF_CURSES_CCHAR_T],
[
AC_REQUIRE([CF_CURSES_CPPFLAGS])dnl
AC_CACHE_CHECK(for cchar_t typedef,cf_cv_cchar_t_decl,[
	AC_TRY_COMPILE([#include <${cf_cv_ncurses_header:-curses.h}>],
		[cchar_t foo],
		[cf_cv_cchar_t_decl=yes],
		[cf_cv_cchar_t_decl=no])])
if test "$cf_cv_cchar_t_decl" = yes ; then
	AC_DEFINE(HAVE_TYPE_CCHAR_T,1,[Define to 1 if cchar_t is declared])
	AC_CACHE_CHECK(if cchar_t is scalar or struct,cf_cv_cchar_t_type,[
		AC_TRY_COMPILE([#include <${cf_cv_ncurses_header:-curses.h}>],
			[cchar_t foo; long x = foo],
			[cf_cv_cchar_t_type=scalar],
			[cf_cv_cchar_t_type=struct])])
	if test "$cf_cv_cchar_t_type" = scalar ; then
		AC_DEFINE(TYPE_CCHAR_T_IS_SCALAR,1,[Define to 1 if cchar_t is a scalar])
	fi
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CURSES_CHTYPE version: 11 updated: 2021/01/02 09:31:20
dnl ----------------
dnl Test if curses defines 'chtype' (usually a 'long' type for SysV curses).
AC_DEFUN([CF_CURSES_CHTYPE],
[
AC_REQUIRE([CF_CURSES_CPPFLAGS])dnl
AC_CACHE_CHECK(for chtype typedef,cf_cv_chtype_decl,[
	AC_TRY_COMPILE([#include <${cf_cv_ncurses_header:-curses.h}>],
		[chtype foo; (void)foo],
		[cf_cv_chtype_decl=yes],
		[cf_cv_chtype_decl=no])])
if test "$cf_cv_chtype_decl" = yes ; then
	AC_DEFINE(HAVE_TYPE_CHTYPE,1,[Define to 1 if chtype is declared])
	AC_CACHE_CHECK(if chtype is scalar or struct,cf_cv_chtype_type,[
		AC_TRY_COMPILE([#include <${cf_cv_ncurses_header:-curses.h}>],
			[static chtype foo; long x = foo; (void)x],
			[cf_cv_chtype_type=scalar],
			[cf_cv_chtype_type=struct])])
	if test "$cf_cv_chtype_type" = scalar ; then
		AC_DEFINE(TYPE_CHTYPE_IS_SCALAR,1,[Define to 1 if chtype is a scaler/integer])
	fi
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CURSES_CONFIG version: 2 updated: 2006/10/29 11:06:27
dnl ----------------
dnl Tie together the configure-script macros for curses.  It may be ncurses,
dnl but unless asked, we do not make a special search for ncurses.  However,
dnl still check for the ncurses version number, for use in other macros.
AC_DEFUN([CF_CURSES_CONFIG],
[
CF_CURSES_CPPFLAGS
CF_NCURSES_VERSION
CF_CURSES_LIBS
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CURSES_CPPFLAGS version: 14 updated: 2021/01/02 09:31:20
dnl ------------------
dnl Look for the curses headers.
AC_DEFUN([CF_CURSES_CPPFLAGS],[

AC_CACHE_CHECK(for extra include directories,cf_cv_curses_incdir,[
cf_cv_curses_incdir=no
case "$host_os" in
(hpux10.*)
	if test "x$cf_cv_screen" = "xcurses_colr"
	then
		test -d /usr/include/curses_colr && \
		cf_cv_curses_incdir="-I/usr/include/curses_colr"
	fi
	;;
(sunos3*|sunos4*)
	if test "x$cf_cv_screen" = "xcurses_5lib"
	then
		test -d /usr/5lib && \
		test -d /usr/5include && \
		cf_cv_curses_incdir="-I/usr/5include"
	fi
	;;
esac
])
if test "$cf_cv_curses_incdir" != no
then
	CF_APPEND_TEXT(CPPFLAGS,$cf_cv_curses_incdir)
fi

CF_CURSES_HEADER
CF_TERM_HEADER
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
dnl CF_CURSES_FUNCS version: 21 updated: 2024/06/11 20:24:35
dnl ---------------
dnl Curses-functions are a little complicated, since a lot of them are macros.
dnl
dnl $1 is a list of functions to test
AC_DEFUN([CF_CURSES_FUNCS],
[
AC_REQUIRE([CF_CURSES_CPPFLAGS])dnl
AC_REQUIRE([CF_XOPEN_CURSES])
AC_REQUIRE([CF_CURSES_TERM_H])
AC_REQUIRE([CF_CURSES_UNCTRL_H])

AC_FOREACH([AC_Func], [$1],
  [AH_TEMPLATE(AS_TR_CPP(HAVE_[]AC_Func),
               [Define if you have the `]AC_Func[' function.])])dnl

for cf_func in $1
do
	CF_UPPER(cf_tr_func,$cf_func)
	AC_MSG_CHECKING(for ${cf_func})
	CF_MSG_LOG(${cf_func})
	AC_CACHE_VAL(cf_cv_func_$cf_func,[
		eval cf_result='$ac_cv_func_'$cf_func
		if test ".$cf_result" != ".no"; then
			AC_TRY_LINK(CF__CURSES_HEAD,
			[
#ifndef ${cf_func}
long foo = (long)(&${cf_func});
fprintf(stderr, "testing linkage of $cf_func:%p\\n", (void *)foo);
if (foo + 1234L > 5678L)
	${cf_cv_main_return:-return}(foo != 0);
#endif
			],
			[cf_result=yes],
			[cf_result=no])
		fi
		eval 'cf_cv_func_'$cf_func'="$cf_result"'
	])
	# use the computed/retrieved cache-value:
	eval 'cf_result=$cf_cv_func_'$cf_func
	AC_MSG_RESULT($cf_result)
	if test "$cf_result" != no; then
		AC_DEFINE_UNQUOTED(HAVE_${cf_tr_func})
	fi
done
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CURSES_HEADER version: 6 updated: 2022/12/02 20:06:52
dnl ----------------
dnl Find a "curses" header file, e.g,. "curses.h", or one of the more common
dnl variations of ncurses' installs.
dnl
dnl $1 = ncurses when looking for ncurses, or is empty
AC_DEFUN([CF_CURSES_HEADER],[
AC_CACHE_CHECK(if we have identified curses headers,cf_cv_ncurses_header,[
cf_cv_ncurses_header=none
for cf_header in \
	ncurses.h ifelse($1,,,[$1/ncurses.h]) \
	curses.h ifelse($1,,,[$1/curses.h]) ifelse($1,,[ncurses/ncurses.h ncurses/curses.h])
do
AC_TRY_COMPILE([#include <${cf_header}>],
	[initscr(); endwin()],
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
dnl CF_CURSES_LIBS version: 45 updated: 2022/12/02 20:06:52
dnl --------------
dnl Look for the curses libraries.  Older curses implementations may require
dnl termcap/termlib to be linked as well.  Call CF_CURSES_CPPFLAGS first.
AC_DEFUN([CF_CURSES_LIBS],[

AC_REQUIRE([CF_CURSES_CPPFLAGS])dnl
AC_MSG_CHECKING(if we have identified curses libraries)
AC_TRY_LINK([#include <${cf_cv_ncurses_header:-curses.h}>],
	[initscr(); endwin()],
	cf_result=yes,
	cf_result=no)
AC_MSG_RESULT($cf_result)

if test "$cf_result" = no ; then
case "$host_os" in
(freebsd*)
	AC_CHECK_LIB(mytinfo,tgoto,[CF_ADD_LIBS(-lmytinfo)])
	;;
(hpux10.*)
	# Looking at HPUX 10.20, the Hcurses library is the oldest (1997), cur_colr
	# next (1998), and xcurses "newer" (2000).  There is no header file for
	# Hcurses; the subdirectory curses_colr has the headers (curses.h and
	# term.h) for cur_colr
	if test "x$cf_cv_screen" = "xcurses_colr"
	then
		AC_CHECK_LIB(cur_colr,initscr,[
			CF_ADD_LIBS(-lcur_colr)
			ac_cv_func_initscr=yes
			],[
		AC_CHECK_LIB(Hcurses,initscr,[
			# HP's header uses __HP_CURSES, but user claims _HP_CURSES.
			CF_ADD_LIBS(-lHcurses)
			CF_APPEND_TEXT(CPPFLAGS,-D__HP_CURSES -D_HP_CURSES)
			ac_cv_func_initscr=yes
			])])
	fi
	;;
(linux*)
	case `arch 2>/dev/null` in
	(x86_64)
		if test -d /lib64
		then
			CF_ADD_LIBDIR(/lib64)
		else
			CF_ADD_LIBDIR(/lib)
		fi
		;;
	(*)
		CF_ADD_LIBDIR(/lib)
		;;
	esac
	;;
(sunos3*|sunos4*)
	if test "x$cf_cv_screen" = "xcurses_5lib"
	then
		if test -d /usr/5lib ; then
			CF_ADD_LIBDIR(/usr/5lib)
			CF_ADD_LIBS(-lcurses -ltermcap)
		fi
	fi
	ac_cv_func_initscr=yes
	;;
esac

if test ".$ac_cv_func_initscr" != .yes ; then
	cf_save_LIBS="$LIBS"

	if test ".${cf_cv_ncurses_version:-no}" != .no
	then
		cf_check_list="ncurses curses cursesX"
	else
		cf_check_list="cursesX curses ncurses"
	fi

	# Check for library containing tgoto.  Do this before curses library
	# because it may be needed to link the test-case for initscr.
	if test "x$cf_term_lib" = x
	then
		AC_CHECK_FUNC(tgoto,[cf_term_lib=predefined],[
			for cf_term_lib in $cf_check_list otermcap termcap tinfo termlib unknown
			do
				AC_CHECK_LIB($cf_term_lib,tgoto,[
					: "${cf_nculib_root:=$cf_term_lib}"
					break
				])
			done
		])
	fi

	# Check for library containing initscr
	test "$cf_term_lib" != predefined && test "$cf_term_lib" != unknown && LIBS="-l$cf_term_lib $cf_save_LIBS"
	if test "x$cf_curs_lib" = x
	then
		for cf_curs_lib in $cf_check_list xcurses jcurses pdcurses unknown
		do
			LIBS="-l$cf_curs_lib $cf_save_LIBS"
			if test "$cf_term_lib" = unknown || test "$cf_term_lib" = "$cf_curs_lib" ; then
				AC_MSG_CHECKING(if we can link with $cf_curs_lib library)
				AC_TRY_LINK([#include <${cf_cv_ncurses_header:-curses.h}>],
					[initscr()],
					[cf_result=yes],
					[cf_result=no])
				AC_MSG_RESULT($cf_result)
				test "$cf_result" = yes && break
			elif test "$cf_curs_lib" = "$cf_term_lib" ; then
				cf_result=no
			elif test "$cf_term_lib" != predefined ; then
				AC_MSG_CHECKING(if we need both $cf_curs_lib and $cf_term_lib libraries)
				AC_TRY_LINK([#include <${cf_cv_ncurses_header:-curses.h}>],
					[initscr(); endwin();],
					[cf_result=no],
					[
					LIBS="-l$cf_curs_lib -l$cf_term_lib $cf_save_LIBS"
					AC_TRY_LINK([#include <${cf_cv_ncurses_header:-curses.h}>],
						[initscr()],
						[cf_result=yes],
						[cf_result=error])
					])
				AC_MSG_RESULT($cf_result)
				test "$cf_result" != error && break
			fi
		done
	fi
	test "$cf_curs_lib" = unknown && AC_MSG_ERROR(no curses library found)
fi
fi

])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CURSES_MOUSE version: 7 updated: 2021/01/10 18:23:00
dnl ---------------
dnl Test for the existence of SysVr4 mouse support in curses. If we've not got
dnl it, we'll simulate the interface (for xterm, at least).
AC_DEFUN([CF_CURSES_MOUSE],
[
AC_REQUIRE([CF_CURSES_CPPFLAGS])dnl
AC_MSG_CHECKING(for curses mouse-support)
AC_CACHE_VAL(cf_cv_curses_mouse,[
AC_TRY_LINK([#include <${cf_cv_ncurses_header:-curses.h}>],[
	int x, y;
#ifdef NCURSES_VERSION
	MEVENT mpos;
	getyx(stdscr, y, x);
	wmouse_trafo(stdscr, &y, &x, TRUE);
	if (wenclose(stdscr, y, x)) {
		getmouse(&mpos);
	}
#else
	getmouse();
	request_mouse_pos();
	wmouse_position(stdscr, &x, &y);
	mouse_on(TRUE);
	mouse_off(FALSE);
	mouse_set(0);
#endif
	],
	[cf_cv_curses_mouse=yes],
	[cf_cv_curses_mouse=no])
])
AC_MSG_RESULT($cf_cv_curses_mouse)
test "$cf_cv_curses_mouse" = yes && AC_DEFINE(CURSES_HAS_MOUSE,1,[Define to 1 if curses has mouse interface])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CURSES_STYLE version: 4 updated: 2009/01/06 19:42:07
dnl ---------------
dnl See which (if any!) of the styles of curses' line/row struct fit.
AC_DEFUN([CF_CURSES_STYLE],
[
AC_MSG_CHECKING(for curses struct-style)
AC_CACHE_VAL(cf_cv_curses_style,[
	cf_cv_curses_style=unknown
	save_CFLAGS="$CFLAGS"
	CFLAGS="$CFLAGS -I./include"
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
dnl CF_CURSES_TERMCAP version: 15 updated: 2023/12/09 10:53:57
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
	cf_tgoto_decl="
	extern char *tgoto(char*,int,int);"
	test -n "${cf_c_opts}${cf_t_opts}" && cf_tgoto_decl=

    AC_TRY_LINK([/* $cf_c_opts $cf_t_opts */
$CHECK_DECL_HDRS $cf_tgoto_decl],
	[static char fmt[] = ""; char *x = tgoto(fmt); (void)x],
	[test "$cf_cv_need_curses_h" = no && {
	     cf_cv_need_curses_h=maybe
	     cf_ok_c_opts=$cf_c_opts
	     cf_ok_t_opts=$cf_t_opts
	}],
	[echo "Recompiling with corrected call (C:$cf_c_opts, T:$cf_t_opts)" >&AC_FD_CC
	AC_TRY_LINK([
$CHECK_DECL_HDRS $cf_tgoto_decl],
	[static char fmt[] = ""; char *x = tgoto(fmt,0,0); (void)x],
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
(both)
	AC_DEFINE_UNQUOTED(NEED_CURSES_H,1,[Define to 1 if we must include curses.h])
	AC_DEFINE_UNQUOTED(NEED_TERMCAP_H,1,[Define to 1 if we must include termcap.h])
	;;
(curses.h)
	AC_DEFINE_UNQUOTED(NEED_CURSES_H,1,[Define to 1 if we must include curses.h])
	;;
(term.h)
	AC_DEFINE_UNQUOTED(NEED_TERM_H,1,[Define to 1 if we must include term.h])
	;;
(termcap.h)
	AC_DEFINE_UNQUOTED(NEED_TERMCAP_H,1,[Define to 1 if we must include termcap.h])
	;;
esac

])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CURSES_TERM_H version: 16 updated: 2024/01/07 06:34:16
dnl ----------------
dnl SVr4 curses should have term.h as well (where it puts the definitions of
dnl the low-level interface).  This may not be true in old/broken implementations,
dnl as well as in misconfigured systems (e.g., gcc configured for Solaris 2.4
dnl running with Solaris 2.5.1).
AC_DEFUN([CF_CURSES_TERM_H],
[
AC_REQUIRE([CF_CURSES_CPPFLAGS])dnl

AC_CACHE_CHECK(for term.h, cf_cv_term_header,[

# If we found <ncurses/curses.h>, look for <ncurses/term.h>, but always look
# for <term.h> if we do not find the variant.

cf_header_list="term.h ncurses/term.h ncursesw/term.h"

case "${cf_cv_ncurses_header:-curses.h}" in
(*/*)
	cf_header_item=`echo "${cf_cv_ncurses_header:-curses.h}" | sed -e 's%\..*%%' -e 's%/.*%/%'`term.h
	cf_header_list="$cf_header_item $cf_header_list"
	;;
esac

for cf_header in $cf_header_list
do
	AC_TRY_COMPILE([
#include <${cf_cv_ncurses_header:-curses.h}>
#include <${cf_header}>],
	[WINDOW *x; (void)x],
	[cf_cv_term_header=$cf_header
	 break],
	[cf_cv_term_header=no])
done

case "$cf_cv_term_header" in
(no)
	# If curses is ncurses, some packagers still mess it up by trying to make
	# us use GNU termcap.  This handles the most common case.
	for cf_header in ncurses/term.h ncursesw/term.h
	do
		AC_TRY_COMPILE([
#include <${cf_cv_ncurses_header:-curses.h}>
#ifdef NCURSES_VERSION
#include <${cf_header}>
#else
#error expected NCURSES_VERSION to be defined
#endif],
			[WINDOW *x; (void)x],
			[cf_cv_term_header=$cf_header
			 break],
			[cf_cv_term_header=no])
	done
	;;
esac
])

case "$cf_cv_term_header" in
(term.h)
	AC_DEFINE(HAVE_TERM_H,1,[Define to 1 if we have term.h])
	;;
(ncurses/term.h)
	AC_DEFINE(HAVE_NCURSES_TERM_H,1,[Define to 1 if we have ncurses/term.h])
	;;
(ncursesw/term.h)
	AC_DEFINE(HAVE_NCURSESW_TERM_H,1,[Define to 1 if we have ncursesw/term.h])
	;;
esac
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CURSES_UNCTRL_H version: 8 updated: 2021/01/02 09:31:20
dnl ------------------
dnl Any X/Open curses implementation must have unctrl.h, but ncurses packages
dnl may put it in a subdirectory (along with ncurses' other headers, of
dnl course).  Packages which put the headers in inconsistent locations are
dnl broken).
AC_DEFUN([CF_CURSES_UNCTRL_H],
[
AC_REQUIRE([CF_CURSES_CPPFLAGS])dnl

AC_CACHE_CHECK(for unctrl.h, cf_cv_unctrl_header,[

# If we found <ncurses/curses.h>, look for <ncurses/unctrl.h>, but always look
# for <unctrl.h> if we do not find the variant.

cf_header_list="unctrl.h ncurses/unctrl.h ncursesw/unctrl.h"

case "${cf_cv_ncurses_header:-curses.h}" in
(*/*)
	cf_header_item=`echo "${cf_cv_ncurses_header:-curses.h}" | sed -e 's%\..*%%' -e 's%/.*%/%'`unctrl.h
	cf_header_list="$cf_header_item $cf_header_list"
	;;
esac

for cf_header in $cf_header_list
do
	AC_TRY_COMPILE([
#include <${cf_cv_ncurses_header:-curses.h}>
#include <${cf_header}>],
	[WINDOW *x; (void)x],
	[cf_cv_unctrl_header=$cf_header
	 break],
	[cf_cv_unctrl_header=no])
done
])

case "$cf_cv_unctrl_header" in
(no)
	AC_MSG_WARN(unctrl.h header not found)
	;;
esac

case "$cf_cv_unctrl_header" in
(unctrl.h)
	AC_DEFINE(HAVE_UNCTRL_H,1,[Define to 1 if we have unctrl.h])
	;;
(ncurses/unctrl.h)
	AC_DEFINE(HAVE_NCURSES_UNCTRL_H,1,[Define to 1 if we have ncurses/unctrl.h])
	;;
(ncursesw/unctrl.h)
	AC_DEFINE(HAVE_NCURSESW_UNCTRL_H,1,[Define to 1 if we have ncursesw/unctrl.h])
	;;
esac
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_DIRNAME version: 5 updated: 2020/12/31 20:19:42
dnl ----------
dnl "dirname" is not portable, so we fake it with a shell script.
AC_DEFUN([CF_DIRNAME],[$1=`echo "$2" | sed -e 's%/[[^/]]*$%%'`])dnl
dnl ---------------------------------------------------------------------------
dnl CF_DISABLE_ECHO version: 14 updated: 2021/09/04 06:35:04
dnl ---------------
dnl You can always use "make -n" to see the actual options, but it is hard to
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
	[  --disable-echo          do not display "compiling" commands],
	[
	ECHO_LT='--silent'
	ECHO_LD='@echo linking [$]@;'
	RULE_CC='@echo compiling [$]<'
	SHOW_CC='@echo compiling [$]@'
	ECHO_CC='@'
],[
	ECHO_LT=''
	ECHO_LD=''
	RULE_CC=''
	SHOW_CC=''
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
dnl CF_DISABLE_LEAKS version: 9 updated: 2021/04/03 16:41:50
dnl ----------------
dnl Combine no-leak checks with the libraries or tools that are used for the
dnl checks.
AC_DEFUN([CF_DISABLE_LEAKS],[

AC_REQUIRE([CF_WITH_DMALLOC])
AC_REQUIRE([CF_WITH_DBMALLOC])
AC_REQUIRE([CF_WITH_VALGRIND])

AC_MSG_CHECKING(if you want to perform memory-leak testing)
AC_ARG_ENABLE(leaks,
	[  --disable-leaks         test: free permanent memory, analyze leaks],
	[enable_leaks=$enableval],
	[enable_leaks=yes])
dnl with_no_leaks is more readable...
if test "x$enable_leaks" = xno; then with_no_leaks=yes; else with_no_leaks=no; fi
AC_MSG_RESULT($with_no_leaks)

if test "$enable_leaks" = no ; then
	AC_DEFINE(NO_LEAKS,1,[Define to 1 if you want to perform memory-leak testing.])
	AC_DEFINE(YY_NO_LEAKS,1,[Define to 1 if you want to perform memory-leak testing.])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ENABLE_PC_FILES version: 16 updated: 2021/11/20 12:48:37
dnl ------------------
dnl This is the "--enable-pc-files" option, which is available if there is a
dnl pkg-config configuration on the local machine.
AC_DEFUN([CF_ENABLE_PC_FILES],[
AC_REQUIRE([CF_PKG_CONFIG])
AC_REQUIRE([CF_WITH_PKG_CONFIG_LIBDIR])

if test "x$PKG_CONFIG" != xnone
then
	AC_MSG_CHECKING(if we should install .pc files for $PKG_CONFIG)
else
	AC_MSG_CHECKING(if we should install .pc files)
fi

AC_ARG_ENABLE(pc-files,
	[  --enable-pc-files       generate and install .pc files for pkg-config],
	[enable_pc_files=$enableval],
	[enable_pc_files=no])
AC_MSG_RESULT($enable_pc_files)

if test "x$enable_pc_files" != xno
then
	MAKE_PC_FILES=
	case "x$PKG_CONFIG_LIBDIR" in
	(xno|xnone|xyes|x)
		AC_MSG_WARN(no PKG_CONFIG_LIBDIR was found)
		;;
	(*)
		cf_pkg_config_libdir="$PKG_CONFIG_LIBDIR"
		CF_PATH_SYNTAX(cf_pkg_config_libdir)
		;;
	esac
else
	MAKE_PC_FILES="#"
fi
AC_SUBST(MAKE_PC_FILES)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ENABLE_WARNINGS version: 9 updated: 2021/01/05 19:40:50
dnl ------------------
dnl Configure-option to enable gcc warnings
dnl
dnl $1 = extra options to add, if supported
dnl $2 = option for checking attributes.  By default, this is done when
dnl      warnings are enabled.  For other values:
dnl      yes: always do this, e.g., to use in generated library-headers
dnl      no: never do this
AC_DEFUN([CF_ENABLE_WARNINGS],[
if test "$GCC" = yes || test "$GXX" = yes
then
CF_FIX_WARNINGS(CFLAGS)
CF_FIX_WARNINGS(CPPFLAGS)
CF_FIX_WARNINGS(LDFLAGS)
AC_MSG_CHECKING(if you want to turn on gcc warnings)
CF_ARG_ENABLE(warnings,
	[  --enable-warnings       test: turn on gcc compiler warnings],
	[enable_warnings=yes],
	[enable_warnings=no])
AC_MSG_RESULT($enable_warnings)
if test "$enable_warnings" = "yes"
then
	ifelse($2,,[CF_GCC_ATTRIBUTES])
	CF_GCC_WARNINGS($1)
fi
ifelse($2,yes,[CF_GCC_ATTRIBUTES])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_FIND_LIBRARY version: 11 updated: 2021/01/02 09:31:20
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
	eval 'cf_cv_have_lib_'"$1"'=no'
	cf_libdir=""
	AC_CHECK_FUNC($5,
		eval 'cf_cv_have_lib_'"$1"'=yes',[
		cf_save_LIBS="$LIBS"
		AC_MSG_CHECKING(for $5 in -l$1)
		LIBS="-l$1 $LIBS"
		AC_TRY_LINK([$3],[$4],
			[AC_MSG_RESULT(yes)
			 eval 'cf_cv_have_lib_'"$1"'=yes'
			],
			[AC_MSG_RESULT(no)
			CF_LIBRARY_PATH(cf_search,$2)
			for cf_libdir in $cf_search
			do
				AC_MSG_CHECKING(for -l$1 in $cf_libdir)
				LIBS="-L$cf_libdir -l$1 $cf_save_LIBS"
				AC_TRY_LINK([$3],[$4],
					[AC_MSG_RESULT(yes)
			 		 eval 'cf_cv_have_lib_'"$1"'=yes'
					 break],
					[AC_MSG_RESULT(no)
					 LIBS="$cf_save_LIBS"])
			done
			])
		])
eval 'cf_found_library="[$]cf_cv_have_lib_'"$1"\"
ifelse($6,,[
if test "$cf_found_library" = no ; then
	AC_MSG_ERROR(Cannot link $1 library)
fi
])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_FIND_LINKAGE version: 22 updated: 2020/12/31 20:19:42
dnl ---------------
dnl Find a library (specifically the linkage used in the code fragment),
dnl searching for it if it is not already in the library path.
dnl See also CF_ADD_SEARCHPATH.
dnl
dnl Parameters (4-on are optional):
dnl     $1 = headers for library entrypoint
dnl     $2 = code fragment for library entrypoint
dnl     $3 = the library name without the "-l" option or ".so" suffix.
dnl     $4 = action to perform if successful (default: update CPPFLAGS, etc)
dnl     $5 = action to perform if not successful
dnl     $6 = module name, if not the same as the library name
dnl     $7 = extra libraries
dnl
dnl Sets these variables:
dnl     $cf_cv_find_linkage_$3 - yes/no according to whether linkage is found
dnl     $cf_cv_header_path_$3 - include-directory if needed
dnl     $cf_cv_library_path_$3 - library-directory if needed
dnl     $cf_cv_library_file_$3 - library-file if needed, e.g., -l$3
AC_DEFUN([CF_FIND_LINKAGE],[

# If the linkage is not already in the $CPPFLAGS/$LDFLAGS configuration, these
# will be set on completion of the AC_TRY_LINK below.
cf_cv_header_path_$3=
cf_cv_library_path_$3=

CF_MSG_LOG([Starting [FIND_LINKAGE]($3,$6)])

cf_save_LIBS="$LIBS"

AC_TRY_LINK([$1],[$2],[
	cf_cv_find_linkage_$3=yes
	cf_cv_header_path_$3=/usr/include
	cf_cv_library_path_$3=/usr/lib
],[

LIBS="-l$3 $7 $cf_save_LIBS"

AC_TRY_LINK([$1],[$2],[
	cf_cv_find_linkage_$3=yes
	cf_cv_header_path_$3=/usr/include
	cf_cv_library_path_$3=/usr/lib
	cf_cv_library_file_$3="-l$3"
],[
	cf_cv_find_linkage_$3=no
	LIBS="$cf_save_LIBS"

	CF_VERBOSE(find linkage for $3 library)
	CF_MSG_LOG([Searching for headers in [FIND_LINKAGE]($3,$6)])

	cf_save_CPPFLAGS="$CPPFLAGS"
	cf_test_CPPFLAGS="$CPPFLAGS"

	CF_HEADER_PATH(cf_search,ifelse([$6],,[$3],[$6]))
	for cf_cv_header_path_$3 in $cf_search
	do
		if test -d "$cf_cv_header_path_$3" ; then
			CF_VERBOSE(... testing $cf_cv_header_path_$3)
			CPPFLAGS="$cf_save_CPPFLAGS"
			CF_APPEND_TEXT(CPPFLAGS,-I$cf_cv_header_path_$3)
			AC_TRY_COMPILE([$1],[$2],[
				CF_VERBOSE(... found $3 headers in $cf_cv_header_path_$3)
				cf_cv_find_linkage_$3=maybe
				cf_test_CPPFLAGS="$CPPFLAGS"
				break],[
				CPPFLAGS="$cf_save_CPPFLAGS"
				])
		fi
	done

	if test "$cf_cv_find_linkage_$3" = maybe ; then

		CF_MSG_LOG([Searching for $3 library in [FIND_LINKAGE]($3,$6)])

		cf_save_LIBS="$LIBS"
		cf_save_LDFLAGS="$LDFLAGS"

		ifelse([$6],,,[
		CPPFLAGS="$cf_test_CPPFLAGS"
		LIBS="-l$3 $7 $cf_save_LIBS"
		AC_TRY_LINK([$1],[$2],[
			CF_VERBOSE(... found $3 library in system)
			cf_cv_find_linkage_$3=yes])
			CPPFLAGS="$cf_save_CPPFLAGS"
			LIBS="$cf_save_LIBS"
			])

		if test "$cf_cv_find_linkage_$3" != yes ; then
			CF_LIBRARY_PATH(cf_search,$3)
			for cf_cv_library_path_$3 in $cf_search
			do
				if test -d "$cf_cv_library_path_$3" ; then
					CF_VERBOSE(... testing $cf_cv_library_path_$3)
					CPPFLAGS="$cf_test_CPPFLAGS"
					LIBS="-l$3 $7 $cf_save_LIBS"
					LDFLAGS="$cf_save_LDFLAGS -L$cf_cv_library_path_$3"
					AC_TRY_LINK([$1],[$2],[
					CF_VERBOSE(... found $3 library in $cf_cv_library_path_$3)
					cf_cv_find_linkage_$3=yes
					cf_cv_library_file_$3="-l$3"
					break],[
					CPPFLAGS="$cf_save_CPPFLAGS"
					LIBS="$cf_save_LIBS"
					LDFLAGS="$cf_save_LDFLAGS"
					])
				fi
			done
			CPPFLAGS="$cf_save_CPPFLAGS"
			LDFLAGS="$cf_save_LDFLAGS"
		fi

	else
		cf_cv_find_linkage_$3=no
	fi
	],$7)
])

LIBS="$cf_save_LIBS"

if test "$cf_cv_find_linkage_$3" = yes ; then
ifelse([$4],,[
	CF_ADD_INCDIR($cf_cv_header_path_$3)
	CF_ADD_LIBDIR($cf_cv_library_path_$3)
	CF_ADD_LIB($3)
],[$4])
else
ifelse([$5],,AC_MSG_WARN(Cannot find $3 library),[$5])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_FIX_WARNINGS version: 4 updated: 2021/12/16 18:22:31
dnl ---------------
dnl Warning flags do not belong in CFLAGS, CPPFLAGS, etc.  Any of gcc's
dnl "-Werror" flags can interfere with configure-checks.  Those go into
dnl EXTRA_CFLAGS.
dnl
dnl $1 = variable name to repair
define([CF_FIX_WARNINGS],[
if test "$GCC" = yes || test "$GXX" = yes
then
	case [$]$1 in
	(*-Werror=*)
		cf_temp_flags=
		for cf_temp_scan in [$]$1
		do
			case "x$cf_temp_scan" in
			(x-Werror=format*)
				CF_APPEND_TEXT(cf_temp_flags,$cf_temp_scan)
				;;
			(x-Werror=*)
				CF_APPEND_TEXT(EXTRA_CFLAGS,$cf_temp_scan)
				;;
			(*)
				CF_APPEND_TEXT(cf_temp_flags,$cf_temp_scan)
				;;
			esac
		done
		if test "x[$]$1" != "x$cf_temp_flags"
		then
			CF_VERBOSE(repairing $1: [$]$1)
			$1="$cf_temp_flags"
			CF_VERBOSE(... fixed [$]$1)
			CF_VERBOSE(... extra $EXTRA_CFLAGS)
		fi
		;;
	esac
fi
AC_SUBST(EXTRA_CFLAGS)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_FUNC_LSTAT version: 6 updated: 2023/01/11 04:05:23
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
AC_TRY_LINK([$ac_includes_default],
	[struct stat sb; lstat(".", &sb); (void) sb],
	[ac_cv_func_lstat=yes],
	[ac_cv_func_lstat=no])
	])
AC_MSG_RESULT($ac_cv_func_lstat )
if test "$ac_cv_func_lstat" = yes; then
	AC_DEFINE(HAVE_LSTAT,1,[Define to 1 if we have lstat])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_GCC_ATTRIBUTES version: 24 updated: 2021/03/20 12:00:25
dnl -----------------
dnl Test for availability of useful gcc __attribute__ directives to quiet
dnl compiler warnings.  Though useful, not all are supported -- and contrary
dnl to documentation, unrecognized directives cause older compilers to barf.
AC_DEFUN([CF_GCC_ATTRIBUTES],
[AC_REQUIRE([AC_PROG_FGREP])dnl
AC_REQUIRE([CF_C11_NORETURN])dnl

if test "$GCC" = yes || test "$GXX" = yes
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
cat > "conftest.$ac_ext" <<EOF
#line __oline__ "${as_me:-configure}"
#include <stdio.h>
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
extern GCC_NORETURN void oops(char *,...) GCC_PRINTFLIKE(1,2);
extern GCC_NORETURN void foo(void);
int main(int argc GCC_UNUSED, char *argv[[]] GCC_UNUSED) { (void)argc; (void)argv; return 0; }
EOF
	cf_printf_attribute=no
	cf_scanf_attribute=no
	for cf_attribute in scanf printf unused noreturn
	do
		CF_UPPER(cf_ATTRIBUTE,$cf_attribute)
		cf_directive="__attribute__(($cf_attribute))"
		echo "checking for $CC $cf_directive" 1>&AC_FD_CC

		case "$cf_attribute" in
		(printf)
			cf_printf_attribute=yes
			cat >conftest.h <<EOF
#define GCC_$cf_ATTRIBUTE 1
EOF
			;;
		(scanf)
			cf_scanf_attribute=yes
			cat >conftest.h <<EOF
#define GCC_$cf_ATTRIBUTE 1
EOF
			;;
		(*)
			cat >conftest.h <<EOF
#define GCC_$cf_ATTRIBUTE $cf_directive
EOF
			;;
		esac

		if AC_TRY_EVAL(ac_compile); then
			test -n "$verbose" && AC_MSG_RESULT(... $cf_attribute)
			cat conftest.h >>confdefs.h
			case "$cf_attribute" in
			(noreturn)
				AC_DEFINE_UNQUOTED(GCC_NORETURN,$cf_directive,[Define to noreturn-attribute for gcc])
				;;
			(printf)
				cf_value='/* nothing */'
				if test "$cf_printf_attribute" != no ; then
					cf_value='__attribute__((format(printf,fmt,var)))'
					AC_DEFINE(GCC_PRINTF,1,[Define to 1 if the compiler supports gcc-like printf attribute.])
				fi
				AC_DEFINE_UNQUOTED(GCC_PRINTFLIKE(fmt,var),$cf_value,[Define to printf-attribute for gcc])
				;;
			(scanf)
				cf_value='/* nothing */'
				if test "$cf_scanf_attribute" != no ; then
					cf_value='__attribute__((format(scanf,fmt,var)))'
					AC_DEFINE(GCC_SCANF,1,[Define to 1 if the compiler supports gcc-like scanf attribute.])
				fi
				AC_DEFINE_UNQUOTED(GCC_SCANFLIKE(fmt,var),$cf_value,[Define to sscanf-attribute for gcc])
				;;
			(unused)
				AC_DEFINE_UNQUOTED(GCC_UNUSED,$cf_directive,[Define to unused-attribute for gcc])
				;;
			esac
		fi
	done
else
	${FGREP-fgrep} define conftest.i >>confdefs.h
fi
rm -rf ./conftest*
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_GCC_VERSION version: 9 updated: 2023/03/05 14:30:13
dnl --------------
dnl Find version of gcc, and (because icc/clang pretend to be gcc without being
dnl compatible), attempt to determine if icc/clang is actually used.
AC_DEFUN([CF_GCC_VERSION],[
AC_REQUIRE([AC_PROG_CC])
GCC_VERSION=none
if test "$GCC" = yes ; then
	AC_MSG_CHECKING(version of $CC)
	GCC_VERSION="`${CC} --version 2>/dev/null | sed -e '2,$d' -e 's/^[[^(]]*([[^)]][[^)]]*) //' -e 's/^[[^0-9.]]*//' -e 's/[[^0-9.]].*//'`"
	test -z "$GCC_VERSION" && GCC_VERSION=unknown
	AC_MSG_RESULT($GCC_VERSION)
fi
CF_INTEL_COMPILER(GCC,INTEL_COMPILER,CFLAGS)
CF_CLANG_COMPILER(GCC,CLANG_COMPILER,CFLAGS)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_GCC_WARNINGS version: 43 updated: 2024/12/21 08:44:12
dnl ---------------
dnl Check if the compiler supports useful warning options.  There's a few that
dnl we don't use, simply because they're too noisy:
dnl
dnl	-Wconversion (useful in older versions of gcc, but not in gcc 2.7.x)
dnl	-Winline (usually not worthwhile)
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
AC_REQUIRE([CF_GCC_VERSION])
if test "x$have_x" = xyes; then CF_CONST_X_STRING fi
cat > "conftest.$ac_ext" <<EOF
#line __oline__ "${as_me:-configure}"
int main(int argc, char *argv[[]]) { return (argv[[argc-1]] == 0) ; }
EOF
if test "$INTEL_COMPILER" = yes
then
# The "-wdXXX" options suppress warnings:
# remark #1419: external declaration in primary source file
# remark #1683: explicit conversion of a 64-bit integral type to a smaller integral type (potential portability problem)
# remark #1684: conversion from pointer to same-sized integral type (potential portability problem)
# remark #193: zero used for undefined preprocessing identifier
# remark #593: variable "curs_sb_left_arrow" was set but never used
# remark #810: conversion from "int" to "Dimension={unsigned short}" may lose significant bits
# remark #869: parameter "tw" was never referenced
# remark #981: operands are evaluated in unspecified order
# warning #279: controlling expression is constant

	AC_CHECKING([for $CC warning options])
	cf_save_CFLAGS="$CFLAGS"
	EXTRA_CFLAGS="$EXTRA_CFLAGS -Wall"
	for cf_opt in \
		wd1419 \
		wd1683 \
		wd1684 \
		wd193 \
		wd593 \
		wd279 \
		wd810 \
		wd869 \
		wd981
	do
		CFLAGS="$cf_save_CFLAGS $EXTRA_CFLAGS -$cf_opt"
		if AC_TRY_EVAL(ac_compile); then
			test -n "$verbose" && AC_MSG_RESULT(... -$cf_opt)
			EXTRA_CFLAGS="$EXTRA_CFLAGS -$cf_opt"
		fi
	done
	CFLAGS="$cf_save_CFLAGS"
elif test "$GCC" = yes && test "$GCC_VERSION" != "unknown"
then
	AC_CHECKING([for $CC warning options])
	cf_save_CFLAGS="$CFLAGS"
	cf_warn_CONST=""
	test "$with_ext_const" = yes && cf_warn_CONST="Wwrite-strings"
	cf_gcc_warnings="Wignored-qualifiers Wlogical-op Wvarargs"
	test "x$CLANG_COMPILER" = xyes && cf_gcc_warnings=
	for cf_opt in W Wall \
		Wbad-function-cast \
		Wcast-align \
		Wcast-qual \
		Wdeclaration-after-statement \
		Wextra \
		Winline \
		Wmissing-declarations \
		Wmissing-prototypes \
		Wnested-externs \
		Wpointer-arith \
		Wshadow \
		Wstrict-prototypes \
		Wundef Wno-inline $cf_gcc_warnings $cf_warn_CONST $1
	do
		CFLAGS="$cf_save_CFLAGS $EXTRA_CFLAGS -$cf_opt"
		if AC_TRY_EVAL(ac_compile); then
			test -n "$verbose" && AC_MSG_RESULT(... -$cf_opt)
			case "$cf_opt" in
			(Winline)
				case "$GCC_VERSION" in
				([[34]].*)
					CF_VERBOSE(feature is broken in gcc $GCC_VERSION)
					continue;;
				esac
				;;
			(Wpointer-arith)
				case "$GCC_VERSION" in
				([[12]].*)
					CF_VERBOSE(feature is broken in gcc $GCC_VERSION)
					continue;;
				esac
				;;
			esac
			EXTRA_CFLAGS="$EXTRA_CFLAGS -$cf_opt"
		fi
	done
	CFLAGS="$cf_save_CFLAGS"
fi
rm -rf ./conftest*

AC_SUBST(EXTRA_CFLAGS)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_GETOPT_HEADER version: 9 updated: 2024/08/10 10:30:39
dnl ----------------
dnl Check for getopt's variables which are commonly defined in stdlib.h,
dnl unistd.h or (nonstandard) in getopt.h
AC_DEFUN([CF_GETOPT_HEADER],
[AC_REQUIRE([AC_HEADER_STDC])

AC_CHECK_HEADERS(getopt.h)
AC_CACHE_CHECK(for header declaring getopt variables,cf_cv_getopt_header,[
cf_cv_getopt_header=none
for cf_header in stdio.h stdlib.h unistd.h getopt.h
do
AC_TRY_COMPILE([
#include <$cf_header>],
[int x = optind; char *y = optarg; (void)x; (void)y],
[cf_cv_getopt_header=$cf_header
 break])
done
])
if test "$cf_cv_getopt_header" != none ; then
	AC_DEFINE(HAVE_GETOPT_HEADER,1,[Define to 1 if getopt variables are declared in header])
fi
if test "$cf_cv_getopt_header" = getopt.h ; then
	AC_DEFINE(NEED_GETOPT_H,1,[Define to 1 if we must include getopt.h])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_GMTOFF version: 6 updated: 2021/01/10 18:23:00
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
test "$cf_cv_decl_localzone" = yes && AC_DEFINE(LOCALZONE_DECLARED,1,[Define to 1 if localzone is declared])

AC_MSG_CHECKING(for timezone declared)
AC_CACHE_VAL(cf_cv_decl_timezone,[
	AC_TRY_COMPILE($cf_decl,
		[long x = timezone;
		 timezone = x],
		[cf_cv_decl_timezone=yes],
		[cf_cv_decl_timezone=no])])
AC_MSG_RESULT($cf_cv_decl_timezone)
if test "$cf_cv_decl_timezone" = yes; then
	AC_DEFINE(TIMEZONE_DECLARED,1,[Define to 1 if timezone is declared])

	AC_MSG_CHECKING(for daylight declared)
	AC_CACHE_VAL(cf_cv_decl_daylight,[
		AC_TRY_COMPILE($cf_decl,
			[long x = daylight],
			[cf_cv_decl_daylight=yes],
			[cf_cv_decl_daylight=no])])
	AC_MSG_RESULT($cf_cv_decl_daylight)
	test "$cf_cv_decl_daylight" = yes && AC_DEFINE(DAYLIGHT_DECLARED,1,[Define to 1 if daylight is declared])
fi

AC_MSG_CHECKING(for .tm_gmtoff in struct tm)
AC_CACHE_VAL(cf_cv_tm_gmtoff_decl,[
	AC_TRY_COMPILE([$cf_decl],
		[struct tm tm; tm.tm_gmtoff],
		[cf_cv_tm_gmtoff_decl=yes],
		[cf_cv_tm_gmtoff_decl=no])])
AC_MSG_RESULT($cf_cv_tm_gmtoff_decl)
test "$cf_cv_tm_gmtoff_decl" = yes && AC_DEFINE(HAVE_TM_GMTOFF,1,[Define to 1 if tm.tm_gmtoff is declared])

AC_MSG_CHECKING(for .tm_isdst in struct tm)
AC_CACHE_VAL(cf_cv_tm_isdst_decl,[
	AC_TRY_COMPILE([$cf_decl],
		[struct tm tm; tm.tm_isdst],
		[cf_cv_tm_isdst_decl=yes],
		[cf_cv_tm_isdst_decl=no])])
AC_MSG_RESULT($cf_cv_tm_isdst_decl)
test "$cf_cv_tm_isdst_decl" = yes && AC_DEFINE(HAVE_TM_ISDST,1,[Define to 1 if tm.tm_isdst is declared])

AC_MSG_CHECKING(for .tm_zone in struct tm)
AC_CACHE_VAL(cf_cv_tm_zone_decl,[
	AC_TRY_COMPILE([$cf_decl],
		[struct tm tm; tm.tm_zone],
		[cf_cv_tm_zone_decl=yes],
		[cf_cv_tm_zone_decl=no])])
AC_MSG_RESULT($cf_cv_tm_zone_decl)
test "$cf_cv_tm_zone_decl" = yes && AC_DEFINE(HAVE_TM_ZONE,1,[Define to 1 if tm.tm_zone is declared])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_GLOB_FULLPATH version: 2 updated: 2024/08/03 12:34:02
dnl ----------------
dnl Use this in case-statements to check for pathname syntax, i.e., absolute
dnl pathnames.  The "x" is assumed since we provide an alternate form for DOS.
AC_DEFUN([CF_GLOB_FULLPATH],[
AC_REQUIRE([CF_WITH_SYSTYPE])dnl
case "$cf_cv_system_name" in
(cygwin*|msys*|mingw32*|mingw64|os2*)
	GLOB_FULLPATH_POSIX='/*'
	GLOB_FULLPATH_OTHER='[[a-zA-Z]]:[[\\/]]*'
	;;
(*)
	GLOB_FULLPATH_POSIX='/*'
	GLOB_FULLPATH_OTHER=$GLOB_FULLPATH_POSIX
	;;
esac
AC_SUBST(GLOB_FULLPATH_POSIX)
AC_SUBST(GLOB_FULLPATH_OTHER)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_GNU_SOURCE version: 10 updated: 2018/12/10 20:09:41
dnl -------------
dnl Check if we must define _GNU_SOURCE to get a reasonable value for
dnl _XOPEN_SOURCE, upon which many POSIX definitions depend.  This is a defect
dnl (or misfeature) of glibc2, which breaks portability of many applications,
dnl since it is interwoven with GNU extensions.
dnl
dnl Well, yes we could work around it...
dnl
dnl Parameters:
dnl	$1 is the nominal value for _XOPEN_SOURCE
AC_DEFUN([CF_GNU_SOURCE],
[
cf_gnu_xopen_source=ifelse($1,,500,$1)

AC_CACHE_CHECK(if this is the GNU C library,cf_cv_gnu_library,[
AC_TRY_COMPILE([#include <sys/types.h>],[
	#if __GLIBC__ > 0 && __GLIBC_MINOR__ >= 0
		return 0;
	#elif __NEWLIB__ > 0 && __NEWLIB_MINOR__ >= 0
		return 0;
	#else
	#	error not GNU C library
	#endif],
	[cf_cv_gnu_library=yes],
	[cf_cv_gnu_library=no])
])

if test x$cf_cv_gnu_library = xyes; then

	# With glibc 2.19 (13 years after this check was begun), _DEFAULT_SOURCE
	# was changed to help a little.  newlib incorporated the change about 4
	# years later.
	AC_CACHE_CHECK(if _DEFAULT_SOURCE can be used as a basis,cf_cv_gnu_library_219,[
		cf_save="$CPPFLAGS"
		CF_APPEND_TEXT(CPPFLAGS,-D_DEFAULT_SOURCE)
		AC_TRY_COMPILE([#include <sys/types.h>],[
			#if (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 19) || (__GLIBC__ > 2)
				return 0;
			#elif (__NEWLIB__ == 2 && __NEWLIB_MINOR__ >= 4) || (__GLIBC__ > 3)
				return 0;
			#else
			#	error GNU C library __GLIBC__.__GLIBC_MINOR__ is too old
			#endif],
			[cf_cv_gnu_library_219=yes],
			[cf_cv_gnu_library_219=no])
		CPPFLAGS="$cf_save"
	])

	if test "x$cf_cv_gnu_library_219" = xyes; then
		cf_save="$CPPFLAGS"
		AC_CACHE_CHECK(if _XOPEN_SOURCE=$cf_gnu_xopen_source works with _DEFAULT_SOURCE,cf_cv_gnu_dftsrc_219,[
			CF_ADD_CFLAGS(-D_DEFAULT_SOURCE -D_XOPEN_SOURCE=$cf_gnu_xopen_source)
			AC_TRY_COMPILE([
				#include <limits.h>
				#include <sys/types.h>
				],[
				#if (_XOPEN_SOURCE >= $cf_gnu_xopen_source) && (MB_LEN_MAX > 1)
					return 0;
				#else
				#	error GNU C library is too old
				#endif],
				[cf_cv_gnu_dftsrc_219=yes],
				[cf_cv_gnu_dftsrc_219=no])
			])
		test "x$cf_cv_gnu_dftsrc_219" = "xyes" || CPPFLAGS="$cf_save"
	else
		cf_cv_gnu_dftsrc_219=maybe
	fi

	if test "x$cf_cv_gnu_dftsrc_219" != xyes; then

		AC_CACHE_CHECK(if we must define _GNU_SOURCE,cf_cv_gnu_source,[
		AC_TRY_COMPILE([#include <sys/types.h>],[
			#ifndef _XOPEN_SOURCE
			#error	expected _XOPEN_SOURCE to be defined
			#endif],
			[cf_cv_gnu_source=no],
			[cf_save="$CPPFLAGS"
			 CF_ADD_CFLAGS(-D_GNU_SOURCE)
			 AC_TRY_COMPILE([#include <sys/types.h>],[
				#ifdef _XOPEN_SOURCE
				#error	expected _XOPEN_SOURCE to be undefined
				#endif],
				[cf_cv_gnu_source=no],
				[cf_cv_gnu_source=yes])
			CPPFLAGS="$cf_save"
			])
		])

		if test "$cf_cv_gnu_source" = yes
		then
		AC_CACHE_CHECK(if we should also define _DEFAULT_SOURCE,cf_cv_default_source,[
			CF_APPEND_TEXT(CPPFLAGS,-D_GNU_SOURCE)
			AC_TRY_COMPILE([#include <sys/types.h>],[
				#ifdef _DEFAULT_SOURCE
				#error	expected _DEFAULT_SOURCE to be undefined
				#endif],
				[cf_cv_default_source=no],
				[cf_cv_default_source=yes])
			])
			if test "$cf_cv_default_source" = yes
			then
				CF_APPEND_TEXT(CPPFLAGS,-D_DEFAULT_SOURCE)
			fi
		fi
	fi

fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_HAVE_FUNCS version: 7 updated: 2023/01/16 16:45:15
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
AC_TRY_LINK([#undef ${cf_func}
extern void ${cf_func}(void);],[${cf_func}();],[
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
dnl CF_HEADER_PATH version: 15 updated: 2021/01/01 13:31:04
dnl --------------
dnl Construct a search-list of directories for a nonstandard header-file
dnl
dnl Parameters
dnl	$1 = the variable to return as result
dnl	$2 = the package name
AC_DEFUN([CF_HEADER_PATH],
[
$1=

# collect the current set of include-directories from compiler flags
cf_header_path_list=""
if test -n "${CFLAGS}${CPPFLAGS}" ; then
	for cf_header_path in $CPPFLAGS $CFLAGS
	do
		case "$cf_header_path" in
		(-I*)
			cf_header_path=`echo ".$cf_header_path" |sed -e 's/^...//' -e 's,/include$,,'`
			CF_ADD_SUBDIR_PATH($1,$2,include,$cf_header_path,NONE)
			cf_header_path_list="$cf_header_path_list [$]$1"
			;;
		esac
	done
fi

# add the variations for the package we are looking for
CF_SUBDIR_PATH($1,$2,include)

test "$includedir" != NONE && \
test "$includedir" != "/usr/include" && \
test -d "$includedir" && {
	test -d "$includedir" &&    $1="[$]$1 $includedir"
	test -d "$includedir/$2" && $1="[$]$1 $includedir/$2"
}

test "$oldincludedir" != NONE && \
test "$oldincludedir" != "/usr/include" && \
test -d "$oldincludedir" && {
	test -d "$oldincludedir"    && $1="[$]$1 $oldincludedir"
	test -d "$oldincludedir/$2" && $1="[$]$1 $oldincludedir/$2"
}

$1="[$]$1 $cf_header_path_list"
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_INSTALL_OPTS version: 3 updated: 2023/06/03 15:17:30
dnl ---------------
dnl prompt for/fill-in useful install-program options
AC_DEFUN([CF_INSTALL_OPTS],
[
CF_INSTALL_OPT_S
CF_INSTALL_OPT_P
CF_INSTALL_OPT_O
CF_INSTALL_OPT_STRIP_PROG
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_INSTALL_OPT_O version: 3 updated: 2020/12/31 20:19:42
dnl ----------------
dnl Almost all "install" programs default to the current user's ownership.
dnl Almost - MINIX is an exception.
AC_DEFUN([CF_INSTALL_OPT_O],
[
AC_MSG_CHECKING(if install needs to be told about ownership)
case `$ac_config_guess` in
(*minix)
	with_install_o=yes
	;;
(*)
	with_install_o=no
	;;
esac

AC_MSG_RESULT($with_install_o)
if test "x$with_install_o" = xyes
then
	INSTALL_OPT_O="`id root|sed -e 's/uid=[[0-9]]*(/ -o /' -e 's/gid=[[0-9]]*(/ -g /' -e 's/ [[^=[:space:]]][[^=[:space:]]]*=.*/ /' -e 's/)//g'`"
else
	INSTALL_OPT_O=
fi

AC_SUBST(INSTALL_OPT_O)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_INSTALL_OPT_P version: 3 updated: 2021/01/01 13:31:04
dnl ----------------
dnl Some install-programs accept a "-p" option to preserve file modification
dnl timestamps.  That can be useful as an install option, as well as a way to
dnl avoid the need for ranlib after copying a static archive.
AC_DEFUN([CF_INSTALL_OPT_P],
[
: "${INSTALL:=install}"
AC_CACHE_CHECK(if install accepts -p option, cf_cv_install_p,[
	rm -rf ./conftest*
	date >conftest.in
	mkdir conftest.out
	sleep 3
	if $INSTALL -p conftest.in conftest.out 2>/dev/null
	then
		if test -f conftest.out/conftest.in
		then
			test conftest.in -nt conftest.out/conftest.in 2>conftest.err && \
			test conftest.out/conftest.in -nt conftest.in 2>conftest.err
			if test -s conftest.err
			then
				cf_cv_install_p=no
			else
				cf_cv_install_p=yes
			fi
		else
			cf_cv_install_p=no
		fi
	else
		cf_cv_install_p=no
	fi
	rm -rf ./conftest*
])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_INSTALL_OPT_S version: 3 updated: 2021/01/05 19:23:48
dnl ----------------
dnl By default, we should strip executables which are installed, but leave the
dnl ability to suppress that for unit-testing.
AC_DEFUN([CF_INSTALL_OPT_S],
[
AC_MSG_CHECKING(if you want to install stripped executables)
CF_ARG_DISABLE(stripping,
	[  --disable-stripping     do not strip (debug info) installed executables],
	[enable_stripping=no],
	[enable_stripping=yes])
AC_MSG_RESULT($enable_stripping)

if test "$enable_stripping" = yes
then
	INSTALL_OPT_S="-s"
else
	INSTALL_OPT_S=
fi
AC_SUBST(INSTALL_OPT_S)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_INSTALL_OPT_STRIP_PROG version: 1 updated: 2023/06/03 15:17:30
dnl -------------------------
dnl Provide an option for overriding the strip program used in install "-s"
dnl
dnl coreutils install provides a --strip-program option
dnl FreeBSD uses STRIPBIN environment variable, while NetBSD and OpenBSD use
dnl STRIP environment variable.  Other versions of install do not support this.
AC_DEFUN([CF_INSTALL_OPT_STRIP_PROG],
[
AC_REQUIRE([CF_INSTALL_OPT_S])
if test -n "$INSTALL_OPT_S"
then
	AC_MSG_CHECKING(if you want to specify strip-program)
	AC_ARG_WITH(strip-program,
		[  --with-strip-program=XX specify program to use when stripping in install],
		[with_strip_program=$withval],
		[with_strip_program=no])
	AC_MSG_RESULT($with_strip_program)
	if test "$with_strip_program" != no
	then
		AC_MSG_CHECKING(if strip-program is supported with this installer)
		cf_install_program=`echo "$INSTALL" | sed -e 's%[[ ]]*[[ ]]-.%%'`
		check_install_strip=no
		if test -f "$cf_install_program"
		then
			check_install_version=`"$cf_install_program" --version 2>/dev/null | head -n 1 | grep coreutils`
			if test -n "$check_install_version"
			then
				check_install_strip="option"
			else
				for check_strip_variable in STRIPBIN STRIP
				do
					if strings "$cf_install_program" | grep "^$check_strip_variable[$]" >/dev/null
					then
						check_install_strip="environ"
						break
					fi
				done
			fi
		fi
		AC_MSG_RESULT($check_install_strip)
		case "$check_install_strip" in
		(no)
			AC_MSG_WARN($cf_install_program does not support strip program option)
			with_strip_program=no
			;;
		(environ)
			cat >install.tmp <<-CF_EOF
			#! $SHELL
			STRIPBIN="$with_strip_program" \\
			STRIP="$with_strip_program" \\
			$INSTALL "[$]@"
			CF_EOF
			INSTALL="`pwd`/install.tmp"
			chmod +x "$INSTALL"
			CF_VERBOSE(created $INSTALL)
			;;
		(option)
			INSTALL_OPT_S="$INSTALL_OPT_S --strip-program=\"$with_strip_program\""
			;;
		esac
	fi
fi
AC_SUBST(INSTALL_OPT_S)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_INTEL_COMPILER version: 9 updated: 2023/02/18 17:41:25
dnl -----------------
dnl Check if the given compiler is really the Intel compiler for Linux.  It
dnl tries to imitate gcc, but does not return an error when it finds a mismatch
dnl between prototypes, e.g., as exercised by CF_MISSING_CHECK.
dnl
dnl This macro should be run "soon" after AC_PROG_CC or AC_PROG_CPLUSPLUS, to
dnl ensure that it is not mistaken for gcc/g++.  It is normally invoked from
dnl the wrappers for gcc and g++ warnings.
dnl
dnl $1 = GCC (default) or GXX
dnl $2 = INTEL_COMPILER (default) or INTEL_CPLUSPLUS
dnl $3 = CFLAGS (default) or CXXFLAGS
AC_DEFUN([CF_INTEL_COMPILER],[
AC_REQUIRE([AC_CANONICAL_HOST])
ifelse([$2],,INTEL_COMPILER,[$2])=no

if test "$ifelse([$1],,[$1],GCC)" = yes ; then
	case "$host_os" in
	(linux*|gnu*)
		AC_MSG_CHECKING(if this is really Intel ifelse([$1],GXX,C++,C) compiler)
		cf_save_CFLAGS="$ifelse([$3],,CFLAGS,[$3])"
		ifelse([$3],,CFLAGS,[$3])="$ifelse([$3],,CFLAGS,[$3]) -no-gcc"
		AC_TRY_COMPILE([],[
#ifdef __INTEL_COMPILER
#else
#error __INTEL_COMPILER is not defined
#endif
],[ifelse([$2],,INTEL_COMPILER,[$2])=yes
cf_save_CFLAGS="$cf_save_CFLAGS -we147"
],[])
		ifelse([$3],,CFLAGS,[$3])="$cf_save_CFLAGS"
		AC_MSG_RESULT($ifelse([$2],,INTEL_COMPILER,[$2]))
		;;
	esac
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_LARGEFILE version: 13 updated: 2023/12/03 19:09:59
dnl ------------
dnl Add checks for large file support.
AC_DEFUN([CF_LARGEFILE],[
ifdef([AC_FUNC_FSEEKO],[
	AC_SYS_LARGEFILE
	if test "$enable_largefile" != no ; then
	AC_FUNC_FSEEKO

	# Normally we would collect these definitions in the config.h,
	# but (like _XOPEN_SOURCE), some environments rely on having these
	# defined before any of the system headers are included.  Another
	# case comes up with C++, e.g., on AIX the compiler compiles the
	# header files by themselves before looking at the body files it is
	# told to compile.  For ncurses, those header files do not include
	# the config.h
	if test "$ac_cv_sys_large_files" != no
	then
		CF_APPEND_TEXT(CPPFLAGS,-D_LARGE_FILES)
	fi
	if test "$ac_cv_sys_largefile_source" != no
	then
		CF_APPEND_TEXT(CPPFLAGS,-D_LARGEFILE_SOURCE)
	fi
	if test "$ac_cv_sys_file_offset_bits" != no
	then
		CF_APPEND_TEXT(CPPFLAGS,-D_FILE_OFFSET_BITS=$ac_cv_sys_file_offset_bits)
	fi

	AC_CACHE_CHECK(whether to use struct dirent64, cf_cv_struct_dirent64,[
		AC_TRY_COMPILE([
#pragma GCC diagnostic error "-Wincompatible-pointer-types"
#include <sys/types.h>
#include <dirent.h>

#ifndef __REDIRECT
/* if transitional largefile support is setup, this is true */
extern struct dirent64 * readdir(DIR *);
#endif
		],[
		DIR *dp = opendir(".");
		struct dirent64 *x = readdir(dp);
		struct dirent *y = readdir(dp);
		int z = x - y;
		(void)z;
		],
		[cf_cv_struct_dirent64=yes],
		[cf_cv_struct_dirent64=no])
	])
	test "$cf_cv_struct_dirent64" = yes && AC_DEFINE(HAVE_STRUCT_DIRENT64,1,[Define to 1 if we have struct dirent64])
	fi
])
])
dnl ---------------------------------------------------------------------------
dnl CF_LD_SEARCHPATH version: 4 updated: 2022/08/27 15:43:08
dnl ----------------
dnl Try to obtain the linker's search-path, for use in scripts.
dnl
dnl Ignore LD_LIBRARY_PATH, etc.
AC_DEFUN([CF_LD_SEARCHPATH],[
AC_CACHE_CHECK(for linker search path,cf_cv_ld_searchpath,[

if test "$cross_compiling" != yes ; then

# GNU binutils' ld does not involve permissions which may stop ldconfig.
cf_pathlist=`${LD:-ld} --verbose 2>/dev/null | grep SEARCH_DIR | sed -e 's,SEARCH_DIR[[("=]][[("=]]*,,g' -e 's/"[[)]];//gp' | sort -u`

# The -NX options tell newer versions of Linux ldconfig to not attempt to
# update the cache, which makes it run faster.
test -z "$cf_pathlist" && \
	cf_pathlist=`(ldconfig -NX -v) 2>/dev/null | sed -e '/^[[ 	]]/d' -e 's/:$//' | sort -u`

test -z "$cf_pathlist" &&
	cf_pathlist=`(ldconfig -v) 2>/dev/null | sed -n -e '/^[[ 	]]/d' -e 's/:$//p' | sort -u`

# This works with OpenBSD 6.5, which lists only filenames
test -z "$cf_pathlist" &&
	cf_pathlist=`(ldconfig -v) 2>/dev/null | sed -n -e 's,^Adding \(.*\)/.*[$],\1,p' | sort -u`

if test -z "$cf_pathlist"
then
	# dyld default path with MacOS
	if test -f /usr/bin/otool && test "x`uname -s`" = xDarwin
	then
		# do this to bypass check
		cf_cv_ld_searchpath='$HOME/lib'
		cf_pathlist="/usr/local/lib /lib /usr/lib"
	fi
fi

if test -z "$cf_pathlist"
then
	# Solaris is "SunOS"
	if test -f /usr/bin/isainfo && test "x`uname -s`" = xSunOS
	then
		case x`(isainfo -b)` in
		(x64)
			cf_pathlist="$cf_pathlist /lib/64 /usr/lib/64"
			;;
		(x32)
			test -d /usr/ccs/lib && cf_pathlist="$cf_pathlist /usr/ccs/lib"
			cf_pathlist="$cf_pathlist /lib /usr/lib"
			;;
		(*)
			AC_MSG_WARN(problem with Solaris architecture)
			;;
		esac
	fi
fi

if test -z "$cf_pathlist"
then
	# HP-UX
	if test x"`uname -s`" = xHP-UX
	then
		case x`getconf LONG_BIT` in
		(x64)
			cf_pathlist="/usr/lib/hpux64"
			;;
		(x*)
			cf_pathlist="/usr/lib/hpux32"
			;;
		esac
	fi
fi

fi

# If nothing else, assume it is conventional
test -z "$cf_pathlist" && cf_pathlist="/usr/lib /lib"

# Finally, check that this is only directories
for cf_path in [$]0 $cf_pathlist
do
	if test -d "$cf_path"; then
		test -n "$cf_cv_ld_searchpath" && cf_cv_ld_searchpath="${cf_cv_ld_searchpath} "
		cf_cv_ld_searchpath="${cf_cv_ld_searchpath}${cf_path}"
	fi
done

# Ensure that it is nonempty
test -z "$cf_cv_ld_searchpath" && cf_cv_ld_searchpath=/usr/lib
])

LD_SEARCHPATH=`echo "$cf_cv_ld_searchpath"|sed -e 's/ /|/g'`
AC_SUBST(LD_SEARCHPATH)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_LIBRARY_PATH version: 11 updated: 2021/01/01 13:31:04
dnl ---------------
dnl Construct a search-list of directories for a nonstandard library-file
dnl
dnl Parameters
dnl	$1 = the variable to return as result
dnl	$2 = the package name
AC_DEFUN([CF_LIBRARY_PATH],
[
$1=
cf_library_path_list=""
if test -n "${LDFLAGS}${LIBS}" ; then
	for cf_library_path in $LDFLAGS $LIBS
	do
		case "$cf_library_path" in
		(-L*)
			cf_library_path=`echo ".$cf_library_path" |sed -e 's/^...//' -e 's,/lib$,,'`
			CF_ADD_SUBDIR_PATH($1,$2,lib,$cf_library_path,NONE)
			cf_library_path_list="$cf_library_path_list [$]$1"
			;;
		esac
	done
fi

CF_SUBDIR_PATH($1,$2,lib)

$1="$cf_library_path_list [$]$1"
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_LIB_PREFIX version: 14 updated: 2021/01/01 13:31:04
dnl -------------
dnl Compute the library-prefix for the given host system
dnl $1 = variable to set
define([CF_LIB_PREFIX],
[
	case "$cf_cv_system_name" in
	(OS/2*|os2*)
		if test "$DFT_LWR_MODEL" = libtool; then
			LIB_PREFIX='lib'
		else
			LIB_PREFIX=''
		fi
		;;
	(*-msvc*)
		LIB_PREFIX=''
		;;
	(*)	LIB_PREFIX='lib'
		;;
	esac
ifelse($1,,,[$1=$LIB_PREFIX])
	AC_SUBST(LIB_PREFIX)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_LOCALE version: 7 updated: 2023/01/11 04:05:23
dnl ---------
dnl Check if we have setlocale() and its header, <locale.h>
dnl The optional parameter $1 tells what to do if we do have locale support.
AC_DEFUN([CF_LOCALE],
[
AC_MSG_CHECKING(for setlocale())
AC_CACHE_VAL(cf_cv_locale,[
AC_TRY_LINK([
$ac_includes_default
#include <locale.h>],
	[setlocale(LC_ALL, "")],
	[cf_cv_locale=yes],
	[cf_cv_locale=no])
	])
AC_MSG_RESULT($cf_cv_locale)
test "$cf_cv_locale" = yes && { ifelse($1,,AC_DEFINE(LOCALE,1,[Define to 1 if we have locale support]),[$1]) }
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_MAKEFLAGS version: 21 updated: 2021/09/04 06:47:34
dnl ------------
dnl Some 'make' programs support ${MAKEFLAGS}, some ${MFLAGS}, to pass 'make'
dnl options to lower-levels.  It is very useful for "make -n" -- if we have it.
dnl (GNU 'make' does both, something POSIX 'make', which happens to make the
dnl ${MAKEFLAGS} variable incompatible because it adds the assignments :-)
AC_DEFUN([CF_MAKEFLAGS],
[AC_REQUIRE([AC_PROG_FGREP])dnl

AC_CACHE_CHECK(for makeflags variable, cf_cv_makeflags,[
	cf_cv_makeflags=''
	for cf_option in '-${MAKEFLAGS}' '${MFLAGS}'
	do
		cat >cf_makeflags.tmp <<CF_EOF
SHELL = $SHELL
all :
	@ echo '.$cf_option'
CF_EOF
		cf_result=`${MAKE:-make} -k -f cf_makeflags.tmp 2>/dev/null | ${FGREP-fgrep} -v "ing directory" | sed -e 's,[[ 	]]*$,,'`
		case "$cf_result" in
		(.*k|.*kw)
			cf_result="`${MAKE:-make} -k -f cf_makeflags.tmp CC=cc 2>/dev/null`"
			case "$cf_result" in
			(.*CC=*)	cf_cv_makeflags=
				;;
			(*)	cf_cv_makeflags=$cf_option
				;;
			esac
			break
			;;
		(.-)
			;;
		(*)
			CF_MSG_LOG(given option \"$cf_option\", no match \"$cf_result\")
			;;
		esac
	done
	rm -f cf_makeflags.tmp
])

AC_SUBST(cf_cv_makeflags)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_MAKE_AR_RULES version: 8 updated: 2021/01/10 18:23:00
dnl ----------------
dnl Check if the 'make' program knows how to interpret archive rules.  Though
dnl this has been common practice since the mid-80's, there were some holdouts
dnl noted in 1997 among the BSD-based Unix systems.  Twenty years later, some
dnl BSDs still fail to provide this POSIX feature.
dnl
dnl For reference, here is the 2004 edition:
dnl
dnl https://pubs.opengroup.org/onlinepubs/009695399/utilities/make.html
dnl
dnl If a target or prerequisite contains parentheses, it shall be treated as a
dnl member of an archive library.  For the lib( member .o) expression lib
dnl refers to the name of the archive library and member .o to the member name. 
dnl The application shall ensure that the member is an object file with the .o
dnl suffix.  The modification time of the expression is the modification time
dnl for the member as kept in the archive library; see ar.  The .a suffix shall
dnl refer to an archive library.  The .s2.a rule shall be used to update a
dnl member in the library from a file with a suffix .s2.
AC_DEFUN([CF_MAKE_AR_RULES],
[
AC_MSG_CHECKING(if ${MAKE:-make} program knows about archives)
AC_CACHE_VAL(cf_cv_ar_rules,[
cf_dir=subd$$
cf_cv_ar_rules=unknown
mkdir "$cf_dir"
cat >"$cf_dir/makefile" <<CF_EOF
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
cat >$cf_dir/conftest.c <<CF_EOF
#include <stdio.h>
extern int conftest(char *name);
int conftest(char *name)
{
	FILE *fp = fopen(name, "r");
	int rc = 0;
	if (fp != 0) {
		fclose(fp);
		rc = 1;
	}
	return rc;
}
CF_EOF
CDPATH=; export CDPATH
if ( cd "$cf_dir" && "${MAKE:-make}" 2>&AC_FD_CC >&AC_FD_CC && test -f conf.a )
then
	cf_cv_ar_rules=yes
else
	echo ... library-rule did not create an archive >&AC_FD_CC
	rm -f "$cf_dir/conftest.o"
	cat >"$cf_dir/makefile" <<CF_EOF
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
	if ( cd "$cf_dir" && "${MAKE:-make}" 2>&AC_FD_CC >&AC_FD_CC && test -f conf.a )
	then
		cf_cv_ar_rules=no
	else
		AC_MSG_ERROR(I do not know how to construct a library)
	fi
fi
rm -rf "$cf_dir"
])
AC_MSG_RESULT($cf_cv_ar_rules)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_MAKE_INCLUDE version: 10 updated: 2021/01/10 18:23:00
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
	eval `(cd "$cf_dir" && "${MAKE:-make}") 2>&AC_FD_CC | grep cf_make_include=OK`
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
	AC_MSG_ERROR(Your $ac_make program does not support includes)
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
dnl CF_MAKE_TAGS version: 6 updated: 2010/10/23 15:52:32
dnl ------------
dnl Generate tags/TAGS targets for makefiles.  Do not generate TAGS if we have
dnl a monocase filesystem.
AC_DEFUN([CF_MAKE_TAGS],[
AC_REQUIRE([CF_MIXEDCASE_FILENAMES])

AC_CHECK_PROGS(CTAGS, exctags ctags)
AC_CHECK_PROGS(ETAGS, exetags etags)

AC_CHECK_PROG(MAKE_LOWER_TAGS, ${CTAGS:-ctags}, yes, no)

if test "$cf_cv_mixedcase" = yes ; then
	AC_CHECK_PROG(MAKE_UPPER_TAGS, ${ETAGS:-etags}, yes, no)
else
	MAKE_UPPER_TAGS=no
fi

if test "$MAKE_UPPER_TAGS" = yes ; then
	MAKE_UPPER_TAGS=
else
	MAKE_UPPER_TAGS="#"
fi

if test "$MAKE_LOWER_TAGS" = yes ; then
	MAKE_LOWER_TAGS=
else
	MAKE_LOWER_TAGS="#"
fi

AC_SUBST(CTAGS)
AC_SUBST(ETAGS)

AC_SUBST(MAKE_UPPER_TAGS)
AC_SUBST(MAKE_LOWER_TAGS)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_MIXEDCASE_FILENAMES version: 9 updated: 2021/01/01 16:53:59
dnl ----------------------
dnl Check if the file-system supports mixed-case filenames.  If we're able to
dnl create a lowercase name and see it as uppercase, it doesn't support that.
AC_DEFUN([CF_MIXEDCASE_FILENAMES],
[
AC_CACHE_CHECK(if filesystem supports mixed-case filenames,cf_cv_mixedcase,[
if test "$cross_compiling" = yes ; then
	case "$target_alias" in
	(*-os2-emx*|*-msdosdjgpp*|*-cygwin*|*-msys*|*-mingw*|*-uwin*|darwin*)
		cf_cv_mixedcase=no
		;;
	(*)
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
test "$cf_cv_mixedcase" = yes && AC_DEFINE(MIXEDCASE_FILENAMES,1,[Define to 1 if filesystem supports mixed-case filenames.])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_MKSTEMP version: 13 updated: 2023/12/01 17:22:50
dnl ----------
dnl Check for a working mkstemp.  This creates two files, checks that they are
dnl successfully created and distinct (AmigaOS apparently fails on the last).
AC_DEFUN([CF_MKSTEMP],[
AC_CHECK_HEADERS( \
unistd.h \
)
AC_CACHE_CHECK(for working mkstemp, cf_cv_func_mkstemp,[
rm -rf ./conftest*
AC_TRY_RUN([
$ac_includes_default

int main(void)
{
	static char tmpl[] = "conftestXXXXXX";
	char name[2][80];
	int n;
	int result = 0;
	int fd;
	struct stat sb;

	umask(077);
	for (n = 0; n < 2; ++n) {
		strcpy(name[n], tmpl);
		if ((fd = mkstemp(name[n])) >= 0) {
			if (!strcmp(name[n], tmpl)
			 || stat(name[n], &sb) != 0
			 || (sb.st_mode & S_IFMT) != S_IFREG
			 || (sb.st_mode & 077) != 0) {
				result = 1;
			}
			close(fd);
		}
	}
	if (result == 0
	 && !strcmp(name[0], name[1]))
		result = 1;
	${cf_cv_main_return:-return}(result);
}
],[cf_cv_func_mkstemp=yes
],[cf_cv_func_mkstemp=no
],[cf_cv_func_mkstemp=maybe])
])
if test "x$cf_cv_func_mkstemp" = xmaybe ; then
	AC_CHECK_FUNC(mkstemp)
fi
if test "x$cf_cv_func_mkstemp" = xyes || test "x$ac_cv_func_mkstemp" = xyes ; then
	AC_DEFINE(HAVE_MKSTEMP,1,[Define to 1 if mkstemp() is available and working.])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_MSG_LOG version: 5 updated: 2010/10/23 15:52:32
dnl ----------
dnl Write a debug message to config.log, along with the line number in the
dnl configure script.
AC_DEFUN([CF_MSG_LOG],[
echo "${as_me:-configure}:__oline__: testing $* ..." 1>&AC_FD_CC
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_NCURSES_CC_CHECK version: 6 updated: 2023/02/18 17:47:58
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
	#error WACS_BSSB is not defined
#endif
])[
printf("%s\\n", NCURSES_VERSION);
#else
#ifdef __NCURSES_H
printf("old\\n");
#else
	#error __NCURSES_H is not defined
#endif
#endif
	]
	,[$1=$2]
	,[$1=no])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_NCURSES_CONFIG version: 29 updated: 2025/01/10 19:55:54
dnl -----------------
dnl Tie together the configure-script macros for ncurses, preferring these in
dnl order:
dnl a) ".pc" files for pkg-config, using $NCURSES_CONFIG_PKG
dnl b) the "-config" script from ncurses, using $NCURSES_CONFIG
dnl c) just plain libraries
dnl
dnl $1 is the root library name (default: "ncurses")
AC_DEFUN([CF_NCURSES_CONFIG],[
AC_REQUIRE([CF_PKG_CONFIG])
cf_ncuconfig_root=ifelse($1,,ncurses,$1)
cf_have_ncuconfig=no

if test "x${PKG_CONFIG:=none}" != xnone; then
	AC_MSG_CHECKING(pkg-config for $cf_ncuconfig_root)
	if "$PKG_CONFIG" --exists $cf_ncuconfig_root ; then
		AC_MSG_RESULT(yes)

		AC_MSG_CHECKING(if the $cf_ncuconfig_root package files work)
		cf_have_ncuconfig=unknown

		cf_save_CFLAGS="$CFLAGS"
		cf_save_CPPFLAGS="$CPPFLAGS"
		cf_save_LIBS="$LIBS"

		cf_pkg_cflags="`$PKG_CONFIG --cflags $cf_ncuconfig_root`"
		cf_pkg_libs="`$PKG_CONFIG --libs $cf_ncuconfig_root`"

		# while -W for passing linker flags is prevalent, it is not "standard".
		# At least one wrapper for c89/c99 (in Apple's xcode) has its own
		# incompatible _and_ non-standard -W option which gives an error.  Work
		# around that pitfall.
		case "x${CC}@@${cf_pkg_libs}@${cf_pkg_cflags}" in
		(x*c[[89]]9@@*-W*)
			CF_ADD_CFLAGS($cf_pkg_cflags)
			CF_ADD_LIBS($cf_pkg_libs)

			AC_TRY_LINK([#include <${cf_cv_ncurses_header:-curses.h}>],
				[initscr(); mousemask(0,0); tigetstr((char *)0);],
				[AC_TRY_RUN([#include <${cf_cv_ncurses_header:-curses.h}>
					int main(void)
					{ const char *xx = curses_version(); return (xx == 0); }],
					[cf_test_ncuconfig=yes],
					[cf_test_ncuconfig=no],
					[cf_test_ncuconfig=maybe])],
				[cf_test_ncuconfig=no])

			CFLAGS="$cf_save_CFLAGS"
			CPPFLAGS="$cf_save_CPPFLAGS"
			LIBS="$cf_save_LIBS"

			if test "x$cf_test_ncuconfig" != xyes; then
				cf_temp=`echo "x$cf_pkg_cflags" | sed -e s/^x// -e 's/-W[[^ 	]]*//g'`
				cf_pkg_cflags="$cf_temp"
				cf_temp=`echo "x$cf_pkg_libs" | sed -e s/^x// -e 's/-W[[^ 	]]*//g'`
				cf_pkg_libs="$cf_temp"
			fi
			;;
		esac

		CF_REQUIRE_PKG($cf_ncuconfig_root)
		CF_APPEND_CFLAGS($cf_pkg_cflags)
		CF_ADD_LIBS($cf_pkg_libs)

		AC_TRY_LINK([#include <${cf_cv_ncurses_header:-curses.h}>],
			[initscr(); mousemask(0,0); tigetstr((char *)0);],
			[AC_TRY_RUN([#include <${cf_cv_ncurses_header:-curses.h}>
				int main(void)
				{ const char *xx = curses_version(); return (xx == 0); }],
				[cf_have_ncuconfig=yes],
				[cf_have_ncuconfig=no],
				[cf_have_ncuconfig=maybe])],
			[cf_have_ncuconfig=no])
		AC_MSG_RESULT($cf_have_ncuconfig)
		test "$cf_have_ncuconfig" = maybe && cf_have_ncuconfig=yes
		if test "$cf_have_ncuconfig" != "yes"
		then
			CPPFLAGS="$cf_save_CPPFLAGS"
			LIBS="$cf_save_LIBS"
			NCURSES_CONFIG_PKG=none
		else
			AC_DEFINE(NCURSES,1,[Define to 1 if we are using ncurses headers/libraries])
			NCURSES_CONFIG_PKG=$cf_ncuconfig_root
			CF_TERM_HEADER
		fi

	else
		AC_MSG_RESULT(no)
		NCURSES_CONFIG_PKG=none
	fi
else
	NCURSES_CONFIG_PKG=none
fi

if test "x$cf_have_ncuconfig" = "xno"; then
	cf_ncurses_config="${cf_ncuconfig_root}${NCURSES_CONFIG_SUFFIX}-config"; echo "Looking for ${cf_ncurses_config}"

	CF_ACVERSION_CHECK(2.52,
		[AC_CHECK_TOOLS(NCURSES_CONFIG, ${cf_ncurses_config} ${cf_ncuconfig_root}6-config ${cf_ncuconfig_root}6-config ${cf_ncuconfig_root}5-config, none)],
		[AC_PATH_PROGS(NCURSES_CONFIG,  ${cf_ncurses_config} ${cf_ncuconfig_root}6-config ${cf_ncuconfig_root}6-config ${cf_ncuconfig_root}5-config, none)])

	if test "$NCURSES_CONFIG" != none ; then

		CF_APPEND_CFLAGS(`$NCURSES_CONFIG --cflags`)
		CF_ADD_LIBS(`$NCURSES_CONFIG --libs`)

		# even with config script, some packages use no-override for curses.h
		CF_CURSES_HEADER(ifelse($1,,ncurses,$1))

		dnl like CF_NCURSES_CPPFLAGS
		AC_DEFINE(NCURSES,1,[Define to 1 if we are using ncurses headers/libraries])

		dnl like CF_NCURSES_LIBS
		CF_UPPER(cf_nculib_ROOT,HAVE_LIB$cf_ncuconfig_root)
		AC_DEFINE_UNQUOTED($cf_nculib_ROOT)

		dnl like CF_NCURSES_VERSION
		cf_cv_ncurses_version="`$NCURSES_CONFIG --version`"

	else

		CF_NCURSES_CPPFLAGS(ifelse($1,,ncurses,$1))
		CF_NCURSES_LIBS(ifelse($1,,ncurses,$1))

	fi
else
	NCURSES_CONFIG=none
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_NCURSES_CPPFLAGS version: 22 updated: 2021/01/02 09:31:20
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
test "$cf_cv_curses_dir" != "no" && { \
  CF_ADD_INCDIR($cf_cv_curses_dir/include/$cf_ncuhdr_root)
}

AC_CACHE_CHECK(for $cf_ncuhdr_root header in include-path, cf_cv_ncurses_h,[
	cf_header_list="$cf_ncuhdr_root/curses.h $cf_ncuhdr_root/ncurses.h"
	{ test "$cf_ncuhdr_root" = ncurses || test "$cf_ncuhdr_root" = ncursesw; } && cf_header_list="$cf_header_list curses.h ncurses.h"
	for cf_header in $cf_header_list
	do
		CF_NCURSES_CC_CHECK(cf_cv_ncurses_h,$cf_header,$1)
		test "$cf_cv_ncurses_h" != no && break
	done
])

CF_NCURSES_HEADER
CF_TERM_HEADER

# some applications need this, but should check for NCURSES_VERSION
AC_DEFINE(NCURSES,1,[Define to 1 if we are using ncurses headers/libraries])

CF_NCURSES_VERSION
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_NCURSES_HEADER version: 7 updated: 2021/01/04 19:33:05
dnl -----------------
dnl Find a "curses" header file, e.g,. "curses.h", or one of the more common
dnl variations of ncurses' installs.
dnl
dnl See also CF_CURSES_HEADER, which sets the same cache variable.
AC_DEFUN([CF_NCURSES_HEADER],[

if test "$cf_cv_ncurses_h" != no ; then
	cf_cv_ncurses_header=$cf_cv_ncurses_h
else

AC_CACHE_CHECK(for $cf_ncuhdr_root include-path, cf_cv_ncurses_h2,[
	test -n "$verbose" && echo
	CF_HEADER_PATH(cf_search,$cf_ncuhdr_root)
	test -n "$verbose" && echo "search path $cf_search"
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
				test -n "$verbose" && echo $ECHO_N "	... found $ECHO_C" 1>&AC_FD_MSG
				break
			fi
			test -n "$verbose" && echo "	... tested $cf_incdir/$cf_header" 1>&AC_FD_MSG
		done
		CPPFLAGS="$cf_save2_CPPFLAGS"
		test "$cf_cv_ncurses_h2" != no && break
	done
	test "$cf_cv_ncurses_h2" = no && AC_MSG_ERROR(not found)
	])

	CF_DIRNAME(cf_1st_incdir,$cf_cv_ncurses_h2)
	cf_cv_ncurses_header="`basename "$cf_cv_ncurses_h2"`"
	if test "`basename "$cf_1st_incdir"`" = "$cf_ncuhdr_root" ; then
		cf_cv_ncurses_header="$cf_ncuhdr_root/$cf_cv_ncurses_header"
	fi
	CF_ADD_INCDIR($cf_1st_incdir)

fi

# Set definitions to allow ifdef'ing for ncurses.h

case "$cf_cv_ncurses_header" in
(*ncurses.h)
	AC_DEFINE(HAVE_NCURSES_H,1,[Define to 1 if we have ncurses.h])
	;;
esac

case "$cf_cv_ncurses_header" in
(ncurses/curses.h|ncurses/ncurses.h)
	AC_DEFINE(HAVE_NCURSES_NCURSES_H,1,[Define to 1 if we have ncurses/ncurses.h])
	;;
(ncursesw/curses.h|ncursesw/ncurses.h)
	AC_DEFINE(HAVE_NCURSESW_NCURSES_H,1,[Define to 1 if we have ncursesw/ncurses.h])
	;;
esac

])dnl
dnl ---------------------------------------------------------------------------
dnl CF_NCURSES_LIBS version: 21 updated: 2021/09/04 06:37:12
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

case "$host_os" in
(freebsd*)
	# This is only necessary if you are linking against an obsolete
	# version of ncurses (but it should do no harm, since it is static).
	if test "$cf_nculib_root" = ncurses ; then
		AC_CHECK_LIB(mytinfo,tgoto,[cf_ncurses_LIBS="-lmytinfo $cf_ncurses_LIBS"])
	fi
	;;
esac

CF_ADD_LIBS($cf_ncurses_LIBS)

if test -n "$cf_cv_curses_dir" && test "$cf_cv_curses_dir" != "no"
then
	CF_ADD_LIBS(-l$cf_nculib_root)
else
	CF_FIND_LIBRARY($cf_nculib_root,$cf_nculib_root,
		[#include <${cf_cv_ncurses_header:-curses.h}>],
		[initscr()],
		initscr)
fi

if test -n "$cf_ncurses_LIBS" ; then
	AC_MSG_CHECKING(if we can link $cf_nculib_root without $cf_ncurses_LIBS)
	cf_ncurses_SAVE="$LIBS"
	for p in $cf_ncurses_LIBS ; do
		q=`echo "$LIBS" | sed -e "s%$p %%" -e "s%$p$%%"`
		if test "$q" != "$LIBS" ; then
			LIBS="$q"
		fi
	done
	AC_TRY_LINK([#include <${cf_cv_ncurses_header:-curses.h}>],
		[initscr(); mousemask(0,0); tigetstr((char *)0);],
		[AC_MSG_RESULT(yes)],
		[AC_MSG_RESULT(no)
		 LIBS="$cf_ncurses_SAVE"])
fi

CF_UPPER(cf_nculib_ROOT,HAVE_LIB$cf_nculib_root)
AC_DEFINE_UNQUOTED($cf_nculib_ROOT)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_NCURSES_PTHREADS version: 3 updated: 2024/07/06 13:57:42
dnl -------------------
dnl Use this followup check, e.g., in CF_WITH_NCURSES_ETC, to ensure that we
dnl link with pthreads if ncurses uses it.
AC_DEFUN([CF_NCURSES_PTHREADS],[
AC_CHECK_FUNC(_nc_init_pthreads,
	cf_cv_ncurses_pthreads=yes,
	cf_cv_ncurses_pthreads=no)
if test "$cf_cv_ncurses_pthreads" = yes
then
	CF_ADD_LIBS(-lpthread)
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_NCURSES_VERSION version: 18 updated: 2024/01/07 06:34:16
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
	rm -f "$cf_tempfile"
	AC_TRY_RUN([
$ac_includes_default

#include <${cf_cv_ncurses_header:-curses.h}>

int main(void)
{
	FILE *fp = fopen("$cf_tempfile", "w");
#ifdef NCURSES_VERSION
# ifdef NCURSES_VERSION_PATCH
	fprintf(fp, "%s.%d\\n", NCURSES_VERSION, NCURSES_VERSION_PATCH);
# else
	fprintf(fp, "%s\\n", NCURSES_VERSION);
# endif
#else
# ifdef __NCURSES_H
	fprintf(fp, "old\\n");
# else
	#error expected ncurses header to define __NCURSES_H
# endif
#endif
	${cf_cv_main_return:-return}(0);
}],[
	cf_cv_ncurses_version=`cat $cf_tempfile`],,[

	# This will not work if the preprocessor splits the line after the
	# Autoconf token.  The 'unproto' program does that.
	cat > "conftest.$ac_ext" <<EOF
#include <${cf_cv_ncurses_header:-curses.h}>
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
		cf_out=`sed -e 's%^Autoconf %%' -e 's%^[[^"]]*"%%' -e 's%".*%%' conftest.out`
		test -n "$cf_out" && cf_cv_ncurses_version="$cf_out"
		rm -f conftest.out
	fi
])
	rm -f "$cf_tempfile"
])
test "$cf_cv_ncurses_version" = no || AC_DEFINE(NCURSES,1,[Define to 1 if we are using ncurses headers/libraries])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_NO_LEAKS_OPTION version: 9 updated: 2021/06/13 19:45:41
dnl ------------------
dnl see CF_WITH_NO_LEAKS
dnl
dnl $1 = option/name
dnl $2 = help-text
dnl $3 = symbol to define if the option is set
dnl $4 = additional actions to take if the option is set
AC_DEFUN([CF_NO_LEAKS_OPTION],[
AC_MSG_CHECKING(if you want to use $1 for testing)
AC_ARG_WITH($1,
	[$2],
	[case "x$withval" in
	(x|xno) ;;
	(*)
		: "${with_cflags:=-g}"
		: "${enable_leaks:=no}"
		with_$1=yes
		AC_DEFINE_UNQUOTED($3,1,"Define to 1 if you want to use $1 for testing.")ifelse([$4],,[
	 $4
])
		;;
	esac],
	[with_$1=])
AC_MSG_RESULT(${with_$1:-no})

case ".$with_cflags" in
(.*-g*)
	case .$CFLAGS in
	(.*-g*)
		;;
	(*)
		CF_ADD_CFLAGS([-g])
		;;
	esac
	;;
esac
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
dnl CF_PATHSEP version: 8 updated: 2021/01/01 13:31:04
dnl ----------
dnl Provide a value for the $PATH and similar separator (or amend the value
dnl as provided in autoconf 2.5x).
AC_DEFUN([CF_PATHSEP],
[
	AC_MSG_CHECKING(for PATH separator)
	case "$cf_cv_system_name" in
	(os2*)	PATH_SEPARATOR=';'  ;;
	(*)	${PATH_SEPARATOR:=':'}  ;;
	esac
ifelse([$1],,,[$1=$PATH_SEPARATOR])
	AC_SUBST(PATH_SEPARATOR)
	AC_MSG_RESULT($PATH_SEPARATOR)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_PATH_SYNTAX version: 19 updated: 2024/08/03 13:08:58
dnl --------------
dnl Check the argument to see that it looks like a pathname.  Rewrite it if it
dnl begins with one of the prefix/exec_prefix variables, and then again if the
dnl result begins with 'NONE'.  This is necessary to work around autoconf's
dnl delayed evaluation of those symbols.
AC_DEFUN([CF_PATH_SYNTAX],[
AC_REQUIRE([CF_GLOB_FULLPATH])dnl

if test "x$prefix" != xNONE; then
	cf_path_syntax="$prefix"
else
	cf_path_syntax="$ac_default_prefix"
fi

case "x[$]$1" in
(x\[$]\(*\)*|x\'*\'*)
	;;
(x.|x$GLOB_FULLPATH_POSIX|x$GLOB_FULLPATH_OTHER)
	;;
(x\[$]\{*prefix\}*|x\[$]\{*dir\}*)
	eval $1="[$]$1"
	case "x[$]$1" in
	(xNONE/*)
		$1=`echo "[$]$1" | sed -e s%NONE%$cf_path_syntax%`
		;;
	esac
	;;
(xno|xNONE/*)
	$1=`echo "[$]$1" | sed -e s%NONE%$cf_path_syntax%`
	;;
(*)
	ifelse([$2],,[AC_MSG_ERROR([expected a pathname, not \"[$]$1\"])],$2)
	;;
esac
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_PDCURSES_X11 version: 15 updated: 2021/01/02 09:31:20
dnl ---------------
dnl Configure for PDCurses' X11 library
AC_DEFUN([CF_PDCURSES_X11],[
AC_REQUIRE([CF_X_ATHENA])

CF_ACVERSION_CHECK(2.52,
	[AC_CHECK_TOOLS(XCURSES_CONFIG, xcurses-config, none)],
	[AC_PATH_PROGS(XCURSES_CONFIG, xcurses-config, none)])

if test "$XCURSES_CONFIG" != none ; then

CF_ADD_CFLAGS(`$XCURSES_CONFIG --cflags`)
CF_ADD_LIBS(`$XCURSES_CONFIG --libs`)

cf_cv_lib_XCurses=yes

else

LDFLAGS="$LDFLAGS $X_LIBS"
CF_CHECK_CFLAGS($X_CFLAGS)
AC_CHECK_LIB(X11,XOpenDisplay,
	[CF_ADD_LIBS(-lX11)],,
	[$X_PRE_LIBS $LIBS $X_EXTRA_LIBS])
AC_CACHE_CHECK(for XCurses library,cf_cv_lib_XCurses,[
CF_ADD_LIBS(-lXCurses)
AC_TRY_LINK([
#include <xcurses.h>
char *XCursesProgramName = "test";
],[XCursesExit();],
[cf_cv_lib_XCurses=yes],
[cf_cv_lib_XCurses=no])
])

fi

if test "$cf_cv_lib_XCurses" = yes ; then
	AC_DEFINE(UNIX,1,[Define to 1 if using PDCurses on Unix])
	AC_DEFINE(XCURSES,1,[Define to 1 if using PDCurses on Unix])
	AC_CHECK_HEADER(xcurses.h, AC_DEFINE(HAVE_XCURSES,1,[Define to 1 if using PDCurses on Unix]))
else
	AC_MSG_ERROR(Cannot link with XCurses)
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_PKG_CONFIG version: 13 updated: 2023/10/28 11:59:01
dnl -------------
dnl Check for the package-config program, unless disabled by command-line.
dnl
dnl Sets $PKG_CONFIG to the pathname of the pkg-config program.
AC_DEFUN([CF_PKG_CONFIG],
[
AC_MSG_CHECKING(if you want to use pkg-config)
AC_ARG_WITH(pkg-config,
	[[  --with-pkg-config[=CMD] enable/disable use of pkg-config and its name CMD]],
	[cf_pkg_config=$withval],
	[cf_pkg_config=yes])
AC_MSG_RESULT($cf_pkg_config)

case "$cf_pkg_config" in
(no)
	PKG_CONFIG=none
	;;
(yes)
	CF_ACVERSION_CHECK(2.52,
		[AC_PATH_TOOL(PKG_CONFIG, pkg-config, none)],
		[AC_PATH_PROG(PKG_CONFIG, pkg-config, none)])
	;;
(*)
	PKG_CONFIG=$withval
	;;
esac

test -z "$PKG_CONFIG" && PKG_CONFIG=none
if test "$PKG_CONFIG" != none ; then
	CF_PATH_SYNTAX(PKG_CONFIG)
elif test "x$cf_pkg_config" != xno ; then
	AC_MSG_WARN(pkg-config is not installed)
fi

AC_SUBST(PKG_CONFIG)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_POSIX_C_SOURCE version: 12 updated: 2023/02/18 17:41:25
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
[AC_REQUIRE([CF_POSIX_VISIBLE])dnl

if test "$cf_cv_posix_visible" = no; then

cf_POSIX_C_SOURCE=ifelse([$1],,199506L,[$1])

cf_save_CFLAGS="$CFLAGS"
cf_save_CPPFLAGS="$CPPFLAGS"

CF_REMOVE_DEFINE(cf_trim_CFLAGS,$cf_save_CFLAGS,_POSIX_C_SOURCE)
CF_REMOVE_DEFINE(cf_trim_CPPFLAGS,$cf_save_CPPFLAGS,_POSIX_C_SOURCE)

AC_CACHE_CHECK(if we should define _POSIX_C_SOURCE,cf_cv_posix_c_source,[
	CF_MSG_LOG(if the symbol is already defined go no further)
	AC_TRY_COMPILE([#include <sys/types.h>],[
#ifndef _POSIX_C_SOURCE
#error _POSIX_C_SOURCE is not defined
#endif],
	[cf_cv_posix_c_source=no],
	[cf_want_posix_source=no
	 case .$cf_POSIX_C_SOURCE in
	 (.[[12]]??*)
		cf_cv_posix_c_source="-D_POSIX_C_SOURCE=$cf_POSIX_C_SOURCE"
		;;
	 (.2)
		cf_cv_posix_c_source="-D_POSIX_C_SOURCE=$cf_POSIX_C_SOURCE"
		cf_want_posix_source=yes
		;;
	 (.*)
		cf_want_posix_source=yes
		;;
	 esac
	 if test "$cf_want_posix_source" = yes ; then
		AC_TRY_COMPILE([#include <sys/types.h>],[
#ifdef _POSIX_SOURCE
#error _POSIX_SOURCE is defined
#endif],[],
		cf_cv_posix_c_source="$cf_cv_posix_c_source -D_POSIX_SOURCE")
	 fi
	 CF_MSG_LOG(ifdef from value $cf_POSIX_C_SOURCE)
	 CFLAGS="$cf_trim_CFLAGS"
	 CPPFLAGS="$cf_trim_CPPFLAGS"
	 CF_APPEND_TEXT(CPPFLAGS,$cf_cv_posix_c_source)
	 CF_MSG_LOG(if the second compile does not leave our definition intact error)
	 AC_TRY_COMPILE([#include <sys/types.h>],[
#ifndef _POSIX_C_SOURCE
#error _POSIX_C_SOURCE is not defined
#endif],,
	 [cf_cv_posix_c_source=no])
	 CFLAGS="$cf_save_CFLAGS"
	 CPPFLAGS="$cf_save_CPPFLAGS"
	])
])

if test "$cf_cv_posix_c_source" != no ; then
	CFLAGS="$cf_trim_CFLAGS"
	CPPFLAGS="$cf_trim_CPPFLAGS"
	CF_ADD_CFLAGS($cf_cv_posix_c_source)
fi

fi # cf_cv_posix_visible

])dnl
dnl ---------------------------------------------------------------------------
dnl CF_POSIX_VISIBLE version: 1 updated: 2018/12/31 20:46:17
dnl ----------------
dnl POSIX documents test-macros which an application may set before any system
dnl headers are included to make features available.
dnl
dnl Some BSD platforms (originally FreeBSD, but copied by a few others)
dnl diverged from POSIX in 2002 by setting symbols which make all of the most
dnl recent features visible in the system header files unless the application
dnl overrides the corresponding test-macros.  Doing that introduces portability
dnl problems.
dnl
dnl This macro makes a special check for the symbols used for this, to avoid a
dnl conflicting definition.
AC_DEFUN([CF_POSIX_VISIBLE],
[
AC_CACHE_CHECK(if the POSIX test-macros are already defined,cf_cv_posix_visible,[
AC_TRY_COMPILE([#include <stdio.h>],[
#if defined(__POSIX_VISIBLE) && ((__POSIX_VISIBLE - 0L) > 0) \
	&& defined(__XSI_VISIBLE) && ((__XSI_VISIBLE - 0L) > 0) \
	&& defined(__BSD_VISIBLE) && ((__BSD_VISIBLE - 0L) > 0) \
	&& defined(__ISO_C_VISIBLE) && ((__ISO_C_VISIBLE - 0L) > 0)
#error conflicting symbols found
#endif
],[cf_cv_posix_visible=no],[cf_cv_posix_visible=yes])
])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_PROGRAM_FULLPATH version: 8 updated: 2021/01/10 18:23:00
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
			case "[$]1" in
			-*)
				;;
			*)
				if test -f "$cf_word" && test ! -f "./$cf_word" && test -x "$cf_word"; then
					cf_cv_$1="$cf_word"
				else
					IFS="${IFS:- 	}"; cf_save_ifs="$IFS"; IFS="${IFS}${PATH_SEPARATOR}"
					for cf_dir in $PATH; do
						test -z "$cf_dir" && cf_dir=.
						if test "$cf_dir" != "." && test -f "$cf_dir/$cf_word" && test -x "$cf_dir/$cf_word" ; then
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
	AC_DEFINE_UNQUOTED($1,"[$]cf_cv_$1",[Define to program path for $1])
  AC_MSG_RESULT("[$]cf_cv_$1")
else
  AC_MSG_RESULT((not found))
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_PROGRAM_PREFIX version: 7 updated: 2021/01/10 18:23:00
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
		IFS="${IFS:- 	}"; cf_save_ifs="$IFS"; IFS="${IFS}${PATH_SEPARATOR}"
		for cf_dir in $PATH; do
			test -z "$cf_dir" && cf_dir=.
			if test "$cf_dir" != "." && test -f "$cf_dir/$cf_word" && test -x "$cf_dir/$cf_word" ; then
				cf_cv_$1="$cf_dir"
				break
			fi
		done
		IFS="$cf_save_ifs"
	fi
	ifelse([$3],,, [test -z "[$]cf_cv_$1" && cf_cv_$1="$3"])
])
if test -n "[$]cf_cv_$1"; then
  AC_DEFINE_UNQUOTED($1,"[$]cf_cv_$1",[Define to program path for $1])
  AC_MSG_RESULT("[$]cf_cv_$1")
else
  AC_MSG_RESULT((not found))
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_PROG_AR version: 1 updated: 2009/01/01 20:15:22
dnl ----------
dnl Check for archiver "ar".
AC_DEFUN([CF_PROG_AR],[
AC_CHECK_TOOL(AR, ar, ar)
])
dnl ---------------------------------------------------------------------------
dnl CF_PROG_CC version: 5 updated: 2019/12/31 08:53:54
dnl ----------
dnl standard check for CC, plus followup sanity checks
dnl $1 = optional parameter to pass to AC_PROG_CC to specify compiler name
AC_DEFUN([CF_PROG_CC],[
CF_ACVERSION_CHECK(2.53,
	[AC_MSG_WARN(this will incorrectly handle gnatgcc choice)
	 AC_REQUIRE([AC_PROG_CC])],
	[])
ifelse($1,,[AC_PROG_CC],[AC_PROG_CC($1)])
CF_GCC_VERSION
CF_ACVERSION_CHECK(2.52,
	[AC_PROG_CC_STDC],
	[CF_ANSI_CC_REQD])
CF_CC_ENV_FLAGS
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_PROG_EXT version: 15 updated: 2021/01/02 09:31:20
dnl -----------
dnl Compute $PROG_EXT, used for non-Unix ports, such as OS/2 EMX.
AC_DEFUN([CF_PROG_EXT],
[
AC_REQUIRE([CF_CHECK_CACHE])
case "$cf_cv_system_name" in
(os2*)
	CFLAGS="$CFLAGS -Zmt"
	CF_APPEND_TEXT(CPPFLAGS,-D__ST_MT_ERRNO__)
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
test -n "$PROG_EXT" && AC_DEFINE_UNQUOTED(PROG_EXT,"$PROG_EXT",[Define to the program extension (normally blank)])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_PROG_GROFF version: 3 updated: 2018/01/07 13:16:19
dnl -------------
dnl Check if groff is available, for cases (such as html output) where nroff
dnl is not enough.
AC_DEFUN([CF_PROG_GROFF],[
AC_PATH_PROG(GROFF_PATH,groff,no)
AC_PATH_PROGS(NROFF_PATH,nroff mandoc,no)
AC_PATH_PROG(TBL_PATH,tbl,cat)
if test "x$GROFF_PATH" = xno
then
	NROFF_NOTE=
	GROFF_NOTE="#"
else
	NROFF_NOTE="#"
	GROFF_NOTE=
fi
AC_SUBST(GROFF_NOTE)
AC_SUBST(NROFF_NOTE)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_PROG_LINT version: 7 updated: 2024/11/30 14:37:45
dnl ------------
AC_DEFUN([CF_PROG_LINT],
[
AC_CHECK_PROGS(LINT, lint cppcheck splint)
case "x$LINT" in
(xlint|x*/lint) # NetBSD 10
	test -z "$LINT_OPTS" && LINT_OPTS="-chapbrxzgFS -v -Ac11"
	;;
(xcppcheck|x*/cppcheck)
	test -z "$LINT_OPTS" && LINT_OPTS="--enable=all -D__CPPCHECK__"
	;;
esac
AC_SUBST(LINT_OPTS)
AC_SUBST(LINT_LIBS)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_RCS_SCCS version: 6 updated: 2019/12/06 06:57:36
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
CF_PROGRAM_PREFIX(ADMIN_PATH, admin)dnl the SCCS tool
CF_PROGRAM_PREFIX(SCCS_PATH, sccs)dnl a later wrapper for the SCCS tools
CF_PROGRAM_PREFIX(VCS_PATH, vcs)dnl VCS is my RCS application
CF_PROGRAM_PREFIX(CVS_PATH, cvs)dnl CVS is a layer above RCS
CF_PROGRAM_PREFIX(SVN_PATH, svn)dnl SVN is like CVS, using local file status
CF_PROGRAM_PREFIX(CMV_PATH, cmv)dnl CmVision combines RCS and SCCS archives
PATH="$cf_rcs_sccs_path"
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_REGCMP_FUNCS version: 4 updated: 2021/01/10 18:23:00
dnl ---------------
dnl Test for the presence of regcmp/regex functions.
AC_DEFUN([CF_REGCMP_FUNCS],
[
AC_HAVE_HEADERS(pw.h libgen.h)
AC_MSG_CHECKING(regcmp/regex functions)
AC_CACHE_VAL(cf_cv_REGCMP_func,[
	CF_REGCMP_LIBS
	if test "$cf_cv_REGCMP_func" = no; then
		cf_save_LIBS="$LIBS"
		LIBS="-lPW $LIBS"
		CF_REGCMP_LIBS
		LIBS="${cf_save_LIBS}"
		test "$cf_cv_REGCMP_func" = yes && cf_cv_REGCMP_func="yes-lPW"
	fi
])
AC_MSG_RESULT($cf_cv_REGCMP_func)
if test "$cf_cv_REGCMP_func" != no; then
	AC_DEFINE(HAVE_REGCMP_FUNCS,1,[Define to 1 if we have regcmp/regex functions])
	test "$cf_cv_REGCMP_func" != yes && AC_CHECK_LIB(PW, regcmp)
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_REGCMP_LIBS version: 5 updated: 2023/01/05 18:57:15
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
$ac_includes_default

#if HAVE_PW_H
#include <pw.h>		/* _must_ use prototype on CLIX */
#endif

#if HAVE_LIBGEN_H
#include <libgen.h>	/* IRIX defines it here */
#endif

int main(void)
{
		char *e;
		char *p = "foo";
		char *s = "foobar";
		if ((e = (char *)regcmp(p, 0)) == 0
		 || regex(e, s, 0) == 0)
		 	${cf_cv_main_return:-return}(1);
		free(e);
		${cf_cv_main_return:-return}(0);
}
	],
	[cf_cv_REGCMP_func=yes],
	[cf_cv_REGCMP_func=no],
	[cf_cv_REGCMP_func=unknown])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_REGEXPR_H_FUNCS version: 8 updated: 2023/01/05 18:54:47
dnl ------------------
dnl Tests for the <regexpr.h> include-file, and the functions associated with it.
AC_DEFUN([CF_REGEXPR_H_FUNCS],
[
cf_save_libs="$LIBS"
AC_CHECK_LIB(gen, compile)
AC_CACHE_CHECK(compile/step functions, cf_cv_REGEXPR_H,[
	AC_TRY_RUN([
$ac_includes_default

#include <regexpr.h>

int main(void)
{
		char *e;
		char *p = "foo";
		char *s = "foobar";
		if ((e = (char *)compile(p, (char *)0, (char *)0)) == 0
		 || step(s, e) == 0)
			${cf_cv_main_return:-return}(1);
		free(e);
		${cf_cv_main_return:-return}(0);
}
	],
	[cf_cv_REGEXPR_H=yes],
	[cf_cv_REGEXPR_H=no],
	[cf_cv_REGEXPR_H=unknown])
])
if test "$cf_cv_REGEXPR_H" = yes; then
	AC_DEFINE(HAVE_REGEXPR_H_FUNCS,1,[Define to 1 if we have regexpr functions])
else
	LIBS="$cf_save_libs"
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_REGEX_H_FUNCS version: 8 updated: 2023/01/05 18:54:42
dnl ----------------
dnl Tests for the <regex.h> include-file, and the functions associated with it.
AC_DEFUN([CF_REGEX_H_FUNCS],
[
AC_CHECK_LIB(regex,regcomp,[LIBS="-lregex $LIBS"])
AC_CACHE_CHECK(regcomp/regexec functions, cf_cv_REGEX_H,[
	AC_TRY_RUN([
$ac_includes_default

#include <regex.h>

int main(void)
{
		regex_t e;
		char *p = "foo";
		char *s = "foobar";
		if (regcomp(&e, p, 0) != 0
		 || regexec(&e, s, 0, (regmatch_t*)0, 0) < 0)
			${cf_cv_main_return:-return}(1);
		regfree(&e);
		${cf_cv_main_return:-return}(0);
}
	],
	[cf_cv_REGEX_H=yes],
	[cf_cv_REGEX_H=no],
	[cf_cv_REGEX_H=unknown])
])
test "$cf_cv_REGEX_H" = yes && AC_DEFINE(HAVE_REGEX_H_FUNCS,1,[Define to 1 if we have regcomp/regexec functions])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_REMOVE_CFLAGS version: 3 updated: 2021/09/05 17:25:40
dnl ----------------
dnl Remove a given option from CFLAGS/CPPFLAGS
dnl $1 = option to remove
dnl $2 = variable to update
dnl $3 = nonempty to allow verbose message
define([CF_REMOVE_CFLAGS],
[
cf_tmp_cflag=`echo "x$1" | sed -e 's/^.//' -e 's/=.*//'`
while true
do
	cf_old_cflag=`echo "x[$]$2" | sed -e 's/^.//' -e 's/[[ 	]][[ 	]]*-/ -/g' -e "s%$cf_tmp_cflag\\(=[[^ 	]][[^ 	]]*\\)\?%%" -e 's/^[[ 	]]*//' -e 's%[[ ]][[ ]]*-D% -D%g' -e 's%[[ ]][[ ]]*-I% -I%g'`
	test "[$]$2" != "$cf_old_cflag" || break
	ifelse([$3],,,[CF_VERBOSE(removing old option $1 from $2)])
	$2="$cf_old_cflag"
done
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_REMOVE_DEFINE version: 3 updated: 2010/01/09 11:05:50
dnl ----------------
dnl Remove all -U and -D options that refer to the given symbol from a list
dnl of C compiler options.  This works around the problem that not all
dnl compilers process -U and -D options from left-to-right, so a -U option
dnl cannot be used to cancel the effect of a preceding -D option.
dnl
dnl $1 = target (which could be the same as the source variable)
dnl $2 = source (including '$')
dnl $3 = symbol to remove
define([CF_REMOVE_DEFINE],
[
$1=`echo "$2" | \
	sed	-e 's/-[[UD]]'"$3"'\(=[[^ 	]]*\)\?[[ 	]]/ /g' \
		-e 's/-[[UD]]'"$3"'\(=[[^ 	]]*\)\?[$]//g'`
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_REQUIRE_PKG version: 1 updated: 2025/01/10 19:55:54
dnl --------------
dnl Update $REQUIRE_PKG, which lists the known required packages for this
dnl program.
dnl
dnl $1 = package(s) to require, e.g., in the generated ".pc" file
define([CF_REQUIRE_PKG],
[
for cf_required in $1
do
	# check for duplicates
	for cf_require_pkg in $REQUIRE_PKG
	do
		if test "$cf_required" = "$cf_require_pkg"
		then
			cf_required=
			break
		fi
	done
	test -n "$cf_required" && REQUIRE_PKG="$REQUIRE_PKG $cf_required"
done
AC_SUBST(REQUIRE_PKG)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_RE_COMP_FUNCS version: 8 updated: 2023/01/11 04:05:23
dnl ----------------
dnl Tests for the presence of re_comp/re_exec functions (no include-file?)
AC_DEFUN([CF_RE_COMP_FUNCS],
[
AC_MSG_CHECKING(re_comp/re_exec functions)
AC_CACHE_VAL(cf_cv_RE_COMP_func,[
	AC_TRY_RUN([
extern char *re_comp(char *);
extern int re_exec(char *);
int main(void)
{
		char *e;
		char *p = "foo";
		char *s = "foobar";
		if ((e = (char *)re_comp(p)) != 0
		 || re_exec(s) <= 0)
		 	${cf_cv_main_return:-return}(1);
		${cf_cv_main_return:-return}(0);
}
	],
	[cf_cv_RE_COMP_func=yes],
	[cf_cv_RE_COMP_func=no],
	[cf_cv_RE_COMP_func=unknown])
	])
AC_MSG_RESULT($cf_cv_RE_COMP_func)
test "$cf_cv_RE_COMP_func" = yes && AC_DEFINE(HAVE_RE_COMP_FUNCS,1,[Define to 1 if we have re_comp/re_exec functions])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_SIZE_T version: 6 updated: 2021/01/10 18:23:00
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
test "$cf_cv_type_size_t" = no && AC_DEFINE(size_t, unsigned, [Define to type if size_t not declared])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_SRC_MAKEFILE version: 7 updated: 2021/01/10 18:23:00
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
${AWK:-awk} <$cf_mod >>$cf_out '
BEGIN	{
		found = 0;
	}
	{
		if ( found == 0 )
		{
			printf "\nCSRC="
			found = 1;
		}
		printf " \\\n\t$(srcdir)/%s.c", [$]1
	}
END	{
		print ""
	}
'
	if test "$cf_cv_ar_rules" = yes ; then
${AWK:-awk} <$cf_mod >>$cf_out '
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
${AWK:-awk} <$cf_mod >>$cf_out '
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

	if test "$cf_cv_ar_rules" = yes ; then
cat >>$cf_out <<CF_EOF

\$Z:	\$(OBJS)
	\$(RANLIB) \$Z
CF_EOF
	else
cat >>$cf_out <<CF_EOF

\$Z:	\$(OBJS)
	\$(AR) \$(ARFLAGS) \$Z \$(OBJS)
	\$(RANLIB) \$Z
	@echo "...working around non-POSIX \"\$(MAKE)\"" && sleep 1
CF_EOF
	fi
fi
test -f $1/$2/makefile.end && \
    cat $1/$2/makefile.end >>$2/makefile
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_STAT_ST_BLOCKS version: 5 updated: 2021/01/10 18:23:00
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
test "$cf_cv_st_blocks" = yes && AC_DEFINE(STAT_HAS_ST_BLOCKS,1,[Define to 1 if stat.st_blocks is declared])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_STRERROR version: 2 updated: 2001/07/11 09:34:49
dnl -----------
dnl Check for strerror(), or it is not found, for the related data.  POSIX
dnl requires strerror(), so only old systems such as SunOS lack it.
AC_DEFUN([CF_STRERROR],[
AC_CHECK_FUNCS(strerror, AC_DEFINE(HAVE_STRERROR),[CF_SYS_ERRLIST])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_STRIP_G_OPT version: 4 updated: 2021/01/02 09:31:20
dnl --------------
dnl	Remove "-g" option from the compiler options
AC_DEFUN([CF_STRIP_G_OPT],
[$1=`echo "${$1}" | CF__SED_TRIMBLANKS(-e 's%-g %%' -e 's%-g$%%')`])dnl
dnl ---------------------------------------------------------------------------
dnl CF_STRUCT_SCREEN version: 6 updated: 2021/01/10 18:23:00
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
#include <${cf_cv_ncurses_header:-curses.h}>],
		[struct screen dummy],
		[cf_cv_have_struct_screen=yes],
		[cf_cv_have_struct_screen=no])
	])
AC_MSG_RESULT($cf_cv_have_struct_screen)

AC_MSG_CHECKING(for definition of struct screen)
AC_CACHE_VAL(cf_cv_need_struct_screen,[
	if test "$cf_cv_have_struct_screen" = yes; then
		AC_TRY_COMPILE([#include <${cf_cv_ncurses_header:-curses.h}>],
			[struct screen { int dummy;}],
			[cf_cv_need_struct_screen=yes],
			[cf_cv_need_struct_screen=no])
	else
		cf_cv_need_struct_screen=no
	fi
	])
AC_MSG_RESULT($cf_cv_need_struct_screen)
test "$cf_cv_need_struct_screen" = yes && AC_DEFINE(NEED_STRUCT_SCREEN,1,[Define to 1 if struct SCREEN is declared])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_SUBDIR_PATH version: 7 updated: 2014/12/04 04:33:06
dnl --------------
dnl Construct a search-list for a nonstandard header/lib-file
dnl	$1 = the variable to return as result
dnl	$2 = the package name
dnl	$3 = the subdirectory, e.g., bin, include or lib
AC_DEFUN([CF_SUBDIR_PATH],
[
$1=

CF_ADD_SUBDIR_PATH($1,$2,$3,$prefix,NONE)

for cf_subdir_prefix in \
	/usr \
	/usr/local \
	/usr/pkg \
	/opt \
	/opt/local \
	[$]HOME
do
	CF_ADD_SUBDIR_PATH($1,$2,$3,$cf_subdir_prefix,$prefix)
done
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
dnl CF_TCAP_CURSOR version: 6 updated: 2021/01/10 18:23:00
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
	AC_TRY_COMPILE([#include <${cf_cv_ncurses_header:-curses.h}>],
		[char *d=KD, *u=KU, *r=KR, *l=KL],
		[cf_cv_termcap_cursor=yes],
		[cf_cv_termcap_cursor=no])])
AC_MSG_RESULT($cf_cv_termcap_cursor)
test "$cf_cv_termcap_cursor" = yes && AC_DEFINE(HAVE_TCAP_CURSOR,1,[Define to 1 if termcap cursor-variables are declared])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_TD_SIG_ARGS version: 9 updated: 2024/04/21 16:11:29
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
$ac_includes_default
#include <signal.h>
		RETSIGTYPE (*signal(int sig, RETSIGTYPE(*func)(int sig)))(int sig2);
		RETSIGTYPE catch(int sig) { exit(0); }
		int main(void) { signal(SIGINT, catch); ${cf_cv_main_return:-return}(0); } ],
		[cf_cv_sig_args=STANDARD],
		[AC_TRY_RUN([
$ac_includes_default
#include <signal.h>
			RETSIGTYPE (*signal(int sig, RETSIGTYPE(*func)(int sig,...)))(int sig2,...);
			RETSIGTYPE catch(int sig, ...) { exit(0); }
			int main(void) { signal(SIGINT, catch); ${cf_cv_main_return:-return}(0); } ],
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
		if test "$cf_cv_sig_args" != UNKNOWN; then	# we have prototypes
			AC_TRY_RUN([
#include <signal.h>
#undef  NOT
#define NOT(s,d) ((long)(s) != (long)(d))
				int main(void) { ${cf_cv_main_return:-return}(NOT(SIG_IGN,1) || NOT(SIG_DFL,0) || NOT(SIG_ERR,-1)); } ],
				[cf_cv_sigs_redef=yes],
				[cf_cv_sigs_redef=no],
				[cf_cv_sigs_redef=unknown])
		fi
		])
		AC_MSG_RESULT($cf_cv_sigs_redef)
		test "$cf_cv_sigs_redef" = yes && AC_DEFINE(SIG_IGN_REDEFINEABLE,1,[Define to 1 if SIG_IGN is redefinable])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_TD_SRC_MAKEFILES version: 5 updated: 2020/12/06 15:33:41
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
	CF_SRC_MAKEFILE($srcdir,src/$cf_src,\$(top_srcdir)/src/td_rules.mk)
	echo "	cd $cf_src &&	\$(MAKE) \$(MAKE_OPTS) \[$]@" >>src/makefile
done
test -f $srcdir/src/makefile.end && \
    cat $srcdir/src/makefile.end >>src/makefile
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_TD_SRC_MODULES version: 4 updated: 2021/01/10 18:23:00
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
	if test -d "$p" ; then
		if test -f "$p/modules" ; then
			p=`basename "$p"`
			cf_cv_src_modules="$cf_cv_src_modules $p"
			cf_cv_src_makefiles="$cf_cv_src_makefiles src/$p/makefile:$srcdir/src/sub_vars.in"
		fi
	fi
done
])
AC_MSG_RESULT($cf_cv_src_modules)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_TD_TEST_MAKEFILES version: 5 updated: 2020/12/06 16:26:55
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
${AWK:-awk} <$q >>$cf_out '
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
${AWK:-awk} <$q >>$cf_out '
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
${AWK:-awk} <$q >>$cf_out '
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


${make_include_left}\$(top_srcdir)/test/td_rules.mk${make_include_right}

# Remind some make-programs that they found these using VPATH:
\$(REF_FILES) :
\$(SCRIPTS) :

# Fix for SunOS VPATH
CF_EOF
${AWK:-awk} <$q >>$cf_out '
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
dnl CF_TD_TEST_MODULES version: 3 updated: 2021/01/10 18:23:00
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
	if test -d "$p" ; then
		if test -f "$p/modules" ; then
			p=`basename "$p"`
			cf_cv_test_modules="$cf_cv_test_modules $p"
			cf_cv_test_makefiles="$cf_cv_test_makefiles test/$p/makefile:$srcdir/test/sub_vars.in"
		fi
	fi
done
])
AC_MSG_RESULT($cf_cv_test_modules)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_TERM_HEADER version: 6 updated: 2021/01/02 09:31:20
dnl --------------
dnl Look for term.h, which is part of X/Open curses.  It defines the interface
dnl to terminfo database.  Usually it is in the same include-path as curses.h,
dnl but some packagers change this, breaking various applications.
AC_DEFUN([CF_TERM_HEADER],[
AC_CACHE_CHECK(for terminfo header, cf_cv_term_header,[
case "${cf_cv_ncurses_header}" in
(*/ncurses.h|*/ncursesw.h)
	cf_term_header=`echo "$cf_cv_ncurses_header" | sed -e 's%ncurses[[^.]]*\.h$%term.h%'`
	;;
(*)
	cf_term_header=term.h
	;;
esac

for cf_test in $cf_term_header "ncurses/term.h" "ncursesw/term.h"
do
AC_TRY_COMPILE([#include <stdio.h>
#include <${cf_cv_ncurses_header:-curses.h}>
#include <$cf_test>
],[int x = auto_left_margin; (void)x],[
	cf_cv_term_header="$cf_test"],[
	cf_cv_term_header=unknown
	])
	test "$cf_cv_term_header" != unknown && break
done
])

# Set definitions to allow ifdef'ing to accommodate subdirectories

case "$cf_cv_term_header" in
(*term.h)
	AC_DEFINE(HAVE_TERM_H,1,[Define to 1 if we have term.h])
	;;
esac

case "$cf_cv_term_header" in
(ncurses/term.h)
	AC_DEFINE(HAVE_NCURSES_TERM_H,1,[Define to 1 if we have ncurses/term.h])
	;;
(ncursesw/term.h)
	AC_DEFINE(HAVE_NCURSESW_TERM_H,1,[Define to 1 if we have ncursesw/term.h])
	;;
esac
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_TOP_SRCDIR version: 4 updated: 2021/01/10 18:23:00
dnl -------------
dnl Make an absolute symbol for the top of the configuration.
AC_DEFUN([CF_TOP_SRCDIR],
[
CDPATH=; export CDPATH
TOP_SRCDIR=`cd "$srcdir";pwd`
AC_SUBST(TOP_SRCDIR)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_TRIM_LDFLAGS version: 1 updated: 2019/12/26 19:50:12
dnl ---------------
dnl When building a package containing a development library, some of the
dnl packager's LDFLAGS are useful; others cause problems.  (Try to) filter
dnl out the common nuisances.
AC_DEFUN([CF_TRIM_LDFLAGS],
[
TRIMMED_LDFLAGS=
cf_save=
for cf_opt in $LDFLAGS
do
	case "x$cf_opt" in
	x-R)
		TRIMMED_LDFLAGS="$TRIMMED_LDFLAGS $cf_opt"
		cf_save=$cf_opt
		;;
	x-l*|x-L*|x-R*)
		TRIMMED_LDFLAGS="$TRIMMED_LDFLAGS $cf_opt"
		cf_save=
		;;
	*)
		test -n "$cf_save" && TRIMMED_LDFLAGS="$TRIMMED_LDFLAGS $cf_opt"
		cf_save=
		;;
	esac
done
AC_SUBST(TRIMMED_LDFLAGS)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_TRIM_X_LIBS version: 3 updated: 2015/04/12 15:39:00
dnl --------------
dnl Trim extra base X libraries added as a workaround for inconsistent library
dnl dependencies returned by "new" pkg-config files.
AC_DEFUN([CF_TRIM_X_LIBS],[
	for cf_trim_lib in Xmu Xt X11
	do
		case "$LIBS" in
		(*-l$cf_trim_lib\ *-l$cf_trim_lib*)
			LIBS=`echo "$LIBS " | sed -e 's/  / /g' -e 's%-l'"$cf_trim_lib"' %%' -e 's/ $//'`
			CF_VERBOSE(..trimmed $LIBS)
			;;
		esac
	done
])
dnl ---------------------------------------------------------------------------
dnl CF_TRY_PKG_CONFIG version: 7 updated: 2025/01/10 19:55:54
dnl -----------------
dnl This is a simple wrapper to use for pkg-config, for libraries which may be
dnl available in that form.
dnl
dnl $1 = package name, which may be a shell variable
dnl $2 = extra logic to use, if any, after updating CFLAGS and LIBS
dnl $3 = logic to use if pkg-config does not have the package
AC_DEFUN([CF_TRY_PKG_CONFIG],[
AC_REQUIRE([CF_PKG_CONFIG])

if test "$PKG_CONFIG" != none && "$PKG_CONFIG" --exists "$1"; then
	CF_VERBOSE(found package $1)
	cf_pkgconfig_incs="`$PKG_CONFIG --cflags "$1" 2>/dev/null`"
	cf_pkgconfig_libs="`$PKG_CONFIG --libs   "$1" 2>/dev/null`"
	CF_VERBOSE(package $1 CFLAGS: $cf_pkgconfig_incs)
	CF_VERBOSE(package $1 LIBS: $cf_pkgconfig_libs)
	CF_REQUIRE_PKG($1)
	CF_ADD_CFLAGS($cf_pkgconfig_incs)
	CF_ADD_LIBS($cf_pkgconfig_libs)
	ifelse([$2],,:,[$2])
else
	cf_pkgconfig_incs=
	cf_pkgconfig_libs=
	ifelse([$3],,:,[$3])
fi
])
dnl ---------------------------------------------------------------------------
dnl CF_TRY_XOPEN_SOURCE version: 4 updated: 2022/09/10 15:16:16
dnl -------------------
dnl If _XOPEN_SOURCE is not defined in the compile environment, check if we
dnl can define it successfully.
AC_DEFUN([CF_TRY_XOPEN_SOURCE],[
AC_CACHE_CHECK(if we should define _XOPEN_SOURCE,cf_cv_xopen_source,[
	AC_TRY_COMPILE(CF__XOPEN_SOURCE_HEAD,CF__XOPEN_SOURCE_BODY,
	[cf_cv_xopen_source=no],
	[cf_save="$CPPFLAGS"
	 CF_APPEND_TEXT(CPPFLAGS,-D_XOPEN_SOURCE=$cf_XOPEN_SOURCE)
	 AC_TRY_COMPILE(CF__XOPEN_SOURCE_HEAD,CF__XOPEN_SOURCE_BODY,
		[cf_cv_xopen_source=no],
		[cf_cv_xopen_source=$cf_XOPEN_SOURCE])
		CPPFLAGS="$cf_save"
	])
])

if test "$cf_cv_xopen_source" != no ; then
	CF_REMOVE_DEFINE(CFLAGS,$CFLAGS,_XOPEN_SOURCE)
	CF_REMOVE_DEFINE(CPPFLAGS,$CPPFLAGS,_XOPEN_SOURCE)
	cf_temp_xopen_source="-D_XOPEN_SOURCE=$cf_cv_xopen_source"
	CF_APPEND_CFLAGS($cf_temp_xopen_source)
fi
])
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
dnl CF_UTF8_LIB version: 11 updated: 2024/08/10 10:23:45
dnl -----------
dnl Check for multibyte support, and if not found, utf8 compatibility library
AC_DEFUN([CF_UTF8_LIB],
[
AC_CHECK_HEADERS(wchar.h)
AC_CACHE_CHECK(for multibyte character support,cf_cv_utf8_lib,[
	cf_save_LIBS="$LIBS"
	AC_TRY_LINK([
$ac_includes_default
#ifdef HAVE_WCHAR_H
#include <wchar.h>
#endif
],[putwc(0,0);],
	[cf_cv_utf8_lib=yes],
	[CF_FIND_LINKAGE([
#include <libutf8.h>],[putwc(0,0);],utf8,
		[cf_cv_utf8_lib=add-on],
		[cf_cv_utf8_lib=no])
])])

# HAVE_LIBUTF8_H is used by ncurses if curses.h is shared between
# ncurses/ncursesw:
if test "$cf_cv_utf8_lib" = "add-on" ; then
	AC_DEFINE(HAVE_LIBUTF8_H,1,[Define to 1 if we should include libutf8.h])
	CF_ADD_INCDIR($cf_cv_header_path_utf8)
	CF_ADD_LIBDIR($cf_cv_library_path_utf8)
	CF_ADD_LIBS($cf_cv_library_file_utf8)
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_VARARGS version: 5 updated: 2021/01/10 18:23:00
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
test "$cf_cv_ansi_varargs" = yes && AC_DEFINE(ANSI_VARARGS,1,[Define to 1 if C uses ANSI varargs])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_VERBOSE version: 3 updated: 2007/07/29 09:55:12
dnl ----------
dnl Use AC_VERBOSE w/o the warnings
AC_DEFUN([CF_VERBOSE],
[test -n "$verbose" && echo "	$1" 1>&AC_FD_MSG
CF_MSG_LOG([$1])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_VERSION_INFO version: 10 updated: 2025/01/10 19:55:54
dnl ---------------
dnl Define several useful symbols derived from the VERSION file.  A separate
dnl file is preferred to embedding the version numbers in various scripts.
dnl (automake is a textbook-example of why the latter is a bad idea, but there
dnl are others).
dnl
dnl The file contents are:
dnl	libtool-version	release-version	patch-version
dnl or
dnl	release-version
dnl where
dnl	libtool-version (see ?) consists of 3 integers separated by '.'
dnl	release-version consists of a major version and minor version
dnl		separated by '.', optionally followed by a patch-version
dnl		separated by '-'.  The minor version need not be an
dnl		integer (but it is preferred).
dnl	patch-version is an integer in the form yyyymmdd, so ifdef's and
dnl		scripts can easily compare versions.
dnl
dnl If libtool is used, the first form is required, since CF_WITH_LIBTOOL
dnl simply extracts the first field using 'cut -f1'.
dnl
dnl Optional parameters:
dnl $1 = internal name for package
dnl $2 = external name for package
AC_DEFUN([CF_VERSION_INFO],
[
if test -f "$srcdir/VERSION" ; then
	AC_MSG_CHECKING(for package version)

	# if there are not enough fields, cut returns the last one...
	cf_field1=`sed -e '2,$d' "$srcdir/VERSION" |cut -f1`
	cf_field2=`sed -e '2,$d' "$srcdir/VERSION" |cut -f2`
	cf_field3=`sed -e '2,$d' "$srcdir/VERSION" |cut -f3`

	# this is how CF_BUNDLED_INTL uses $VERSION:
	VERSION="$cf_field1"

	VERSION_MAJOR=`echo "$cf_field2" | sed -e 's/\..*//'`
	test -z "$VERSION_MAJOR" && AC_MSG_ERROR(missing major-version)

	VERSION_MINOR=`echo "$cf_field2" | sed -e 's/^[[^.]]*\.//' -e 's/-.*//'`
	test -z "$VERSION_MINOR" && AC_MSG_ERROR(missing minor-version)

	AC_MSG_RESULT(${VERSION_MAJOR}.${VERSION_MINOR})

	AC_MSG_CHECKING(for package patch date)
	VERSION_PATCH=`echo "$cf_field3" | sed -e 's/^[[^-]]*-//'`
	case .$VERSION_PATCH in
	(.)
		AC_MSG_ERROR(missing patch-date $VERSION_PATCH)
		;;
	(.[[0-9]][[0-9]][[0-9]][[0-9]][[0-9]][[0-9]][[0-9]][[0-9]])
		;;
	(*)
		AC_MSG_ERROR(illegal patch-date $VERSION_PATCH)
		;;
	esac
	AC_MSG_RESULT($VERSION_PATCH)
else
	AC_MSG_ERROR(did not find $srcdir/VERSION)
fi

# show the actual data that we have for versions:
CF_VERBOSE(ABI VERSION $VERSION)
CF_VERBOSE(VERSION_MAJOR $VERSION_MAJOR)
CF_VERBOSE(VERSION_MINOR $VERSION_MINOR)
CF_VERBOSE(VERSION_PATCH $VERSION_PATCH)

AC_SUBST(VERSION)
AC_SUBST(VERSION_MAJOR)
AC_SUBST(VERSION_MINOR)
AC_SUBST(VERSION_PATCH)

dnl if a package name is given, define its corresponding version info.  We
dnl need the package name to ensure that the defined symbols are unique.
ifelse($1,,,[
	PROGRAM=$1
	PACKAGE=ifelse($2,,$1,$2)
	AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE",[Define to the package-name])
	AC_SUBST(PROGRAM)
	AC_SUBST(PACKAGE)
	AH_TEMPLATE([AS_TR_CPP($1[_VERSION])],[version of package])
	AC_DEFINE_UNQUOTED(AS_TR_CPP($1[_VERSION]),"${VERSION_MAJOR}.${VERSION_MINOR}")
	AH_TEMPLATE([AS_TR_CPP($1[_PATCHDATE])],[patchdate of package])
	AC_DEFINE_UNQUOTED(AS_TR_CPP($1[_PATCHDATE]),${VERSION_PATCH})
])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WAIT version: 4 updated: 2021/01/10 18:23:00
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
if test "$ac_cv_header_sys_wait_h" = yes; then
cf_decl="$cf_decl
#include <sys/wait.h>
"
elif test "$ac_cv_header_wait_h" = yes; then
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

if test "$cf_cv_decl_union_wait" = yes; then
	AC_MSG_CHECKING(union wait used as wait-arg)
	AC_CACHE_VAL(cf_cv_arg_union_wait,[
		AC_TRY_COMPILE($cf_decl,
 			[union wait x; wait(&x)],
			[cf_cv_arg_union_wait=yes],
			[cf_cv_arg_union_wait=no])
		])
	AC_MSG_RESULT($cf_cv_arg_union_wait)
	test "$cf_cv_arg_union_wait" = yes && AC_DEFINE(WAIT_USES_UNION,1,[Define to 1 if wait() uses union])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITHOUT_X version: 3 updated: 2021/01/13 16:51:52
dnl ------------
dnl Use this to cancel the check for X headers/libraries which would be pulled
dnl in via CF_GCC_WARNINGS.
define([CF_WITHOUT_X],
AC_DEFUN([AC_PATH_XTRA],[])
AC_DEFUN([CF_SAVE_XTRA_FLAGS],[])
AC_DEFUN([CF_RESTORE_XTRA_FLAGS],[])
AC_DEFUN([CF_CONST_X_STRING],[echo "skipping X-const check";])dnl
AC_SUBST(X_CFLAGS)
AC_SUBST(X_LIBS)
[])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_CURSES_DIR version: 4 updated: 2021/01/02 19:22:58
dnl ------------------
dnl Wrapper for AC_ARG_WITH to specify directory under which to look for curses
dnl libraries.
AC_DEFUN([CF_WITH_CURSES_DIR],[

AC_MSG_CHECKING(for specific curses-directory)
AC_ARG_WITH(curses-dir,
	[  --with-curses-dir=DIR   directory in which (n)curses is installed],
	[cf_cv_curses_dir=$withval],
	[cf_cv_curses_dir=no])
AC_MSG_RESULT($cf_cv_curses_dir)

if test -n "$cf_cv_curses_dir" && test "$cf_cv_curses_dir" != "no"
then
	CF_PATH_SYNTAX(withval)
	if test -d "$cf_cv_curses_dir"
	then
		CF_ADD_INCDIR($cf_cv_curses_dir/include)
		CF_ADD_LIBDIR($cf_cv_curses_dir/lib)
	fi
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_DBMALLOC version: 7 updated: 2010/06/21 17:26:47
dnl ----------------
dnl Configure-option for dbmalloc.  The optional parameter is used to override
dnl the updating of $LIBS, e.g., to avoid conflict with subsequent tests.
AC_DEFUN([CF_WITH_DBMALLOC],[
CF_NO_LEAKS_OPTION(dbmalloc,
	[  --with-dbmalloc         test: use Conor Cahill's dbmalloc library],
	[USE_DBMALLOC])

if test "$with_dbmalloc" = yes ; then
	AC_CHECK_HEADER(dbmalloc.h,
		[AC_CHECK_LIB(dbmalloc,[debug_malloc]ifelse([$1],,[],[,$1]))])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_DMALLOC version: 7 updated: 2010/06/21 17:26:47
dnl ---------------
dnl Configure-option for dmalloc.  The optional parameter is used to override
dnl the updating of $LIBS, e.g., to avoid conflict with subsequent tests.
AC_DEFUN([CF_WITH_DMALLOC],[
CF_NO_LEAKS_OPTION(dmalloc,
	[  --with-dmalloc          test: use Gray Watson's dmalloc library],
	[USE_DMALLOC])

if test "$with_dmalloc" = yes ; then
	AC_CHECK_HEADER(dmalloc.h,
		[AC_CHECK_LIB(dmalloc,[dmalloc_debug]ifelse([$1],,[],[,$1]))])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_MAN2HTML version: 14 updated: 2024/09/09 17:17:46
dnl ----------------
dnl Check for man2html and groff.  Prefer man2html over groff, but use groff
dnl as a fallback.  See
dnl
dnl		https://invisible-island.net/scripts/man2html.html
dnl
dnl Generate a shell script which hides the differences between the two.
dnl
dnl We name that "man2html.tmp".
dnl
dnl The shell script can be removed later, e.g., using "make distclean".
AC_DEFUN([CF_WITH_MAN2HTML],[
AC_REQUIRE([CF_PROG_GROFF])dnl
AC_REQUIRE([AC_PROG_FGREP])dnl

case "x${with_man2html}" in
(xno)
	cf_man2html=no
	;;
(x|xyes)
	AC_PATH_PROG(cf_man2html,man2html,no)
	case "x$cf_man2html" in
	(x/*)
		AC_MSG_CHECKING(for the modified Earl Hood script)
		if ( $cf_man2html -help 2>&1 | grep 'Make an index of headers at the end' >/dev/null )
		then
			cf_man2html_ok=yes
		else
			cf_man2html=no
			cf_man2html_ok=no
		fi
		AC_MSG_RESULT($cf_man2html_ok)
		;;
	(*)
		cf_man2html=no
		;;
	esac
esac

AC_MSG_CHECKING(for program to convert manpage to html)
AC_ARG_WITH(man2html,
	[[  --with-man2html[=XXX]   use XXX rather than groff]],
	[cf_man2html=$withval],
	[cf_man2html=$cf_man2html])

cf_with_groff=no

case $cf_man2html in
(yes)
	AC_MSG_RESULT(man2html)
	AC_PATH_PROG(cf_man2html,man2html,no)
	;;
(no|groff|*/groff*)
	cf_with_groff=yes
	cf_man2html=$GROFF_PATH
	AC_MSG_RESULT($cf_man2html)
	;;
(*)
	AC_MSG_RESULT($cf_man2html)
	;;
esac

MAN2HTML_TEMP="man2html.tmp"
	cat >$MAN2HTML_TEMP <<CF_EOF
#!$SHELL
# Temporary script generated by CF_WITH_MAN2HTML
# Convert inputs to html, sending result to standard output.
#
# Parameters:
# \${1} = rootname of file to convert
# \${2} = suffix of file to convert, e.g., "1"
# \${3} = macros to use, e.g., "man"
#
ROOT=\[$]1
TYPE=\[$]2
MACS=\[$]3

unset LANG
unset LC_ALL
unset LC_CTYPE
unset LANGUAGE
GROFF_NO_SGR=stupid
export GROFF_NO_SGR

CF_EOF

NROFF_OPTS=
if test "x$cf_with_groff" = xyes
then
	MAN2HTML_NOTE="$GROFF_NOTE"
	MAN2HTML_PATH="$GROFF_PATH"
	cat >>$MAN2HTML_TEMP <<CF_EOF
$SHELL -c "$TBL_PATH \${ROOT}.\${TYPE} | $GROFF_PATH -P -o0 -I\${ROOT}_ -Thtml -\${MACS}"
CF_EOF
else
	# disable hyphenation if this is groff
	if test "x$GROFF_PATH" != xno
	then
		AC_MSG_CHECKING(if nroff is really groff)
		cf_check_groff="`$NROFF_PATH --version 2>/dev/null | grep groff`"
		test -n "$cf_check_groff" && cf_check_groff=yes
		test -n "$cf_check_groff" || cf_check_groff=no
		AC_MSG_RESULT($cf_check_groff)
		test "x$cf_check_groff" = xyes && NROFF_OPTS="-rHY=0"
	fi
	MAN2HTML_NOTE=""
	CF_PATH_SYNTAX(cf_man2html)
	MAN2HTML_PATH="$cf_man2html"
	AC_MSG_CHECKING(for $cf_man2html top/bottom margins)

	# for this example, expect 3 lines of content, the remainder is head/foot
	cat >conftest.in <<CF_EOF
.TH HEAD1 HEAD2 HEAD3 HEAD4 HEAD5
.SH SECTION
MARKER
CF_EOF

	LC_ALL=C LC_CTYPE=C LANG=C LANGUAGE=C $NROFF_PATH -man conftest.in >conftest.out

	cf_man2html_1st="`${FGREP-fgrep} -n MARKER conftest.out |sed -e 's/^[[^0-9]]*://' -e 's/:.*//'`"
	cf_man2html_top=`expr "$cf_man2html_1st" - 2`
	cf_man2html_bot="`wc -l conftest.out |sed -e 's/[[^0-9]]//g'`"
	cf_man2html_bot=`expr "$cf_man2html_bot" - 2 - "$cf_man2html_top"`
	cf_man2html_top_bot="-topm=$cf_man2html_top -botm=$cf_man2html_bot"

	AC_MSG_RESULT($cf_man2html_top_bot)

	AC_MSG_CHECKING(for pagesize to use)
	for cf_block in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
	do
	cat >>conftest.in <<CF_EOF
.nf
0
1
2
3
4
5
6
7
8
9
CF_EOF
	done

	LC_ALL=C LC_CTYPE=C LANG=C LANGUAGE=C $NROFF_PATH -man conftest.in >conftest.out
	cf_man2html_page="`${FGREP-fgrep} -n HEAD1 conftest.out |sed -n '$p' |sed -e 's/^[[^0-9]]*://' -e 's/:.*//'`"
	test -z "$cf_man2html_page" && cf_man2html_page=99999
	test "$cf_man2html_page" -gt 100 && cf_man2html_page=99999

	rm -rf conftest*
	AC_MSG_RESULT($cf_man2html_page)

	cat >>$MAN2HTML_TEMP <<CF_EOF
: \${MAN2HTML_PATH=$MAN2HTML_PATH}
MAN2HTML_OPTS="\$MAN2HTML_OPTS -index -title=\"\$ROOT(\$TYPE)\" -compress -pgsize $cf_man2html_page"
case \${TYPE} in
(ms)
	$TBL_PATH \${ROOT}.\${TYPE} | $NROFF_PATH $NROFF_OPTS -\${MACS} | \$MAN2HTML_PATH -topm=0 -botm=0 \$MAN2HTML_OPTS
	;;
(*)
	$TBL_PATH \${ROOT}.\${TYPE} | $NROFF_PATH $NROFF_OPTS -\${MACS} | \$MAN2HTML_PATH $cf_man2html_top_bot \$MAN2HTML_OPTS
	;;
esac
CF_EOF
fi

chmod 700 $MAN2HTML_TEMP

AC_SUBST(MAN2HTML_NOTE)
AC_SUBST(MAN2HTML_PATH)
AC_SUBST(MAN2HTML_TEMP)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_NCURSES_ETC version: 7 updated: 2025/01/12 13:41:15
dnl -------------------
dnl Use this macro for programs which use any variant of "curses", e.g.,
dnl "ncurses", and "PDCurses".  Programs that can use curses and some unrelated
dnl library (such as slang) should use a "--with-screen=XXX" option.
dnl
dnl This does not use AC_DEFUN, because that would tell autoconf to run each
dnl of the macros inside this one - before this macro.
define([CF_WITH_NCURSES_ETC],[
CF_WITH_CURSES_DIR

cf_cv_screen=curses

AC_MSG_CHECKING(for specified curses library type)
AC_ARG_WITH(screen,
	[  --with-screen=XXX       use specified curses-libraries],
	[cf_cv_screen=$withval],[

AC_ARG_WITH(ncursesw,
	[  --with-ncursesw         use wide ncurses-libraries],
	[cf_cv_screen=ncursesw],[

AC_ARG_WITH(ncurses,
	[  --with-ncurses          use ncurses-libraries],
	[cf_cv_screen=ncurses],[

AC_ARG_WITH(pdcurses,
	[  --with-pdcurses         compile/link with pdcurses X11 library],
	[cf_cv_screen=pdcurses],[

AC_ARG_WITH(curses-colr,
	[  --with-curses-colr      compile/link with HPUX 10.x color-curses],
	[cf_cv_screen=curses_colr],[

AC_ARG_WITH(curses-5lib,
	[  --with-curses-5lib      compile/link with SunOS 5lib curses],
	[cf_cv_screen=curses_5lib])])])])])])

AC_MSG_RESULT($cf_cv_screen)

case $cf_cv_screen in
(curses|curses_*)
	CF_CURSES_CONFIG
	;;
(ncursesw*)
	CF_UTF8_LIB
	CF_NCURSES_CONFIG($cf_cv_screen)
	;;
(ncurses*)
	CF_NCURSES_CONFIG($cf_cv_screen)
	;;
(pdcurses*)
	CF_PDCURSES_X11
	;;
(*)
	AC_MSG_ERROR(unexpected screen-value: $cf_cv_screen)
	;;
esac

CF_NCURSES_PTHREADS($cf_cv_screen)
AC_SUBST(cf_cv_screen)

])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_PKG_CONFIG_LIBDIR version: 25 updated: 2024/08/03 13:34:29
dnl -------------------------
dnl Allow the choice of the pkg-config library directory to be overridden.
dnl
dnl pkg-config uses a search-list built from these colon-separated lists of
dnl directories:
dnl a) $PKG_CONFIG_PATH (tested first, added if set)
dnl b) $PKG_CONFIG_LIBDIR (tested second, added if set)
dnl c) builtin-list (added if $PKG_CONFIG_LIBDIR is not set)
dnl
dnl pkgconf (used with some systems such as FreeBSD in place of pkg-config)
dnl optionally ignores $PKG_CONFIG_LIBDIR.  Very old versions of pkg-config,
dnl e.g., Solaris 10 also do not recognize $PKG_CONFIG_LIBDIR.
AC_DEFUN([CF_WITH_PKG_CONFIG_LIBDIR],[
AC_REQUIRE([CF_GLOB_FULLPATH])dnl

case "$PKG_CONFIG" in
(no|none|yes)
	AC_MSG_CHECKING(for pkg-config library directory)
	;;
(*)
	AC_MSG_CHECKING(for $PKG_CONFIG library directory)
	;;
esac

# if $PKG_CONFIG_LIBDIR is set, try to use that
if test -n "$PKG_CONFIG_PATH"; then
	cf_search_path=`echo "$PKG_CONFIG_PATH" | sed -e 's/:/ /g' -e 's,^[[ 	]]*,,' -e 's,[[ 	]]*$,,'`
elif test -n "$PKG_CONFIG_LIBDIR"; then
	cf_search_path=`echo "$PKG_CONFIG_LIBDIR" | sed -e 's/:/ /g' -e 's,^[[ 	]]*,,' -e 's,[[ 	]]*$,,'`
else
	cf_search_path=auto
fi

# if the option is used, let that override.  otherwise default to "libdir"
AC_ARG_WITH(pkg-config-libdir,
	[[  --with-pkg-config-libdir[=XXX] use given directory for installing pc-files]],
	[cf_search_path=$withval],
	[test "x$PKG_CONFIG" != xnone && test -z "$cf_search_path" && cf_search_path=libdir])

case "x$cf_search_path" in
(xlibdir)
	PKG_CONFIG_LIBDIR='${libdir}/pkgconfig'
	AC_MSG_RESULT($PKG_CONFIG_LIBDIR)
	cf_search_path=
	;;
(x)
	;;
(x$GLOB_FULLPATH_POSIX\ *|x$GLOB_FULLPATH_OTHER\ *)
	PKG_CONFIG_LIBDIR=
	;;
(x$GLOB_FULLPATH_POSIX|x$GLOB_FULLPATH_OTHER)
	PKG_CONFIG_LIBDIR="$cf_search_path"
	AC_MSG_RESULT($PKG_CONFIG_LIBDIR)
	cf_search_path=
	;;
(xyes|xauto)
	AC_MSG_RESULT(auto)
	cf_search_path=
	# Look for the library directory using the same prefix as the executable
	AC_MSG_CHECKING(for search-list)
	if test "x$PKG_CONFIG" != xnone
	then
		# works for pkg-config since version 0.24 (2009)
		# works for pkgconf since version 0.8.3 (2012)
		for cf_pkg_program in \
			`echo "$PKG_CONFIG" | sed -e 's,^.*/,,'` \
			pkg-config \
			pkgconf
		do
			cf_raw_search_path=`"$PKG_CONFIG" --variable=pc_path "$cf_pkg_program" 2>/dev/null`
			case "$cf_raw_search_path" in
			(*\\*)
				cf_search_path=`echo "$cf_raw_search_path" | tr ';' ' ' | tr '\' '/'`
				;;
			(*/*)
				cf_search_path=`echo "$cf_raw_search_path" | tr : ' '`
				;;
			esac
			test -n "$cf_search_path" && break
		done

		# works for pkg-config since import in 2005 of original 2001 HP code.
		test -z "$cf_search_path" && \
		cf_search_path=`
		"$PKG_CONFIG" --debug --exists no-such-package 2>&1 | $AWK "\
/^Scanning directory (#[1-9][0-9]* )?'.*'$/{ \
	sub(\"^[[^']]*'\",\"\"); \
	sub(\"'.*\",\"\"); \
	printf \" %s\", \\[$]0; } \
{ next; } \
"`
	fi

	AC_MSG_RESULT($cf_search_path)
	;;
(*)
	AC_MSG_ERROR(Unexpected option value: $cf_search_path)
	;;
esac

if test -n "$cf_search_path"
then
	AC_MSG_CHECKING(for first directory)
	cf_pkg_config_path=none
	for cf_config in $cf_search_path
	do
		if test -d "$cf_config"
		then
			cf_pkg_config_path=$cf_config
			break
		fi
	done
	AC_MSG_RESULT($cf_pkg_config_path)

	if test "x$cf_pkg_config_path" != xnone ; then
		# limit this to the first directory found
		PKG_CONFIG_LIBDIR="$cf_pkg_config_path"
	fi

	if test -z "$PKG_CONFIG_LIBDIR" && test -n "$cf_search_path"
	then
		AC_MSG_CHECKING(for workaround)
		if test "$prefix" = "NONE" ; then
			cf_prefix="$ac_default_prefix"
		else
			cf_prefix="$prefix"
		fi
		eval cf_libdir=$libdir
		cf_libdir=`echo "$cf_libdir" | sed -e "s,^NONE,$cf_prefix,"`
		cf_backup=
		for cf_config in $cf_search_path
		do
			case $cf_config in
			$cf_libdir/pkgconfig)
				PKG_CONFIG_LIBDIR=$cf_libdir/pkgconfig
				break
				;;
			*)
				test -z "$cf_backup" && cf_backup=$cf_config
				;;
			esac
		done
		test -z "$PKG_CONFIG_LIBDIR" && PKG_CONFIG_LIBDIR=$cf_backup
		AC_MSG_RESULT($PKG_CONFIG_LIBDIR)
	fi
fi

AC_SUBST(PKG_CONFIG_LIBDIR)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_SYSTYPE version: 1 updated: 2013/01/26 16:26:12
dnl ---------------
dnl For testing, override the derived host system-type which is used to decide
dnl things such as the linker commands used to build shared libraries.  This is
dnl normally chosen automatically based on the type of system which you are
dnl building on.  We use it for testing the configure script.
dnl
dnl This is different from the --host option: it is used only for testing parts
dnl of the configure script which would not be reachable with --host since that
dnl relies on the build environment being real, rather than mocked up.
AC_DEFUN([CF_WITH_SYSTYPE],[
CF_CHECK_CACHE([AC_CANONICAL_SYSTEM])
AC_ARG_WITH(system-type,
	[  --with-system-type=XXX  test: override derived host system-type],
[AC_MSG_WARN(overriding system type to $withval)
	cf_cv_system_name=$withval
	host_os=$withval
])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_VALGRIND version: 1 updated: 2006/12/14 18:00:21
dnl ----------------
AC_DEFUN([CF_WITH_VALGRIND],[
CF_NO_LEAKS_OPTION(valgrind,
	[  --with-valgrind         test: use valgrind],
	[USE_VALGRIND])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_XOPEN_CURSES version: 20 updated: 2024/01/07 06:54:12
dnl ---------------
dnl Test if we should define X/Open source for curses, needed on Digital Unix
dnl 4.x, to see the extended functions, but breaks on IRIX 6.x.
dnl
dnl The getbegyx() check is needed for HPUX, which omits legacy macros such
dnl as getbegy().  The latter is better design, but the former is standard.
AC_DEFUN([CF_XOPEN_CURSES],
[
AC_REQUIRE([CF_CURSES_CPPFLAGS])dnl
AC_CACHE_CHECK(definition to turn on extended curses functions,cf_cv_need_xopen_extension,[
cf_cv_need_xopen_extension=unknown
AC_TRY_LINK([
$ac_includes_default
#include <${cf_cv_ncurses_header:-curses.h}>],[
#if defined(NCURSES_VERSION_PATCH)
#if (NCURSES_VERSION_PATCH < 20100501) && (NCURSES_VERSION_PATCH >= 20100403)
	#error disallow ncurses versions between 2020/04/03 and 2010/05/01
#endif
#endif
#ifdef NCURSES_WIDECHAR
#error prefer to fall-through on the second checks
#endif
	static char dummy[10];
	cchar_t check;
	int check2 = curs_set((int)sizeof(check));
	long x = winnstr(stdscr, dummy, 5);
	int x1, y1;
	(void)check2;
	getbegyx(stdscr, y1, x1);
	(void)x;
	(void)y1;
	(void)x1;
	],
	[cf_cv_need_xopen_extension=none],
	[
	for cf_try_xopen_extension in _XOPEN_SOURCE_EXTENDED NCURSES_WIDECHAR
	do
		AC_TRY_LINK([
#define $cf_try_xopen_extension 1
$ac_includes_default
#include <${cf_cv_ncurses_header:-curses.h}>],[
		static char dummy[10];
		cchar_t check;
		int check2 = curs_set((int)sizeof(check));
		long x = winnstr(stdscr, dummy, 5);
		int x1, y1;
		getbegyx(stdscr, y1, x1);
		(void)check2;
		(void)x;
		(void)y1;
		(void)x1;
		],
		[cf_cv_need_xopen_extension=$cf_try_xopen_extension; break])
	done
	])
])

case "$cf_cv_need_xopen_extension" in
(*_*)
	CF_APPEND_TEXT(CPPFLAGS,-D$cf_cv_need_xopen_extension)
	;;
esac

])dnl
dnl ---------------------------------------------------------------------------
dnl CF_XOPEN_SOURCE version: 68 updated: 2024/11/09 18:07:29
dnl ---------------
dnl Try to get _XOPEN_SOURCE defined properly that we can use POSIX functions,
dnl or adapt to the vendor's definitions to get equivalent functionality,
dnl without losing the common non-POSIX features.
dnl
dnl Parameters:
dnl	$1 is the nominal value for _XOPEN_SOURCE
dnl	$2 is the nominal value for _POSIX_C_SOURCE
dnl
dnl The default case prefers _XOPEN_SOURCE over _POSIX_C_SOURCE if the
dnl implementation predefines it, because X/Open and most implementations agree
dnl that the latter is a legacy or "aligned" value.
dnl
dnl Because _XOPEN_SOURCE is preferred, if defining _POSIX_C_SOURCE turns
dnl that off, then refrain from setting _POSIX_C_SOURCE explicitly.
dnl
dnl References:
dnl https://pubs.opengroup.org/onlinepubs/007904975/functions/xsh_chap02_02.html
dnl https://docs.oracle.com/cd/E19253-01/816-5175/standards-5/index.html
dnl https://www.gnu.org/software/libc/manual/html_node/Feature-Test-Macros.html
AC_DEFUN([CF_XOPEN_SOURCE],[
AC_REQUIRE([AC_CANONICAL_HOST])
AC_REQUIRE([CF_POSIX_VISIBLE])

if test "$cf_cv_posix_visible" = no; then

cf_XOPEN_SOURCE=ifelse([$1],,500,[$1])
cf_POSIX_C_SOURCE=ifelse([$2],,199506L,[$2])
cf_xopen_source=

case "$host_os" in
(aix[[4-7]]*)
	cf_xopen_source="-D_ALL_SOURCE"
	;;
(darwin[[0-8]].*)
	cf_xopen_source="-D_APPLE_C_SOURCE"
	;;
(darwin*)
	cf_xopen_source="-D_DARWIN_C_SOURCE"
	cf_XOPEN_SOURCE=
	;;
(freebsd*|dragonfly*|midnightbsd*)
	# 5.x headers associate
	#	_XOPEN_SOURCE=600 with _POSIX_C_SOURCE=200112L
	#	_XOPEN_SOURCE=500 with _POSIX_C_SOURCE=199506L
	cf_POSIX_C_SOURCE=200112L
	cf_XOPEN_SOURCE=600
	cf_xopen_source="-D_BSD_TYPES -D__BSD_VISIBLE -D_POSIX_C_SOURCE=$cf_POSIX_C_SOURCE -D_XOPEN_SOURCE=$cf_XOPEN_SOURCE"
	;;
(hpux11*)
	cf_xopen_source="-D_HPUX_SOURCE -D_XOPEN_SOURCE=500"
	;;
(hpux*)
	cf_xopen_source="-D_HPUX_SOURCE"
	;;
(irix[[56]].*)
	cf_xopen_source="-D_SGI_SOURCE"
	cf_XOPEN_SOURCE=
	;;
(linux*gnu|linux*gnuabi64|linux*gnuabin32|linux*gnueabi|linux*gnueabihf|linux*gnux32|uclinux*|gnu*|mint*|k*bsd*-gnu|cygwin|msys|mingw*|linux*uclibc)
	CF_GNU_SOURCE($cf_XOPEN_SOURCE)
	;;
linux*musl)
	cf_xopen_source="-D_BSD_SOURCE"
	;;
(minix*)
	cf_xopen_source="-D_NETBSD_SOURCE" # POSIX.1-2001 features are ifdef'd with this...
	;;
(mirbsd*)
	# setting _XOPEN_SOURCE or _POSIX_SOURCE breaks <sys/select.h> and other headers which use u_int / u_short types
	cf_XOPEN_SOURCE=
	CF_POSIX_C_SOURCE($cf_POSIX_C_SOURCE)
	;;
(netbsd*)
	cf_xopen_source="-D_NETBSD_SOURCE" # setting _XOPEN_SOURCE breaks IPv6 for lynx on NetBSD 1.6, breaks xterm, is not needed for ncursesw
	;;
(openbsd[[6-9]]*)
	# OpenBSD 6.x has broken locale support, both compile-time and runtime.
	# see https://www.mail-archive.com/bugs@openbsd.org/msg13200.html
	# Abusing the conformance level is a workaround.
	AC_MSG_WARN(this system does not provide usable locale support)
	cf_xopen_source="-D_BSD_SOURCE"
	cf_XOPEN_SOURCE=700
	;;
(openbsd[[4-5]]*)
	# setting _XOPEN_SOURCE lower than 500 breaks g++ compile with wchar.h, needed for ncursesw
	cf_xopen_source="-D_BSD_SOURCE"
	cf_XOPEN_SOURCE=600
	;;
(openbsd*)
	# setting _XOPEN_SOURCE breaks xterm on OpenBSD 2.8, is not needed for ncursesw
	;;
(osf[[45]]*)
	cf_xopen_source="-D_OSF_SOURCE"
	;;
(nto-qnx*)
	cf_xopen_source="-D_QNX_SOURCE"
	;;
(sco*)
	# setting _XOPEN_SOURCE breaks Lynx on SCO Unix / OpenServer
	;;
(solaris2.*)
	cf_xopen_source="-D__EXTENSIONS__"
	cf_cv_xopen_source=broken
	;;
(sysv4.2uw2.*) # Novell/SCO UnixWare 2.x (tested on 2.1.2)
	cf_XOPEN_SOURCE=
	cf_POSIX_C_SOURCE=
	;;
(*)
	CF_TRY_XOPEN_SOURCE
	cf_save_xopen_cppflags="$CPPFLAGS"
	CF_POSIX_C_SOURCE($cf_POSIX_C_SOURCE)
	# Some of these niche implementations use copy/paste, double-check...
	if test "$cf_cv_xopen_source" = no ; then
		CF_VERBOSE(checking if _POSIX_C_SOURCE interferes with _XOPEN_SOURCE)
		AC_TRY_COMPILE(CF__XOPEN_SOURCE_HEAD,CF__XOPEN_SOURCE_BODY,,[
			AC_MSG_WARN(_POSIX_C_SOURCE definition is not usable)
			CPPFLAGS="$cf_save_xopen_cppflags"])
	fi
	;;
esac

if test -n "$cf_xopen_source" ; then
	CF_APPEND_CFLAGS($cf_xopen_source,true)
fi

dnl In anything but the default case, we may have system-specific setting
dnl which is still not guaranteed to provide all of the entrypoints that
dnl _XOPEN_SOURCE would yield.
if test -n "$cf_XOPEN_SOURCE" && test -z "$cf_cv_xopen_source" ; then
	AC_MSG_CHECKING(if _XOPEN_SOURCE really is set)
	AC_TRY_COMPILE([#include <stdlib.h>],[
#ifndef _XOPEN_SOURCE
#error _XOPEN_SOURCE is not defined
#endif],
	[cf_XOPEN_SOURCE_set=yes],
	[cf_XOPEN_SOURCE_set=no])
	AC_MSG_RESULT($cf_XOPEN_SOURCE_set)
	if test "$cf_XOPEN_SOURCE_set" = yes
	then
		AC_TRY_COMPILE([#include <stdlib.h>],[
#if (_XOPEN_SOURCE - 0) < $cf_XOPEN_SOURCE
#error (_XOPEN_SOURCE - 0) < $cf_XOPEN_SOURCE
#endif],
		[cf_XOPEN_SOURCE_set_ok=yes],
		[cf_XOPEN_SOURCE_set_ok=no])
		if test "$cf_XOPEN_SOURCE_set_ok" = no
		then
			AC_MSG_WARN(_XOPEN_SOURCE is lower than requested)
		fi
	else
		CF_TRY_XOPEN_SOURCE
	fi
fi
fi # cf_cv_posix_visible
])
dnl ---------------------------------------------------------------------------
dnl CF_X_ATHENA version: 25 updated: 2023/01/11 04:05:23
dnl -----------
dnl Check for Xaw (Athena) libraries
dnl
dnl Sets $cf_x_athena according to the flavor of Xaw which is used.
AC_DEFUN([CF_X_ATHENA],
[
cf_x_athena=${cf_x_athena:-Xaw}

AC_MSG_CHECKING(if you want to link with Xaw 3d library)
withval=
AC_ARG_WITH(Xaw3d,
	[  --with-Xaw3d            link with Xaw 3d library])
if test "$withval" = yes ; then
	cf_x_athena=Xaw3d
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
fi

AC_MSG_CHECKING(if you want to link with Xaw 3d xft library)
withval=
AC_ARG_WITH(Xaw3dxft,
	[  --with-Xaw3dxft         link with Xaw 3d xft library])
if test "$withval" = yes ; then
	cf_x_athena=Xaw3dxft
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
fi

AC_MSG_CHECKING(if you want to link with neXT Athena library)
withval=
AC_ARG_WITH(neXtaw,
	[  --with-neXtaw           link with neXT Athena library])
if test "$withval" = yes ; then
	cf_x_athena=neXtaw
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
fi

AC_MSG_CHECKING(if you want to link with Athena-Plus library)
withval=
AC_ARG_WITH(XawPlus,
	[  --with-XawPlus          link with Athena-Plus library])
if test "$withval" = yes ; then
	cf_x_athena=XawPlus
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
fi

cf_x_athena_lib=""

if test "$PKG_CONFIG" != none ; then
	cf_athena_list=
	test "$cf_x_athena" = Xaw && cf_athena_list="xaw8 xaw7 xaw6"
	for cf_athena_pkg in \
		$cf_athena_list \
		${cf_x_athena} \
		${cf_x_athena}-devel \
		lib${cf_x_athena} \
		lib${cf_x_athena}-devel
	do
		CF_TRY_PKG_CONFIG($cf_athena_pkg,[
			cf_x_athena_lib="$cf_pkgconfig_libs"
			CF_UPPER(cf_x_athena_LIBS,HAVE_LIB_$cf_x_athena)
			AC_DEFINE_UNQUOTED($cf_x_athena_LIBS)

			CF_TRIM_X_LIBS

AC_CACHE_CHECK(for usable $cf_x_athena/Xmu package,cf_cv_xaw_compat,[
AC_TRY_LINK([
$ac_includes_default
#include <X11/Xmu/CharSet.h>
],[
int check = XmuCompareISOLatin1("big", "small");
(void)check;
],[cf_cv_xaw_compat=yes],[cf_cv_xaw_compat=no])])

			if test "$cf_cv_xaw_compat" = no
			then
				# workaround for broken ".pc" files...
				case "$cf_x_athena_lib" in
				(*-lXmu*)
					;;
				(*)
					CF_VERBOSE(work around broken package)
					cf_save_xmu="$LIBS"
					cf_first_lib=`echo "$cf_save_xmu" | sed -e 's/^[ ][ ]*//' -e 's/ .*//'`
					CF_TRY_PKG_CONFIG(xmu,[
							LIBS="$cf_save_xmu"
							CF_ADD_LIB_AFTER($cf_first_lib,$cf_pkgconfig_libs)
						],[
							CF_ADD_LIB_AFTER($cf_first_lib,-lXmu)
						])
					CF_TRIM_X_LIBS
					;;
				esac
			fi

			break])
	done
fi

if test -z "$cf_x_athena_lib" ; then
	CF_X_EXT
	CF_X_TOOLKIT
	CF_X_ATHENA_CPPFLAGS($cf_x_athena)
	CF_X_ATHENA_LIBS($cf_x_athena)
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_X_ATHENA_CPPFLAGS version: 9 updated: 2020/12/31 10:54:15
dnl --------------------
dnl Normally invoked by CF_X_ATHENA, with $1 set to the appropriate flavor of
dnl the Athena widgets, e.g., Xaw, Xaw3d, neXtaw.
AC_DEFUN([CF_X_ATHENA_CPPFLAGS],
[
AC_REQUIRE([AC_PATH_XTRA])
cf_x_athena_root=ifelse([$1],,Xaw,[$1])
cf_x_athena_inc=""

for cf_path in default \
	/usr/contrib/X11R6 \
	/usr/contrib/X11R5 \
	/usr/lib/X11R5 \
	/usr/local
do
	if test -z "$cf_x_athena_inc" ; then
		CF_SAVE_XTRA_FLAGS([CF_X_ATHENA_CPPFLAGS])
		cf_test=X11/$cf_x_athena_root/SimpleMenu.h
		if test "$cf_path" != default ; then
			CF_APPEND_TEXT(CPPFLAGS,-I$cf_path/include)
			AC_MSG_CHECKING(for $cf_test in $cf_path)
		else
			AC_MSG_CHECKING(for $cf_test)
		fi
		AC_TRY_COMPILE([
#include <X11/Intrinsic.h>
#include <$cf_test>],[],
			[cf_result=yes],
			[cf_result=no])
		AC_MSG_RESULT($cf_result)
		CF_RESTORE_XTRA_FLAGS([CF_X_ATHENA_CPPFLAGS])
		if test "$cf_result" = yes ; then
			test "$cf_path"  = default && cf_x_athena_inc=default
			test "$cf_path" != default && cf_x_athena_inc="$cf_path/include"
			break
		fi
	fi
done

if test -z "$cf_x_athena_inc" ; then
	AC_MSG_WARN([Unable to find Athena header files])
elif test "$cf_x_athena_inc" != default ; then
	CF_APPEND_TEXT(CPPFLAGS,-I$cf_x_athena_inc)
fi
])
dnl ---------------------------------------------------------------------------
dnl CF_X_ATHENA_LIBS version: 14 updated: 2023/01/11 04:05:23
dnl ----------------
dnl Normally invoked by CF_X_ATHENA, with $1 set to the appropriate flavor of
dnl the Athena widgets, e.g., Xaw, Xaw3d, neXtaw.
AC_DEFUN([CF_X_ATHENA_LIBS],
[AC_REQUIRE([CF_X_TOOLKIT])
cf_x_athena_root=ifelse([$1],,Xaw,[$1])
cf_x_athena_lib=""

for cf_path in default \
	/usr/contrib/X11R6 \
	/usr/contrib/X11R5 \
	/usr/lib/X11R5 \
	/usr/local
do
	for cf_lib in \
		${cf_x_athena_root} \
		${cf_x_athena_root}7 \
		${cf_x_athena_root}6
	do
	for cf_libs in \
		"-l$cf_lib -lXmu" \
		"-l$cf_lib -lXpm -lXmu" \
		"-l${cf_lib}_s -lXmu_s"
	do
		test -n "$cf_x_athena_lib" && break

		CF_SAVE_XTRA_FLAGS([CF_X_ATHENA_LIBS])
		cf_test=XawSimpleMenuAddGlobalActions
		test "$cf_path" != default && cf_libs="-L$cf_path/lib $cf_libs"
		CF_ADD_LIBS($cf_libs)
		AC_MSG_CHECKING(for $cf_test in $cf_libs)
		AC_TRY_LINK([
$ac_includes_default
#include <X11/Intrinsic.h>
#include <X11/$cf_x_athena_root/SimpleMenu.h>
],[
$cf_test((XtAppContext) 0)],
			[cf_result=yes],
			[cf_result=no])
		AC_MSG_RESULT($cf_result)
		CF_RESTORE_XTRA_FLAGS([CF_X_ATHENA_LIBS])

		if test "$cf_result" = yes ; then
			cf_x_athena_lib="$cf_libs"
			break
		fi
	done # cf_libs
		test -n "$cf_x_athena_lib" && break
	done # cf_lib
done

if test -z "$cf_x_athena_lib" ; then
	AC_MSG_ERROR(
[Unable to successfully link Athena library (-l$cf_x_athena_root) with test program])
fi

CF_ADD_LIBS($cf_x_athena_lib)
CF_UPPER(cf_x_athena_LIBS,HAVE_LIB_$cf_x_athena)
AC_DEFINE_UNQUOTED($cf_x_athena_LIBS)
])
dnl ---------------------------------------------------------------------------
dnl CF_X_EXT version: 3 updated: 2010/06/02 05:03:05
dnl --------
AC_DEFUN([CF_X_EXT],[
CF_TRY_PKG_CONFIG(Xext,,[
	AC_CHECK_LIB(Xext,XextCreateExtension,
		[CF_ADD_LIB(Xext)])])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_X_TOOLKIT version: 27 updated: 2023/01/11 04:05:23
dnl ------------
dnl Check for X Toolkit libraries
AC_DEFUN([CF_X_TOOLKIT],
[
AC_REQUIRE([AC_PATH_XTRA])
AC_REQUIRE([CF_CHECK_CACHE])

# OSX is schizoid about who owns /usr/X11 (old) versus /opt/X11 (new) (and
# in some cases has installed dummy files in the former, other cases replaced
# it with a link to the new location).  This complicates the configure script.
# Check for that pitfall, and recover using pkg-config
#
# If none of these are set, the configuration is almost certainly broken.
if test -z "${X_CFLAGS}${X_PRE_LIBS}${X_LIBS}${X_EXTRA_LIBS}"
then
	CF_TRY_PKG_CONFIG(x11,,[AC_MSG_WARN(unable to find X11 library)])
	CF_TRY_PKG_CONFIG(ice,,[AC_MSG_WARN(unable to find ICE library)])
	CF_TRY_PKG_CONFIG(sm,,[AC_MSG_WARN(unable to find SM library)])
	CF_TRY_PKG_CONFIG(xt,,[AC_MSG_WARN(unable to find Xt library)])
else
	LIBS="$X_PRE_LIBS $LIBS $X_EXTRA_LIBS"
fi

cf_have_X_LIBS=no

CF_TRY_PKG_CONFIG(xt,[

	case "x$LIBS" in
	(*-lX11*)
		;;
	(*)
# we have an "xt" package, but it may omit Xt's dependency on X11
AC_CACHE_CHECK(for usable X dependency,cf_cv_xt_x11_compat,[
AC_TRY_LINK([
$ac_includes_default
#include <X11/Xlib.h>
],[
	int rc1 = XDrawLine((Display*) 0, (Drawable) 0, (GC) 0, 0, 0, 0, 0);
	int rc2 = XClearWindow((Display*) 0, (Window) 0);
	int rc3 = XMoveWindow((Display*) 0, (Window) 0, 0, 0);
	int rc4 = XMoveResizeWindow((Display*)0, (Window)0, 0, 0, 0, 0);
],[cf_cv_xt_x11_compat=yes],[cf_cv_xt_x11_compat=no])])
		if test "$cf_cv_xt_x11_compat" = no
		then
			CF_VERBOSE(work around broken X11 dependency)
			# 2010/11/19 - good enough until a working Xt on Xcb is delivered.
			CF_TRY_PKG_CONFIG(x11,,[CF_ADD_LIB_AFTER(-lXt,-lX11)])
		fi
		;;
	esac

AC_CACHE_CHECK(for usable X Toolkit package,cf_cv_xt_ice_compat,[
AC_TRY_LINK([
$ac_includes_default
#include <X11/Shell.h>
],[int num = IceConnectionNumber(0); (void) num
],[cf_cv_xt_ice_compat=yes],[cf_cv_xt_ice_compat=no])])

	if test "$cf_cv_xt_ice_compat" = no
	then
		# workaround for broken ".pc" files used for X Toolkit.
		case "x$X_PRE_LIBS" in
		(*-lICE*)
			case "x$LIBS" in
			(*-lICE*)
				;;
			(*)
				CF_VERBOSE(work around broken ICE dependency)
				CF_TRY_PKG_CONFIG(ice,
					[CF_TRY_PKG_CONFIG(sm)],
					[CF_ADD_LIB_AFTER(-lXt,$X_PRE_LIBS)])
				;;
			esac
			;;
		esac
	fi

	cf_have_X_LIBS=yes
],[

	LDFLAGS="$X_LIBS $LDFLAGS"
	CF_CHECK_CFLAGS($X_CFLAGS)

	AC_CHECK_FUNC(XOpenDisplay,,[
	AC_CHECK_LIB(X11,XOpenDisplay,
		[CF_ADD_LIB(X11)])])

	AC_CHECK_FUNC(XtAppInitialize,,[
	AC_CHECK_LIB(Xt, XtAppInitialize,
		[AC_DEFINE(HAVE_LIBXT,1,[Define to 1 if we can compile with the Xt library])
		 cf_have_X_LIBS=Xt
		 LIBS="-lXt $LIBS"])])
])

if test "$cf_have_X_LIBS" = no ; then
	AC_MSG_WARN(
[Unable to successfully link X Toolkit library (-lXt) with
test program.  You will have to check and add the proper libraries by hand
to makefile.])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF__CURSES_HEAD version: 2 updated: 2010/10/23 15:54:49
dnl ---------------
dnl Define a reusable chunk which includes <curses.h> and <term.h> when they
dnl are both available.
define([CF__CURSES_HEAD],[
#ifdef HAVE_XCURSES
#include <xcurses.h>
char * XCursesProgramName = "test";
#else
#include <${cf_cv_ncurses_header:-curses.h}>
#if defined(NCURSES_VERSION) && defined(HAVE_NCURSESW_TERM_H)
#include <ncursesw/term.h>
#elif defined(NCURSES_VERSION) && defined(HAVE_NCURSES_TERM_H)
#include <ncurses/term.h>
#elif defined(HAVE_TERM_H)
#include <term.h>
#endif
#endif
])
dnl ---------------------------------------------------------------------------
dnl CF__SED_TRIMBLANKS version: 1 updated: 2021/01/02 09:31:20
dnl ------------------
dnl Trim something using sed, then trim extra whitespace
dnl $1 = extra parameters, e.g., in CF_STRIP_G_OPT
define([CF__SED_TRIMBLANKS],[sed ifelse($1,,,[$1] )-e 's%[[	]]% %g' -e 's% [[ ]]*% %g' -e 's%^ %%' -e 's% [$]%%'])dnl
dnl ---------------------------------------------------------------------------
dnl CF__XOPEN_SOURCE_BODY version: 2 updated: 2023/02/18 17:41:25
dnl ---------------------
dnl body of test when test-compiling for _XOPEN_SOURCE check
define([CF__XOPEN_SOURCE_BODY],
[
#ifndef _XOPEN_SOURCE
#error _XOPEN_SOURCE is not defined
#endif
])
dnl ---------------------------------------------------------------------------
dnl CF__XOPEN_SOURCE_HEAD version: 2 updated: 2023/02/18 17:41:25
dnl ---------------------
dnl headers to include when test-compiling for _XOPEN_SOURCE check
define([CF__XOPEN_SOURCE_HEAD],
[
$ac_includes_default
])
