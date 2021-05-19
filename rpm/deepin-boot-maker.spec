%define pkgrelease  1
%if 0%{?openeuler}
%define specrelease %{pkgrelease}
%else
## allow specrelease to have configurable %%{?dist} tag in other distribution
%define specrelease %{pkgrelease}%{?dist}
%endif

Name:       deepin-boot-maker
Version:    5.6.14
Release:    %{specrelease}
Summary:    Simple tool to write system image files into CD/DVD, USB flash drive and other media.
License:    GPLv3
URL:        https://github.com/linuxdeepin/%{name}
Packager:   LiuRui
Source0:    %{name}_%{version}.orig.tar.xz


BuildRequires: python3
BuildRequires: qt5-devel
BuildRequires: xcb-util-devel
BuildRequires: mtdev-devel
BuildRequires: libXrender-devel
BuildRequires: glib2-devel
BuildRequires: fontconfig-devel
BuildRequires: mesa-libEGL-devel
BuildRequires: startup-notification-devel
BuildRequires: dtkwidget-devel
BuildRequires: gtest-devel
BuildRequires: libXext-devel


%ifarch x86_64
Requires: syslinux
%endif
Requires: p7zip
Requires: mtools
Requires: udisks2
Requires: genisoimage


%description
%{summary}.

%prep
%autosetup


%build
# help find (and prefer) qt5 utilities, e.g. qmake, lrelease
export PATH=%{_qt5_bindir}:$PATH
%qmake_qt5 DAPP_VERSION=%{version} DEFINES+="VERSION=%{version}"
%make_build


%install
%make_install  INSTALL_ROOT="%buildroot"

%files
%doc README.md
%license LICENSE
%{_bindir}/%{name}
%{_datadir}/*
/usr/lib/deepin-daemon/deepin-boot-maker-service


%changelog

