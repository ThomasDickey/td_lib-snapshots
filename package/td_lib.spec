Summary: Thomas Dickeys library
%define AppProgram td_lib
%define AppVersion 12.x
%define AppRelease 20250117
# $Id: td_lib.spec,v 1.45 2025/01/18 00:16:32 tom Exp $
Name: %{AppProgram}
Version: %{AppVersion}
Release: %{AppRelease}
License: MIT-X11
Group: Development/Libraries
URL: https://invisible-island.net/ded
Source0: https://invisible-island.net/archives/ded/%{AppProgram}-%{AppRelease}.tgz
Vendor: Thomas Dickey <dickey@invisible-island.net>

%description
This is a library used for building ded and related programs.
It includes (n)curses support for ded, which can be for any
curses implementation.  The ncursesw library is more useful
to ded.

%prep

# no need for debugging symbols...
%define debug_package %{nil}

%setup -q -n %{AppProgram}-%{AppRelease}

%build

INSTALL_PROGRAM='${INSTALL}' \
%configure \
		--target %{_target_platform} \
		--prefix=%{_prefix} \
		--bindir=%{_bindir} \
		--includedir=%{_includedir} \
		--libdir=%{_libdir} \
		--mandir=%{_mandir} \
		--datadir=%{_datadir} \
		--disable-echo \
		--disable-selflink \
		--enable-pc-files \
		--with-man2html \
		--with-screen=ncursesw6

make

%install

[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

make install DESTDIR=$RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{_libdir}/libtd.a
%{_libdir}/pkgconfig/td_lib.pc
%{_datadir}/td/td_lib.mk
%{_datadir}/td/man2html.sh
%{_includedir}/td/cmv_defs.h
%{_includedir}/td/deltree.h
%{_includedir}/td/dyn_str.h
%{_includedir}/td/port2vms.h
%{_includedir}/td/ptypes.h
%{_includedir}/td/rcsdefs.h
%{_includedir}/td/sccsdefs.h
%{_includedir}/td/td_btree.h
%{_includedir}/td/td_config.h
%{_includedir}/td/td_curse.h
%{_includedir}/td/td_ext.h
%{_includedir}/td/td_lib.h
%{_includedir}/td/td_local.h
%{_includedir}/td/td_qsort.h
%{_includedir}/td/td_regex.h
%{_includedir}/td/td_scomp.h
%{_includedir}/td/td_sheet.h
%{_includedir}/td/vms_dirs.h

%changelog
# each patch should add its ChangeLog entries here

* Thu Jan 19 2023 Thomas Dickey
- add td_lib.pc to build

* Tue Nov 21 2017 Thomas Dickey
- add man2html.sh to build

* Fri Jul 09 2010 Thomas Dickey
- implement ded "-m" mouse option

* Sat Jul 03 2010 Thomas Dickey
- initial version
