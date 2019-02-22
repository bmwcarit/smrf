# -*- rpm-spec -*-
BuildRoot:      /data/build/smrf/package/RPM/smrf
Summary:        smrf base package including executables, libraries and default settings
Name:           smrf
Version:        0.3.2
Release:        1
License:        Apache License, Version 2.0
Group:          unknown
Vendor:         BMW Car IT GmbH (http://www.bmw-carit.de)
Url: https://github.com/bmwcarit/smrf

Prefix: /usr

%define _rpmdir /data/build/smrf/package/RPM
%define _unpackaged_files_terminate_build 0
%define _topdir /data/build/smrf/package/RPM
%debug_package

%description
The Secure Message wRapper Format (SMRF) aims at providing a unified messaging format for communication between heterogenous endpoints. It fulfils the following requirements:
* Non-encrypted meta-information (e.g. routing information)
* Optional signing of message contents
* Optional compression of message payload
* Optional encryption of message payload

%package devel
Summary: smrf extra development files
Requires: %{name}%{?_isa} = %{version}-%{release}

%description devel
The smrf cmake support files, C++ headers and code generator

%prep
mv $RPM_BUILD_ROOT /data/build/smrf/package/RPM/tmp
%install
if [ -e $RPM_BUILD_ROOT ];
then
  rm -rf $RPM_BUILD_ROOT
fi
mv /data/build/smrf/package/RPM/tmp $RPM_BUILD_ROOT

%clean

%post

%postun

%pre

%preun

%files
%defattr(-,root,root,-)
/usr/lib64/libsmrf*.so*

%files devel
%defattr(-,root,root,-)
%dir "/usr/lib64/cmake"
/usr/lib64/cmake/smrf
/usr/include/smrf

