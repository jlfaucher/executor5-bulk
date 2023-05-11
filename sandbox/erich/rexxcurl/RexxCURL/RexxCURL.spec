%define debug_package %{nil}
%bcond_with source
Summary: Rexx/CURL is a Rexx external function package for interfacing to cURL.
%if "%{myrexx}" == "regina"
Name: RexxCURL-regina
Conflicts: RexxCURL-oorexx RexxCURL-rexxtrans
%endif
%if "%{myrexx}" == "oorexx"
Name: RexxCURL-oorexx
Conflicts: RexxCURL-regina RexxCURL-rexxtrans
%endif
%if "%{myrexx}" == "rexxtrans"
Name: RexxCURL-rexxtrans
Conflicts: RexxCURL-oorexx RexxCURL-regina
%endif
%if %{with source}
Name: RexxCURL
%endif
Version: %{myversion}
Release: 1
License: CPL
Group: Development/Languages
Source: RexxCURL-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-root
Vendor: Mark Hessling
Packager: Mark Hessling
URL: http://rexxcurl.sourceforge.net
Provides: %{name} RexxCURL
Prefix: /usr

%description
Rexx/CURL is an external function package for interfacing to cURL, 7.8 or
above.  It enables Rexx programmers to manipulate URL based resources
by calling functions that wrap the cURL easy functions provided in libcurl.
It is also available on several other operating systems.

For more information on Rexx/CURL, visit http://rexxcurl.sourceforge.net/
For more information on Rexx, visit http://www.rexxla.org/
For more information on cURL, visit http://curl.haxx.se/

%package devel
Summary: Rexx/CURL development libraries for embedding in interpreter.

%description devel
Rexx/CURL is an external function package for interfacing to cURL, 7.8 or
above.  It enables Rexx programmers to manipulate URL based resources
by calling functions that wrap the cURL easy functions provided in libcurl.
It is also available on several other operating systems.

For more information on Rexx/CURL, visit http://rexxcurl.sourceforge.net/
For more information on Rexx, visit http://www.rexxla.org/
For more information on cURL, visit http://curl.haxx.se/

%package lib
Summary: Rexx/CURL external function package for %{myrexx} interpreter.

%description lib
Rexx/CURL is an external function package for interfacing to cURL, 7.8 or
above.  It enables Rexx programmers to manipulate URL based resources
by calling functions that wrap the cURL easy functions provided in libcurl.
It is also available on several other operating systems.

For more information on Rexx/CURL, visit http://rexxcurl.sourceforge.net/
For more information on Rexx, visit http://www.rexxla.org/
For more information on cURL, visit http://curl.haxx.se/

%package -n RexxCURL-doc
Summary: Rexx/CURL documentation
BuildArch: noarch

%description -n RexxCURL-doc
Rexx/CURL is an external function package for interfacing to cURL, 7.8 or
above.  It enables Rexx programmers to manipulate URL based resources
by calling functions that wrap the cURL easy functions provided in libcurl.
It is also available on several other operating systems.

For more information on Rexx/CURL, visit http://rexxcurl.sourceforge.net/
For more information on Rexx, visit http://www.rexxla.org/
For more information on cURL, visit http://curl.haxx.se/

%if 0%{?suse_version} > 910
%define mydocdir %{prefix}/share/doc/packages/%{name}
%else
%if 0%{?mdkversion} > 2006
%define mydocdir %{prefix}/share/doc/%{name}
%else
%define mydocdir %{prefix}/share/doc/%{name}-%{version}
%endif
%endif

%prep
%setup -n RexxCURL-%{version}

%build
./configure --prefix=%{prefix} --with-docdir=%{mydocdir} --with-rexx=%{myrexx}
make all html

%install
rm -fr %{buildroot}
%if "%{myrexx}" == "regina"
make DESTDIR=%{buildroot} installtools installaddons installdev installdoc
%else
make DESTDIR=%{buildroot} install
%endif

%files
%{_bindir}/rexxcurl

%files devel
%{_bindir}/rexxcurl-config
%{_libdir}/librexxcurl.a

%files lib
%if "%{myrexx}" == "regina"
/usr%{addonsdir}/librexxcurl.so
%else
/usr/%{_lib}/librexxcurl.so
%endif

%files -n RexxCURL-doc
%{_datadir}/rexxcurl/testcurl.rexx
%{_datadir}/rexxcurl/httppost.rexx
%{_datadir}/rexxcurl/scp.rexx
%{_datadir}/rexxcurl/upload.rexx
%{_datadir}/rexxcurl/getright.rexx
%{_datadir}/rexxcurl/getmail-pop3.rexx
%{_datadir}/rexxcurl/sendmail-smtp.rexx
%{_datadir}/rexxcurl/README.txt
%{mydocdir}/CPLv1.0.txt
%{mydocdir}/index.html
%{mydocdir}/images/

%post
ldconfig %{_libdir}
