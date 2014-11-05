chrootfs
========

chrootfs an overlay fuse filesystem. It allows you to hide certain 
files and directories from the underlaying filesystem. You can 
use this filesystem for providing chroot environments to users 
without copying any libraries or binaries. 

## Installation
You can install chrootfs by running "make" and "make install". Finally, you need to enable the option *user_allow_other* in your FUSE configuration (*/etc/fuse.conf*). 

### Dependencies

chrootfs depends on fuse, pkg-config, bison, flex and pam

```
apt-get install fuse libfuse-dev pkg-config bison flex libpam0g-dev
```

## Example 
Before:
```
test@tusnelda:~$ ls -ld /etc/squid
drwxr-xr-x 2 root root 4096 Jun 12  2013 /etc/squid
test@tusnelda:~$ ls -l /etc/squid
insgesamt 152
-rw------- 1 root root 149182 Feb 23  2009 squid.conf
test@tusnelda:~$ ls -l /home
insgesamt 60
drwxrwxr-x  41 cvs     cvs    4096 Mai 16  2010 cvs
drwxr-xr-x 185 kristof users 24576 Okt 31 14:21 kristof
drwx------   2 root    root   4096 Apr  3  2005 lost+found
drwxrwxrwx   4 root    staff  4096 Jun 16  2005 profiles
drwxr-xr-x  37 svn     svn    4096 Apr  6  2009 svn
drwxr-xr-x   9 test    users  4096 Nov  5  2012 test
test@tusnelda:~$
```

Run chrootfs and chroot user:
```
/usr/bin/chrootfs /opt/chroot/test -o allow_root
su 
chroot /opt/chroot/test
su - test
```

After:
```
test@tusnelda:~$ ls -ld /etc/squid
ls: cannot access /etc/squid: No such file or directory
test@tusnelda:~$ ls -l /etc/squid
ls: cannot access /etc/squid: No such file or directory
test@tusnelda:~$ ls -l /home
insgesamt 4
drwxr-xr-x 9 test users 4096 Nov  5  2012 test
test@tusnelda:~$
```
## Configuration
The configuration file of chrootfs is */etc/chrootfs.conf*
```
// Sample configuration for chrootfs

hide {
        /etc/ppp
        /etc/wvdial.conf
        /etc/squid
}

show_only_user {
        /home
}
```
