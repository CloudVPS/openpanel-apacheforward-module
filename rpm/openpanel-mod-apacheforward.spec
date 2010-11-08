# This file is part of OpenPanel - The Open Source Control Panel
# OpenPanel is free software: you can redistribute it and/or modify it 
# under the terms of the GNU General Public License as published by the Free 
# Software Foundation, using version 3 of the License.
#
# Please note that use of the OpenPanel trademark may be subject to additional 
# restrictions. For more information, please visit the Legal Information 
# section of the OpenPanel website on http://www.openpanel.com/

%define version 0.9.3

%define libpath /usr/lib
%ifarch x86_64
  %define libpath /usr/lib64
%endif

Summary: Configure the apache2 webserver for http redirects / framed forwards
Name: openpanel-mod-apacheforward
Version: %version
Release: 1
License: GPLv2
Group: Development
Source: http://packages.openpanel.com/archive/openpanel-mod-apacheforward-%{version}.tar.gz
BuildRoot: /var/tmp/%{name}-buildroot
Requires: openpanel-core >= 0.8.3
Requires: openpanel-mod-user
Requires: openpanel-mod-domain
Requires: openpanel-mod-apache2

%description
Configure the apache2 webserver for http redirects / framed forwards
Openpanel website forwarding module

%prep
%setup -q -n openpanel-mod-apacheforward-%version

%build
BUILD_ROOT=$RPM_BUILD_ROOT
./configure
make

%install
BUILD_ROOT=$RPM_BUILD_ROOT
rm -rf ${BUILD_ROOT}
mkdir -p ${BUILD_ROOT}/var/opencore/modules/ApacheForward.module
mkdir -p ${BUILD_ROOT}/var/opencore/conf/staging/ApacheForward
mkdir -p ${BUILD_ROOT}/var/www
install -m 755 action ${BUILD_ROOT}/var/opencore/modules/ApacheForward.module/
cp module.xml *.html ${BUILD_ROOT}/var/opencore/modules/ApacheForward.module/
install -m 755 verify ${BUILD_ROOT}/var/opencore/modules/ApacheForward.module/

%post
chown opencore:authd /var/opencore/conf/staging/ApacheForward
service httpd restart >/dev/null 2>&1

%files
%defattr(-,root,root)
/
