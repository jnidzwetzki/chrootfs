.PHONY: all clean install uninstall 

CFLAGS := -fPIC -DPIC -shared -rdynamic 

LBITS := $(shell getconf LONG_BIT)

INSTALLDIR := $(DESTDIR)/lib/security

all: chrootfs_pam.so

chrootfs_pam.so: pam_chrootfs.c 
	$(CC) $(CFLAGS) -o pam_chrootfs.so pam_chrootfs.c 

clean:
	rm -f pam_chrootfs.so

install: pam_chrootfs.so
	mkdir -p $(INSTALLDIR)
	cp pam_chrootfs.so $(INSTALLDIR)
	mkdir -p $(DESTDIR)/var/chrootfs

uninstall:
	rm -f $(INSTALLDIR)/pam_chrootfs.so

