%define name	tun
%define version	1.1
%define release	6
%define kernel  %(uname -r)
%define devstyl	%(uname -r| awk -F. '{print $1"."$2;}')
#catch that cool kung fu.

#for OpenLinux crunchy goodness
%define	_buildshell	%([ -x /bin/bash2 ] && echo /bin/bash2 || echo /bin/bash )

Name: %{name}
Version: %{version}
Release: %{release}
Copyright: GPL
Group: System/Drivers
Url: http://vtun.sourceforge.net/tun/
Source: http://vtun.sourceforge.net/tun/%{name}-%{version}.tar.gz
Summary: Universal TUN/TAP device driver.
Vendor: Maxim Krasnyansky <max_mk@yahoo.com>
Packager: Bishop Clark (LC957) <bishop@platypus.bc.ca>
BuildRoot: /var/tmp/%{name}-%{version}-build

#doesn't work
#Requires:	kernel=%(uname -r)

%description
  TUN/TAP provides packet reception and transmission for user space programs. 
  It can be viewed as a simple Point-to-Point or Ethernet device, which 
  instead of receiving packets from a physical media, receives them from 
  user space program and instead of sending packets via physical media 
  writes them to the user space program. 

%prep
%setup -n %{name}-%{version}
./configure

%build
make 

# the alteration to the install section is intended to supply ONLY the
# files required for each platform.  Given the magic with the tun
# module checking to see if it needs to be built, some magic with
# %files -f is required.

%install
#okay, you NEED this, Max.  It is for repeat builds.  Stop deleting it.
[ $RPM_BUILD_ROOT != / ] && rm -rf $RPM_BUILD_ROOT

install -m 755 -o root -g root -d $RPM_BUILD_ROOT/lib/modules/%{kernel}/net

#schroedinger's tun.o
cat <<EOF > listA
%defattr(644,root,root)
%doc FAQ README
EOF
if [ -f linux/tun.o ]; then
install -m 644 -o root -g root linux/tun.o $RPM_BUILD_ROOT/lib/modules/%{kernel}/net
 echo "%attr(600,root,root) /lib/modules/"%{kernel}"/net/tun.o" >> listA
fi
install -m 755 -o root -g root -d $RPM_BUILD_ROOT/dev
install -m 755 -o root -g root -d $RPM_BUILD_ROOT/dev/net
if [ %devstyl = 2.4 ]; then 
 mknod $RPM_BUILD_ROOT/dev/net/tun c 10 200
 echo "%attr(600,root,root) /dev/net/tun" >> listA
elif [ %devstyl = 2.2 ]; then
 for I in 0 1 2 3 4 5 6 7 8 9 ; do 
  mknod $RPM_BUILD_ROOT/dev/tun$I c 90 $I
  mknod $RPM_BUILD_ROOT/dev/tap$I c 90 $(($I+128))
  echo "%attr(600,root,root) /dev/tun$I" >> listA
  echo "%attr(600,root,root) /dev/tap$I" >> listA
 done
fi

#/usr/bin/find $RPM_BUILD_ROOT -type f -print > listA
#| sed 's?/var/tmp/tun-1.1-build?%attr(600,root,root) ?' 

%clean
[ $RPM_BUILD_ROOT != / ] && rm -rf $RPM_BUILD_ROOT

%post
#I should do this up near the top...
%if %([ x%devstyl = x2.4 ] && echo 1 || echo 0)
  TEST="char-major-10-200" ; STRING="alias char-major-10-200 tun"
%elseif %([ x%devstyl = x2.2 ] && echo 1 || echo 0)
  TEST="char-major-90" ; STRING="alias char-major-90 tun"
%endif
#add module into the modules.conf
cp /etc/modules.conf /etc/modules.conf.pre-mod
grep -v "$TEST" /etc/modules.conf | \
    diff /etc/modules.conf -  | patch -sb /etc/modules.conf
(cat /etc/modules.conf && echo  "$STRING") | \
    diff /etc/modules.conf -  | patch -s /etc/modules.conf
echo "Please compare /etc/modules.conf{.orig,} and delete the backup (.orig)"
depmod -a

%postun
%if %([ %devstyl = 2.4 ] && echo 1 || echo 0)
  TEST="char-major-10-200" ; STRING="alias char-major-10-200 tun"
%elseif %([ %devstyl = 2.2 ] && echo 1 || echo 0)
  TEST="char-major-90" ; STRING="alias char-major-90 tun"
%endif
#add module into the modules.conf
cp /etc/modules.conf /etc/modules.conf.pre-mod
grep -v "$TEST" /etc/modules.conf | \
    diff /etc/modules.conf -  | patch -sb /etc/modules.conf
echo "Please compare /etc/modules.conf{.orig,} and delete the backup (.orig)"
depmod -a

%files -f listA

%changelog
* Thu Dec 20 2001 Bishop Clark (LC957) <bishop@platypus.bc.ca>         1.1-6
- edit spec file to account for continuing COL/rpm306 builds like on
  20011112
* Mon Dec 03 2001 Bishop Clark (LC957) <bishop@platypus.bc.ca>         1.1-5
- semi-intelligent tun module addition/removal, with a checker
  warning.
* Tue Nov 20 2001 Bishop Clark (LC957) <bishop@platypus.bc.ca>         1.1-4
- tun/tap devices on kernel 2.2 now 0-9 from 0-3.  Cleaned up creation
  routine.
* Fri Nov 16 2001 Bishop Clark (LC957) <bishop@platypus.bc.ca>         1.1-3
- the kernel=%(uname -r)Requires: line doesn't work.  Removing for
  now.
- change to buildshell part to on-the-fly do bash2.  RH62 hated it
  before.
* Mon Nov 12 2001 Bishop Clark (LC957) <bishop@platypus.bc.ca>         1.1-2
- Added minor change to support COL 31 builds
- more commenting, to explain weird stuff.
* Wed Oct 31 2001 Bishop Clark (LC957) <bishop@platypus.bc.ca>         1.1-1lc1
- remove directory so repeat builds succeed (3rd time)
- fixed a schroedinger's tun problem with trying to install or package
  nonexistent files
- only package files required for the target (no 2.4 devs on 2.2)
- more accurately tracks the kernel version
