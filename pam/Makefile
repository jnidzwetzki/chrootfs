.PHONY: all clean install uninstall 

CFLAGS := -fPIC -DPIC -shared -rdynamic 
CFLAGS += -I../lib
CFLAGS += -Wl,-whole-archive ../lib/libchrootfs.a -Wl,-no-whole-archive

LBITS := $(shell getconf LONG_BIT)

INSTALLDIR := $(DESTDIR)/lib/security

all: pam_chrootfs.so

pam_chrootfs.so: pam_chrootfs.c 
	$(CC) $(CFLAGS) -o pam_chrootfs.so pam_chrootfs.c 

clean:
	rm -f pam_chrootfs.so

install: pam_chrootfs.so
	install -d -m 0755 $(DESTDIR)/var/chrootfs
	install -d -m 0755 $(INSTALLDIR)
	install -o root -g root -m 0644 pam_chrootfs.so $(INSTALLDIR)/pam_chrootfs.so

uninstall:
	rm -f $(INSTALLDIR)/pam_chrootfs.so

