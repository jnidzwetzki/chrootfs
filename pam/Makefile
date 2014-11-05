.PHONY: all clean install uninstall 

CFLAGS := -fPIC -DPIC -shared -rdynamic 

INSTALLDIR := $(DESTDIR)/lib/i386-linux-gnu/security

all: chrootfs_pam.so

chrootfs_pam.so: chrootfs_pam.c 
	$(CC) $(CFLAGS) -o chrootfs_pam.so chrootfs_pam.c 

clean:
	rm -f chrootfs_pam.so

install: chrootfs_pam.so
	cp chrootfs_pam.so $(INSTALLDIR)
	
uninstall:
	rm -f $(INSTALLDIR)/chrootfs_pam.so

