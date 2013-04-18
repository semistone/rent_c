Summary: dfor load balance library
Name: dfor
Version: 0.0.1
Release: 1
License: GPL
Group: Libraries
Source: dfor.tar-%{version}.gz
URL: https://github.com/semistone/rent_c/tree/master/dfor
Vendor: -
Packager: Angus Chen
#BuildRequires: log4cpp-devel
#BuildRequires: sqlite-devel
Requires: log4cpp
Requires: sqlite 
#Requires: node 
BuildRoot: %{_tmpdir}/%{name}-%{version}-%{release}
%description
name resolve load balance library.

%prep
%setup

%build
make

%install
make install prefix=$RPM_BUILD_ROOT

%files 
%defattr(-, root, root)
"/var/run/dfor/cache.db"
"/usr/local/lib/dfor.so"
"/usr/local/etc/dfor/log4cpp.properties"
"/usr/local/etc/dfor/config.yaml"
"/usr/local/bin/dford.js"
