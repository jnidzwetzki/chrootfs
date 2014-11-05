.PHONY: all clean install uninstall 

CFLAGS := -fPIC -DPIC -shared -rdynamic 

LBITS := $(shell getconf LONG_BIT)

ifeq ($(LBITS),64)
	INSTALLDIR := $(DESTDIR)/lib/x86_64-linux-gnu/security
else
	INSTALLDIR := $(DESTDIR)/lib/i386-linux-gnu/security
endif

all: chrootfs_pam.so

chrootfs_pam.so: chrootfs_pam.c 
	$(CC) $(CFLAGS) -o chrootfs_pam.so chrootfs_pam.c 

clean:
	rm -f chrootfs_pam.so

install: chrootfs_pam.so
	mkdir -p $(INSTALLDIR)
	cp chrootfs_pam.so $(INSTALLDIR)

uninstall:
	rm -f $(INSTALLDIR)/chrootfs_pam.so

