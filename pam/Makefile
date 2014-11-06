.PHONY: all clean install uninstall 

CFLAGS := -fPIC -DPIC -shared -rdynamic 

LBITS := $(shell getconf LONG_BIT)

ifeq ($(LBITS),64)
	INSTALLDIR := $(DESTDIR)/lib/x86_64-linux-gnu/security
else
	INSTALLDIR := $(DESTDIR)/lib/i386-linux-gnu/security
endif

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

