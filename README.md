chrootfs
========

chrootfs an overlay fuse filesystem. It allows to hide certain 
files and directories from the underlaying filesystem. You can 
use this filesystem for providing chroot environments to users 
without copying any libraries or binaries. 

## Installation
You can install chrootfs by running "make" and "make install". Finally, you need to enable the option *user_allow_other* in your FUSE configuration (*/etc/fuse.conf*). 

## Usage 
Example:
```
/usr/bin/chrootfs /opt/chroot/user1
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
